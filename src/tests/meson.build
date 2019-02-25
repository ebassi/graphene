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

python = python3.find_python()
gen_installed_test = join_paths(meson.current_source_dir(), 'gen-installed-test.py')

installed_test_datadir = join_paths(get_option('prefix'), get_option('datadir'), 'installed-tests', graphene_api_path)
installed_test_bindir = join_paths(get_option('prefix'), get_option('libexecdir'), 'installed-tests', graphene_api_path)

# Make tests conditional on having mutest-1 installed system-wide, or
# available as a subproject
mutest_dep = dependency('mutest-1', required: false)
if not mutest_dep.found()
  mutest = subproject('mutest',
    default_options: [ 'static=true', ],
    required: false,
  )

  if mutest.found()
    mutest_dep = mutest.get_variable('mutest_dep')
  else
    mutest_dep = disabler()
  endif
endif

if mutest_dep.found()
  foreach unit: unit_tests
    wrapper = '@0@.test'.format(unit)
    custom_target(wrapper,
      output: wrapper,
      command: [
        python,
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
