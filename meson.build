project('graphene', 'c',
        version: '1.6.1',
        default_options: [
          'buildtype=debugoptimized',
          'c_std=c99',
          'warning_level=1'
        ],
        license: 'MIT',
        meson_version: '>= 0.38.1')

graphene_version = meson.project_version()
version_array = graphene_version.split('.')
graphene_major_version = version_array[0].to_int()
graphene_minor_version = version_array[1].to_int()
graphene_micro_version = version_array[2].to_int()

graphene_api_version = '@0@.0'.format(graphene_major_version)

# The interface age is reset every time we add new API; this
# should only happen during development cycles, otherwise the
# interface age is the same as the micro version
if graphene_minor_version.is_odd()
  graphene_interface_age = 0
else
  graphene_interface_age = graphene_micro_version
endif

soversion = 0
# maintaining compatibility with the previous libtool versioning
# current = minor * 100 + micro - interface
# revision = interface
current = graphene_minor_version * 100 + graphene_micro_version - graphene_interface_age
revision = graphene_interface_age
libversion = '@0@.@1@.@2@'.format(soversion, current, revision)

graphene_prefix = get_option('prefix')
graphene_libdir = join_paths(graphene_prefix, get_option('libdir'))
graphene_includedir = join_paths(graphene_prefix, get_option('includedir'))
graphene_datadir = join_paths(graphene_prefix, get_option('datadir'))

graphene_simd = []

cc = meson.get_compiler('c')
host_system = host_machine.system()

conf = configuration_data()

# Compat variables for pkgconfig
conf.set('prefix', graphene_prefix)
conf.set('exec_prefix', graphene_prefix)
conf.set('libdir', graphene_libdir)
conf.set('includedir', graphene_includedir)

# Version macros
conf.set('GRAPHENE_MAJOR_VERSION', graphene_major_version)
conf.set('GRAPHENE_MINOR_VERSION', graphene_minor_version)
conf.set('GRAPHENE_MICRO_VERSION', graphene_micro_version)
conf.set('GRAPHENE_VERSION', graphene_version)

# Compiler flags
if cc.get_id() == 'msvc'
  # Make MSVC more pedantic, this is a recommended pragma list
  # from _Win32_Programming_ by Rector and Newcomer.  Taken from
  # glib's msvc_recommended_pragmas.h--please see that file for
  # the meaning of the warning codes used here
  test_cflags = [
    '-we4002',
    '-we4003',
    '-w14010',
    '-we4013',
    '-w14016',
    '-we4020',
    '-we4021',
    '-we4027',
    '-we4029',
    '-we4033',
    '-we4035',
    '-we4045',
    '-we4047',
    '-we4049',
    '-we4053',
    '-we4071',
    '-we4150',
    '-we4819'
  ]
else
  test_cflags = [
    '-ffast-math',
    '-fstrict-aliasing',
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wformat=2',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wnested-externs',
    '-Wold-style-definition',
    '-Wunused',
    '-Wuninitialized',
    '-Wshadow',
    '-Wmissing-noreturn',
    '-Wmissing-format-attribute',
    '-Wredundant-decls',
    '-Wlogical-op',
    '-Wcast-align',
    '-Wno-unused-local-typedefs',
    '-Werror=implicit',
    '-Werror=init-self',
    '-Werror=main',
    '-Werror=missing-braces',
    '-Werror=return-type',
    '-Werror=array-bounds',
    '-Werror=write-strings',
  ]
endif
common_flags = []
foreach cflag: test_cflags
  if cc.has_argument(cflag)
    common_flags += [ cflag ]
  endif
endforeach

# Headers
conf.set('HAVE_STDLIB_H', cc.has_header('stdlib.h'))
conf.set('HAVE_STDINT_H', cc.has_header('stdint.h'))
conf.set('HAVE_STDBOOL_H', cc.has_header('stdbool.h'))
conf.set('HAVE_MEMORY_H', cc.has_header('memory.h'))

