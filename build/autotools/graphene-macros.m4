AC_DEFUN([graphene_COMPILER_TYPE], [
AC_LANG_CASE(
	[C], [
		AC_REQUIRE([AC_PROG_CC_C99])
	],
	[C++], [
		AC_REQUIRE([AC_PROG_CXX])
	]
)
AC_CHECK_DECL([__clang__], [CLANGCC="yes"], [CLANGCC="no"])
AC_CHECK_DECL([__INTEL_COMPILER], [INTELCC="yes"], [INTELCC="no"])
AC_CHECK_DECL([__SUNPRO_C], [SUNCC="yes"], [SUNCC="no"])
]) # graphene_COMPILER_TYPE


AC_DEFUN([graphene_TESTSET_CFLAG], [
m4_if([$#], 0, [m4_fatal([graphene_TESTSET_CFLAG was given with an unsupported number of arguments])])
m4_if([$#], 1, [m4_fatal([graphene_TESTSET_CFLAG was given with an unsupported number of arguments])])

AC_LANG_COMPILER_REQUIRE

AC_LANG_CASE(
	[C], [
		AC_REQUIRE([AC_PROG_CC_C99])
		define([PREFIX], [C])
		define([CACHE_PREFIX], [cc])
		define([COMPILER], [$CC])
	],
	[C++], [
		define([PREFIX], [CXX])
		define([CACHE_PREFIX], [cxx])
		define([COMPILER], [$CXX])
	]
)

[graphene_testset_save_]PREFIX[FLAGS]="$PREFIX[FLAGS]"

if test "x$[graphene_testset_]CACHE_PREFIX[_unknown_warning_option]" = x ; then
	PREFIX[FLAGS]="$PREFIX[FLAGS] -Werror=unknown-warning-option"
	AC_CACHE_CHECK([if ]COMPILER[ supports -Werror=unknown-warning-option],
			[graphene_cv_]CACHE_PREFIX[_flag_unknown_warning_option],
			AC_COMPILE_IFELSE([AC_LANG_SOURCE([int i;])],
					  [graphene_cv_]CACHE_PREFIX[_flag_unknown_warning_option=yes],
					  [graphene_cv_]CACHE_PREFIX[_flag_unknown_warning_option=no]))
	[graphene_testset_]CACHE_PREFIX[_unknown_warning_option]=$[graphene_cv_]CACHE_PREFIX[_flag_unknown_warning_option]
	PREFIX[FLAGS]="$[graphene_testset_save_]PREFIX[FLAGS]"
fi

if test "x$[graphene_testset_]CACHE_PREFIX[_unused_command_line_argument]" = x ; then
	if test "x$[graphene_testset_]CACHE_PREFIX[_unknown_warning_option]" = xyes ; then
		PREFIX[FLAGS]="$PREFIX[FLAGS] -Werror=unknown-warning-option"
	fi
	PREFIX[FLAGS]="$PREFIX[FLAGS] -Werror=unused-command-line-argument"
	AC_CACHE_CHECK([if ]COMPILER[ supports -Werror=unused-command-line-argument],
			[graphene_cv_]CACHE_PREFIX[_flag_unused_command_line_argument],
			AC_COMPILE_IFELSE([AC_LANG_SOURCE([int i;])],
					  [graphene_cv_]CACHE_PREFIX[_flag_unused_command_line_argument=yes],
					  [graphene_cv_]CACHE_PREFIX[_flag_unused_command_line_argument=no]))
	[graphene_testset_]CACHE_PREFIX[_unused_command_line_argument]=$[graphene_cv_]CACHE_PREFIX[_flag_unused_command_line_argument]
	PREFIX[FLAGS]="$[graphene_testset_save_]PREFIX[FLAGS]"
fi

found="no"
m4_foreach([flag], m4_cdr($@), [
	if test $found = no ; then
		if test "x$graphene_testset_]CACHE_PREFIX[_unknown_warning_option" = xyes ; then
			PREFIX[FLAGS]="$PREFIX[FLAGS] -Werror=unknown-warning-option"
		fi

		if test "x$graphene_testset_]CACHE_PREFIX[_unused_command_line_argument" = xyes ; then
			PREFIX[FLAGS]="$PREFIX[FLAGS] -Werror=unused-command-line-argument"
		fi

		PREFIX[FLAGS]="$PREFIX[FLAGS] ]flag["

dnl Some hackery here since AC_CACHE_VAL can't handle a non-literal varname
		AC_MSG_CHECKING([if ]COMPILER[ supports ]flag[])
		cacheid=AS_TR_SH([graphene_cv_]CACHE_PREFIX[_flag_]flag[])
		AC_CACHE_VAL($cacheid,
			     [AC_LINK_IFELSE([AC_LANG_PROGRAM([int i;])],
					     [eval $cacheid=yes],
					     [eval $cacheid=no])])

		PREFIX[FLAGS]="$[graphene_testset_save_]PREFIX[FLAGS]"

		eval supported=\$$cacheid
		AC_MSG_RESULT([$supported])
		if test "$supported" = yes ; then
			$1="$$1 ]flag["
			found="yes"
		fi
	fi
])
]) # graphene_TESTSET_CFLAG

AC_DEFUN([graphene_DEFAULT_COMPILER_FLAGS], [
AC_REQUIRE([graphene_COMPILER_TYPE])

AC_LANG_CASE(
        [C], [
                define([PREFIX], [C])
        ],
        [C++], [
                define([PREFIX], [CXX])
        ]
)
# -v is too short to test reliably with graphene_TESTSET_CFLAG
if test "x$SUNCC" = "xyes"; then
    [BASE_]PREFIX[FLAGS]="-v"
else
    [BASE_]PREFIX[FLAGS]=""
fi

graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-O3])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-ffast-math])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-fstrict-aliasing])

graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wall])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wpointer-arith])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wmissing-declarations])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wformat=2], [-Wformat])

AC_LANG_CASE(
	[C], [
		graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wstrict-prototypes])
		graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wmissing-prototypes])
		graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wnested-externs])
		graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wold-style-definition], [-fd])
		graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wdeclaration-after-statement])
	]
)

# This chunk adds additional warnings that could catch undesired effects.
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wunused])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wuninitialized])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wshadow])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wmissing-noreturn])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wmissing-format-attribute])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wredundant-decls])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wlogical-op])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Wcast-align])

# Turn some warnings into errors, so we don't accidently get successful builds
# when there are problems that should be fixed.
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=implicit], [-errwarn=E_NO_EXPLICIT_TYPE_GIVEN -errwarn=E_NO_IMPLICIT_DECL_ALLOWED])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=nonnull])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=init-self])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=main])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=missing-braces])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=sequence-point])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=return-type], [-errwarn=E_FUNC_HAS_NO_RETURN_STMT])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=trigraphs])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=array-bounds])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=write-strings])
graphene_TESTSET_CFLAG([[BASE_]PREFIX[FLAGS]], [-Werror=address])

AC_SUBST([BASE_]PREFIX[FLAGS])
]) # graphene_DEFAULT_COMPILER_FLAGS
