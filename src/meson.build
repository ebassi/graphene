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
configure_file(
  output: 'config.h',
  configuration: conf,
)

# External configuration header
configure_file(
  input: 'graphene-config.h.meson',
  output: 'graphene-config.h',
  configuration: graphene_conf,
  install: true,
  install_dir: join_paths(graphene_libdir, graphene_api_path, 'include'),
)

# Version header
version_conf = configuration_data()
version_conf.set('GRAPHENE_VERSION', graphene_version)
version_conf.set('GRAPHENE_MAJOR_VERSION', graphene_major_version)
version_conf.set('GRAPHENE_MINOR_VERSION', graphene_minor_version)
version_conf.set('GRAPHENE_MICRO_VERSION', graphene_micro_version)

configure_file(
  input: 'graphene-version.h.in',
  output: 'graphene-version.h',
  configuration: version_conf,
  install: true,
  install_dir: join_paths(graphene_includedir, graphene_api_path),
)

install_headers(headers + simd_headers + [ 'graphene.h' ], subdir: graphene_api_path)

pkgconfig_files = [ graphene_api_path ]
platform_deps = []

if build_gobject
  pkgconfig_files += [ graphene_gobject_api_path ]
  platform_deps += [ gobject ]
endif

libgraphene = library(
  graphene_api_path,
  sources: sources + simd_sources + private_headers,
  version: libversion,
  soversion: soversion,
  install: true,
  dependencies: [ mathlib, threadlib ] + platform_deps,
  c_args: extra_args + common_cflags + debug_flags + [
    '-DGRAPHENE_COMPILATION',
  ],
  link_args: common_ldflags,
)

# Generate the pkg-config files
pkg_conf = configuration_data()
pkg_conf.set('GRAPHENE_VERSION', graphene_version)
pkg_conf.set('prefix', graphene_prefix)
pkg_conf.set('exec_prefix', graphene_prefix)
pkg_conf.set('libdir', graphene_libdir)
pkg_conf.set('includedir', graphene_includedir)
pkg_conf.set('SSE2_CFLAGS', sse2_cflags)
pkg_conf.set('NEON_CFLAGS', neon_cflags)
pkg_conf.set('GRAPHENE_SIMD', ' '.join(graphene_simd))

foreach simd: [ 'sse2', 'gcc', 'neon' ]
  pkg_conf.set('has_' + simd, graphene_simd.contains(simd) ? '1' : '0')
endforeach

if build_gobject
  pkg_conf.set('GRAPHENE_REQS', 'gobject-2.0')
else
  pkg_conf.set('GRAPHENE_REQS', '')
endif

foreach p: pkgconfig_files
  configure_file(
    input: 'graphene.pc.in',
    output: p + '.pc',
    configuration: pkg_conf,
    install: true,
    install_dir: join_paths(graphene_libdir, 'pkgconfig'),
  )
endforeach

graphene_dep_sources = []

# Introspection
if build_gir
  python = python3.find_python()
  identfilter_py = join_paths(meson.current_source_dir(), 'identfilter.py')

  gir_extra_args = [
    '--identifier-filter-cmd=@0@ @1@'.format(python.path(), identfilter_py),
    '--accept-unprefixed',
    '--quiet',
    '--warn-all',
    '-DGRAPHENE_COMPILATION',
  ]
  graphene_gir = gnome.generate_gir(libgraphene,
                                    sources: headers + sources,
                                    namespace: 'Graphene',
                                    nsversion: graphene_api_version,
                                    identifier_prefix: 'Graphene',
                                    symbol_prefix: 'graphene',
                                    export_packages: graphene_gobject_api_path,
                                    includes: [ 'GObject-2.0' ],
                                    header: 'graphene-object.h',
                                    install: true,
                                    extra_args: gir_extra_args)
  graphene_dep_sources += graphene_gir
endif

# Dependency for tests and benchmarks and subproject usage
graphene_inc = include_directories('.')
graphene_dep = declare_dependency(
  sources: graphene_dep_sources,
  link_with: libgraphene,
  include_directories: [ graphene_inc ],
  dependencies: [ mathlib, threadlib ] + platform_deps,
)

# tests and benchmarks depend on GLib, so we only build them if GObject is enabled
if build_gobject
  if get_option('tests')
    subdir('tests')
  endif
  if get_option('benchmarks')
    subdir('bench')
  endif
endif
