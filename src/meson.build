headers = [
  'graphene-box.h',
  'graphene-euler.h',
  'graphene-frustum.h',
  'graphene-macros.h',
  'graphene-matrix.h',
  'graphene-plane.h',
  'graphene-point.h',
  'graphene-point3d.h',
  'graphene-quad.h',
  'graphene-quaternion.h',
  'graphene-ray.h',
  'graphene-rect.h',
  'graphene-size.h',
  'graphene-sphere.h',
  'graphene-triangle.h',
  'graphene-types.h',
  'graphene-vec2.h',
  'graphene-vec3.h',
  'graphene-vec4.h',
  'graphene-version-macros.h',
]

simd_headers = [
  'graphene-simd4f.h',
  'graphene-simd4x4f.h',
]

private_headers = [
  'graphene-alloc-private.h',
  'graphene-line-segment-private.h',
  'graphene-private.h',
  'graphene-vectors-private.h',
]

sources = [
  'graphene-alloc.c',
  'graphene-box.c',
  'graphene-euler.c',
  'graphene-frustum.c',
  'graphene-matrix.c',
  'graphene-plane.c',
  'graphene-point.c',
  'graphene-point3d.c',
  'graphene-quad.c',
  'graphene-quaternion.c',
  'graphene-ray.c',
  'graphene-rect.c',
  'graphene-size.c',
  'graphene-sphere.c',
  'graphene-triangle.c',
  'graphene-vectors.c'
]

simd_sources = [
  'graphene-simd4f.c',
  'graphene-simd4x4f.c',
]

if build_gobject
  headers += [ 'graphene-gobject.h' ]
  sources += [ 'graphene-gobject.c' ]
endif

# Internal configuration header
configure_file(input: 'config.h.meson',
               output: 'config.h',
               configuration: conf)

# External configuration header
configure_file(input: 'graphene-config.h.meson',
               output: 'graphene-config.h',
               configuration: graphene_conf,
               install: true,
               install_dir: 'lib/graphene-1.0/include')

# Version header
configure_file(input: 'graphene-version.h.in',
               output: 'graphene-version.h',
               configuration: conf,
               install: true,
               install_dir: 'include/graphene-1.0')

install_headers(headers + simd_headers + [ 'graphene.h' ], subdir: 'graphene-1.0')

platform_deps = []

if build_gobject
  platform_deps += [ gobject ]
endif

libgraphene = shared_library('graphene-1.0',
  sources: sources + simd_sources + private_headers,
  version: graphene_version,
  soversion: graphene_lib_version,
  install: true,
  dependencies: [ mathlib, threadlib ] + platform_deps,
  c_args: debug_flags + [
            '-DGRAPHENE_COMPILATION',
            '-DG_LOG_DOMAIN="Graphene"',
            '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_30',
            '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_32' ],
  link_args: [ '-Wl,-Bsymbolic-functions' ])

configure_file(input: 'graphene.pc.in',
               output: 'graphene-1.0.pc',
               configuration: conf,
               install: true,
               install_dir: 'lib/pkgconfig')

graphene_inc = include_directories([ meson.source_root() + '/src', meson.build_root() + '/src' ])
graphene_dep = declare_dependency(link_with: libgraphene,
                                  include_directories: [ graphene_inc ],
                                  dependencies: [ mathlib, threadlib ] + platform_deps)

# tests and benchmarks depend on GLib, so we only build them if GObject is enabled
if build_gobject
  subdir('tests')
  subdir('bench')
endif