# Required dependencies
mathlib = cc.find_library('m', required: false)
threadlib = dependency('threads', required: true)

# Functions
if cc.has_function('memalign', prefix: '#include <stdlib.h>\n#include <malloc.h>')
  conf.set('HAVE_MEMALIGN', 1)
elif cc.has_function('_aligned_malloc', prefix: '#include <malloc.h>')
  conf.set('HAVE__ALIGNED_MALLOC', 1)
# Don't probe the ones below on Windows because when building with
# MinGW-w64 on MSYS2, Meson<0.37.0 incorrectly detects those below as
# being available even though they're not.
elif cc.has_function('aligned_alloc', prefix: '#include <stdlib.h>') and not (host_system == 'windows')
  conf.set('HAVE_ALIGNED_ALLOC', 1)
elif cc.has_function('posix_memalign', prefix: '#include <stdlib.h>') and not (host_system == 'windows')
  conf.set('HAVE_POSIX_MEMALIGN', 1)
else
  error('No aligned malloc function could be found.')
endif

# Look for sincosf(), a GNU libc extension
conf.set('HAVE_SINCOSF',
         cc.has_function('sincosf',
                         prefix: '#include <math.h>',
                         args: [ '-D_GNU_SOURCE' ],
                         dependencies: mathlib))

# Debugging
debug_flags = []
buildtype = get_option('buildtype')
if buildtype == 'release'
  debug_flags += [ '-DG_DISABLE_ASSERT' ]
elif buildtype == 'debug' or buildtype == 'debugoptimized'
  debug_flags += [ '-DGRAPHENE_ENABLE_DEBUG' ]
endif

extra_args= []
# Detect and set symbol visibility
if get_option('default_library') != 'static'
  if host_system == 'windows'
    conf.set('DLL_EXPORT', true)
    if cc.get_id() == 'msvc'
      conf.set('_GRAPHENE_PUBLIC', '__declspec(dllexport) extern')
    else
      conf.set('_GRAPHENE_PUBLIC', '__attribute__((visibility("default"))) __declspec(dllexport) extern')
      extra_args += ['-fvisibility=hidden']
    endif
  else
    conf.set('_GRAPHENE_PUBLIC', '__attribute__((visibility("default"))) extern')
    extra_args += ['-fvisibility=hidden']
  endif
endif

# Optional dependency on GObject
build_gobject = false
if get_option('enable-gobject-types')
  gobject = dependency('gobject-2.0', version: '>= 2.30.0', required: false)
  build_gobject = gobject.found()
  if build_gobject
    if cc.get_id() == 'msvc'
      extra_args += ['/FImsvc_recommended_pragmas.h']
    endif
  endif
  conf.set('GRAPHENE_REQS', 'gobject-2.0')
endif

# Optional dependency on GObject-Introspection; if GObject is disabled
# then we don't build introspection data either
build_gir = build_gobject
if build_gobject and get_option('enable-introspection')
  # XXX: Not nice, but probably our best option
  gir = find_program('g-ir-scanner', required: false)
  if get_option('default_library') == 'shared'
    build_gir = gir.found() and not meson.is_cross_build()
  else
    build_gir = false
  endif
endif

# Check for InitOnce on Windows
if host_system == 'windows'
  init_once_prog = '''
#define _WIN32_WINNT 0x0600
#include <windows.h>
INIT_ONCE g_InitOnce = INIT_ONCE_STATIC_INIT;
BOOL CALLBACK InitHandleFunc (PINIT_ONCE i, PVOID arg, PVOID *ctx) { return TRUE; }
int main (void) {
  BOOL bInitStatus = InitOnceExecuteOnce (&g_InitOnce, InitHandleFunc, NULL, NULL);
  return 0;
}
'''
  if cc.compiles(init_once_prog, name: 'InitOnceExecuteOnce')
    conf.set('HAVE_INIT_ONCE', 1)
  endif
endif

# Configuration for our installed config header
graphene_conf = configuration_data()

