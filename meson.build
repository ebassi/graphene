project('graphene', 'c', default_options: [ 'c_std=gnu99', 'warning_level=1' ], license: 'MIT')

graphene_major_version = 1
graphene_minor_version = 4
graphene_micro_version = 1

graphene_version = '@0@.@1@.@2@'.format(graphene_major_version, graphene_minor_version, graphene_micro_version)
graphene_api_version = '@0@.0'.format(graphene_major_version)
graphene_interface_version = 400 # FIXME: This should be computed dynamically
graphene_lib_version = '0.@0@.@1@'.format(graphene_interface_version, graphene_micro_version)

graphene_prefix = get_option('prefix')
graphene_libdir = graphene_prefix + '/' + get_option('libdir')
graphene_datadir = graphene_prefix + '/share'
graphene_includedir = graphene_prefix + '/include'

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

# Headers
conf.set('HAVE_STDINT_H', cc.has_header('stdint.h'))
conf.set('HAVE_STDBOOL_H', cc.has_header('stdbool.h'))
conf.set('HAVE_MEMORY_H', cc.has_header('memory.h'))

# Functions
conf.set('HAVE_ALIGNED_ALLOC', cc.has_function('aligned_alloc'))
conf.set('HAVE_MEMALIGN', cc.has_function('memalign'))
conf.set('HAVE_POSIX_MEMALIGN', cc.has_function('posix_memalign', prefix: '#include <stdlib.h>'))
conf.set('HAVE_SINCOSF', cc.has_function('sincosf'))

# Debugging
debug_flags = []
if get_option('enable-debug') == 'yes'
  debug_flags += [ '-DGRAPHENE_ENABLE_DEBUG' ]
endif

if get_option('enable-debug') == 'minimum'
  debug_flags += [ '-DGRAPHENE_ENABLE_DEBUG', '-DG_DISABLE_CAST_CHECKS' ]
endif

if get_option('enable-debug') == 'no'
  debug_flags += [
    '-DG_DISABLE_CAST_CHECKS',
    '-DG_DISABLE_CHECKS',
    '-DG_DISABLE_ASSERT'
  ]
endif

# Detect and set symbol visibility
if get_option('default_library') != 'static'
  if host_system == 'windows'
    conf.set('DLL_EXPORT', true)
    if cc.get_id() == 'msvc'
      conf.set('_GRAPHENE_PUBLIC', '__declspec(dllexport) extern')
    else
      conf.set('_GRAPHENE_PUBLIC', '__attribute__((visibility("default"))) __declspec(dllexport) extern')
      add_global_arguments('-fvisibility=hidden', language : 'c')
    endif
  else
    conf.set('_GRAPHENE_PUBLIC', '__attribute__((visibility("default")))')
    add_global_arguments('-fvisibility=hidden', language : 'c')
  endif
endif

# Required dependencies
mathlib = cc.find_library('m', required: true)
threadlib = dependency('threads')

# Optional dependency on GObject
build_gobject = false
if get_option('enable-gobject-types')
  gobject = dependency('gobject-2.0', version: '>= 2.30.0', required: false)
  build_gobject = gobject.found()
  conf.set('GRAPHENE_REQS', 'gobject-2.0')
endif

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

graphene_conf = configuration_data()

# SSE intrinsics
if get_option('enable-sse2')
  sse_prog = '''
#if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 2))
# if !defined(__amd64__) && !defined(__x86_64__)
#   error "Need GCC >= 4.2 for SSE2 intrinsics on x86"
# endif
#endif
#if defined(__SSE__) || (_M_IX86_FP > 0) || (_M_X64 > 0)
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
    conf.set('SSE2_CFLAGS', '-mfpmath=sse -msse -msse2')
    add_global_arguments('-mfpmath=sse', language: 'c')
    add_global_arguments('-msse', language: 'c')
    add_global_arguments('-msse2', language: 'c')
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
    add_global_arguments('-mfpu=neon', language: 'c')

    if host_system == 'android'
      add_global_arguments('-mfloat-abi=softfp', language: 'c')
      conf.set('NEON_CFLAGS', '-mfpu=neon -mfloat-abi=softfp')
    else
      conf.set('NEON_CFLAGS', '-mfpu=neon')
    endif

    graphene_simd += [ 'neon' ]
  endif
endif

# XXX: Would be nice to have a join(CHR) method on ARRAY
simd = ''
foreach s : graphene_simd
  simd = simd + s + ' '
endforeach

simd += 'scalar'

conf.set('GRAPHENE_SIMD', simd)

gnome = import('gnome')

subdir('src')

if get_option('enable-gtk-doc')
  subdir('doc')
endif
