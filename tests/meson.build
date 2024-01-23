unit_tests = [
  'box',
  'box2d',
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

gen_installed_test = find_program('gen-installed-test.py')

installed_test_datadir = get_option('prefix') / get_option('datadir') / 'installed-tests' / graphene_api_path
installed_test_bindir = get_option('prefix') / get_option('libexecdir') / 'installed-tests' / graphene_api_path

# Make tests conditional on having mutest-1 installed system-wide, or
# available as a subproject
mutest_dep = dependency('mutest-1',
  fallback: ['mutest', 'mutest_dep'],
  default_options: ['static=true'],
  required: false,
  disabler: true,
)

if mutest_dep.found()
  foreach unit: unit_tests
    wrapper = '@0@.test'.format(unit)
    custom_target(wrapper,
      output: wrapper,
      command: [
        gen_installed_test,
        '--testdir=@0@'.format(installed_test_bindir),
        '--testname=@0@'.format(unit),
        '--outdir=@OUTDIR@',
        '--outfile=@0@'.format(wrapper),
      ],
      install: get_option('installed_tests'),
      install_dir: installed_test_datadir,
    )

    test(unit,
      executable(unit, unit + '.c',
        dependencies: [graphene_dep, mutest_dep],
        include_directories: graphene_inc,
        c_args: common_cflags,
        install: get_option('installed_tests'),
        install_dir: installed_test_bindir,
      ),
      env: ['MUTEST_OUTPUT=tap'],
      protocol: 'tap',
    )
  endforeach
endif

src_build_path = meson.current_build_dir() / '../src'

if build_gir and host_system == 'linux' and not meson.is_cross_build()
  foreach unit: ['introspection.py']
    if get_option('installed_tests')
      install_data(unit, install_dir: installed_test_bindir)
    endif

    wrapper = '@0@.test'.format(unit)
    custom_target(wrapper,
      output: wrapper,
      command: [
        gen_installed_test,
        '--testdir=@0@'.format(installed_test_bindir),
        '--testname=@0@'.format(unit),
        '--outdir=@OUTDIR@',
        '--outfile=@0@'.format(wrapper),
      ],
      install: get_option('installed_tests'),
      install_dir: installed_test_datadir,
    )

    test(unit,
      python,
      args: [files(unit)],
      env: [
        'GI_TYPELIB_PATH=@0@'.format(src_build_path),
        'LD_LIBRARY_PATH=@0@'.format(src_build_path),
      ],
      protocol: 'tap',
    )
  endforeach
endif