# SSE intrinsics
if get_option('enable-sse2')
  sse_prog = '''
#if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 2))
# if !defined(__amd64__) && !defined(__x86_64__)
#   error "Need GCC >= 4.2 for SSE2 intrinsics on x86"
# endif
#elif defined (_MSC_VER) && (_MSC_VER < 1800)
# if !defined (_M_X64) && !defined (_M_AMD64)
#   error "Need MSVC 2013 or later for SSE2 intrinsics on x86"
# endif
#endif
#if defined(__SSE__) || (_M_IX86_FP > 0) || (_M_X64 > 0) || (_MSC_VER >= 1800)
# include <mmintrin.h>
# include <xmmintrin.h>
# include <emmintrin.h>
#else
# error "No SSE intrinsics available"
#endif
int main () {
    __m128i a = _mm_set1_epi32 (0), b = _mm_set1_epi32 (0), c;
    c = _mm_xor_si128 (a, b);
    return 0;
}'''
  if cc.compiles(sse_prog, name: 'SSE intrinsics')
    graphene_conf.set('GRAPHENE_HAS_SSE', 1)
    if cc.get_id() != 'msvc'
      conf.set('SSE2_CFLAGS', '-mfpmath=sse -msse -msse2')
      extra_args += ['-mfpmath=sse', '-msse', '-msse2']
    endif
    graphene_simd += [ 'sse2' ]
  endif
endif

# GCC vector intrinsics
if get_option('enable-gcc-vector')
  gcc_vector_prog = '''
#if defined(__GNUC__)
# if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
#   error "GCC vector intrinsics are disabled on GCC prior to 4.9"
# elif defined(__arm__)
#   error "GCC vector intrinsics are disabled on ARM"
# endif
#else
# error "Need GCC for GCC vectors intrinsics"
#endif
typedef float simd4f __attribute__((vector_size(16)));
typedef int simd4i __attribute__((vector_size(16)));
int main () {
  simd4f s = { 1.f, 2.f, 3.f, 4.f };
  simd4i m = { 0,   1,   1,   3   };
  simd4f r = __builtin_shuffle (s, m);
  return 0;
}'''
  if cc.compiles(gcc_vector_prog, name: 'GCC vector intrinsics')
    graphene_conf.set('GRAPHENE_HAS_GCC', 1)
    graphene_simd += [ 'gcc' ]
  endif
endif

# ARM NEON intrinsics
if get_option('enable-arm-neon')
  neon_prog = '''
#ifndef __ARM_EABI__
#error "EABI is required (to be sure that calling conventions are compatible)"
#endif
#ifndef __ARM_NEON__
#error "No ARM NEON instructions available"
#endif
#include <arm_neon.h>
int main () {
    const float32_t __v[4] = { 1, 2, 3, 4 }; \
    const unsigned int __umask[4] = { \
      0x80000000, \
      0x80000000, \
      0x80000000, \
      0x80000000 \
    }; \
    const uint32x4_t __mask = vld1q_u32 (__umask); \
    float32x4_t s = vld1q_f32 (__v); \
    float32x4_t c = vreinterpretq_f32_u32 (veorq_u32 (vreinterpretq_u32_f32 (s), __mask)); \
    return 0;
}'''
  if cc.compiles(neon_prog, name: 'ARM NEON intrinsics')
    graphene_conf.set('GRAPHENE_HAS_ARM_NEON', 1)
    extra_args += ['-mfpu=neon']

    if host_system == 'android'
      extra_args += ['-mfloat-abi=softfp']
      conf.set('NEON_CFLAGS', '-mfpu=neon -mfloat-abi=softfp')
    else
      conf.set('NEON_CFLAGS', '-mfpu=neon')
    endif

    graphene_simd += [ 'neon' ]
  endif
endif

# Scalar is always available as a fallback
graphene_simd += [ 'scalar' ]

conf.set('GRAPHENE_SIMD', ' '.join(graphene_simd))

python3 = import('python3')
gnome = import('gnome')

subdir('src')

if get_option('enable-gtk-doc')
  subdir('doc')
endif
