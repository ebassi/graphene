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
configure_file(output: 'config.h',
               configuration: conf)

# External configuration header
configure_file(input: 'graphene-config.h.meson',
               output: 'graphene-config.h',
               configuration: graphene_conf,
               install: true,
               install_dir: join_paths(graphene_libdir, graphene_api_path, 'include'))

# Version header
version_conf = configuration_data()
version_conf.set('GRAPHENE_MAJOR_VERSION', graphene_major_version)
version_conf.set('GRAPHENE_MINOR_VERSION', graphene_minor_version)
version_conf.set('GRAPHENE_MICRO_VERSION', graphene_micro_version)
version_conf.set('GRAPHENE_VERSION', graphene_version)

configure_file(input: 'graphene-version.h.in',
               output: 'graphene-version.h',
               configuration: version_conf,
               install: true,
               install_dir: join_paths(graphene_includedir, graphene_api_path))

install_headers(headers + simd_headers + [ 'graphene.h' ], subdir: graphene_api_path)

pkgconfig_files = [ graphene_api_path ]
platform_deps = []

if build_gobject
  pkgconfig_files += [ graphene_gobject_api_path ]
  platform_deps += [ gobject ]
endif

libgraphene = library(graphene_api_path,
                      sources: sources + simd_sources + private_headers,
                      version: libversion,
                      soversion: soversion,
                      install: true,
                      dependencies: [ mathlib, threadlib ] + platform_deps,
                      c_args: extra_args + common_cflags + debug_flags + [
                        '-DGRAPHENE_COMPILATION',
                      ],
                      link_args: common_ldflags)

# Internal dependency, for tests and benchmarks
graphene_inc = include_directories('.')
graphene_dep = declare_dependency(link_with: libgraphene,
                                  include_directories: [ graphene_inc ],
                                  dependencies: [ mathlib, threadlib ] + platform_deps)


# Feature discovery variables
graphene_simd_vars = []
foreach simd: [ 'sse2', 'gcc', 'neon', 'scalar', ]
  has_simd = graphene_simd.contains(simd) ? '1' : '0'
  graphene_simd_vars += [ 'graphene_has_@0@=@1@'.format(simd, has_simd) ]
endforeach

# Backward compatibility variables
graphene_simd_vars += [ 'graphene_simd=@0@'.format(' '.join(graphene_simd)) ]
if graphene_simd.contains('sse2') and sse2_cflags != ''
  graphene_simd_vars += [ 'sse2_cflags=@0@'.format(sse2_cflags) ]
endif
if graphene_simd.contains('neon') and neon_cflags != ''
  graphene_simd_vars += [ 'neon_cflags=@0@'.format(neon_cflags) ]
endif

# Generate the pkg-config files
pkg = import('pkgconfig')
pkg.generate(libraries: libgraphene,
             name: 'Graphene',
             description: 'Math classes for graphic libraries',
             version: meson.project_version(),
             filebase: graphene_api_path,
             subdirs: graphene_api_path,
             extra_cflags: sse2_cflags + neon_cflags,
             variables: graphene_simd_vars + [
               'exec_prefix=${prefix}',
             ])

if build_gobject
  pkg.generate(libraries: libgraphene,
               name: 'Graphene',
               description: 'Math classes for graphic libraries (with GObject types)',
               version: meson.project_version(),
               filebase: graphene_gobject_api_path,
               subdirs: graphene_api_path,
               requires: 'gobject-2.0',
               extra_cflags: sse2_cflags + neon_cflags,
               variables: graphene_simd_vars + [
                 'exec_prefix=${prefix}',
               ])
endif

if build_gir
  python = python3.find_python()
  identfilter_py = join_paths(meson.current_source_dir(), 'identfilter.py')

  gir_extra_args = [
    '--identifier-filter-cmd=@0@ @1@'.format(python.path(), identfilter_py),
    '--c-include=graphene-gobject.h',
    '--accept-unprefixed',
    '--quiet',
    '--warn-all',
    '-DGRAPHENE_COMPILATION',
  ]
  gnome.generate_gir(libgraphene,
                     sources: headers + sources,
                     namespace: 'Graphene',
                     nsversion: graphene_api_version,
                     identifier_prefix: 'Graphene',
                     symbol_prefix: 'graphene',
                     export_packages: graphene_gobject_api_path,
                     includes: [ 'GObject-2.0' ],
                     install: true,
                     extra_args: gir_extra_args)
endif

# tests and benchmarks depend on GLib, so we only build them if GObject is enabled
if build_gobject
  subdir('tests')
  subdir('bench')
endif
