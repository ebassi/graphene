graphene_bench_sources = [
  'graphene-bench-utils.h',
  'graphene-bench-utils.c'
]

graphene_bench = static_library('graphene-bench', graphene_bench_sources,
                                c_args: [
                                  '-DG_LOG_DOMAIN="Graphene-Bench"'
                                ],
                                include_directories: [ graphene_inc ],
                                install: false,
                                dependencies: [ mathlib, graphene_dep ])
graphene_bench_dep = declare_dependency(link_with: [ graphene_bench ])

# XXX: Would be nicer if we could do: have_FOO = 'FOO' in ARRAY
have_sse = graphene_simd.contains('sse2')
have_gcc = graphene_simd.contains('gcc')
have_neon = graphene_simd.contains('neon')
have_scalar = true      # scalar support is always compiled in

all_simds = [
  [ 'SSE', 'sse', have_sse ],
  [ 'GCC', 'gcc', have_gcc ],
  [ 'ARM_NEON', 'neon', have_neon ],
  [ 'SCALAR', 'scalar', have_scalar ]
]

bench_units = [
  'matrix'
]

foreach simd: all_simds
  run_test = simd[2]
  if run_test
    foreach unit: bench_units
      exe = executable('@0@-@1@'.format(unit, simd[1]), unit + '.c',
                       c_args: [
                         '-DGRAPHENE_COMPILATION',
                         '-DGRAPHENE_SIMD_BENCHMARK',
                         '-DGRAPHENE_HAS_' + simd[0],
                       ],
                       install: false,
                       dependencies: [ graphene_bench_dep, graphene_dep ])
      benchmark('@0@-@1@'.format(unit, simd[1]), exe)
    endforeach
  endif
endforeach
