unit_tests = [
  'box',
  'euler',
  'frustum',
  'matrix',
  'plane',
  'point',
  'point3d',
  'quad',
  'quaternion',
  'ray',
  'rect',
  'simd',
  'size',
  'sphere',
  'triangle',
  'vec2',
  'vec3',
  'vec4'
]

foreach unit: unit_tests
  exe = executable(unit, unit + '.c',
                   install: true,
                   install_dir: 'share/installed-tests/graphene-1.0',
                   dependencies: graphene_dep,
                   include_directories: graphene_inc,
                   c_args: [
                     '-DG_LOG_DOMAIN="Graphene-Test"',
                     '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_30',
                     '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_32'
                   ])
  test(unit, exe, args: [ '--tap', '-k' ])
endforeach
