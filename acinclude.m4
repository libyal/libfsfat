dnl Checks for required headers and functions
dnl
dnl Version: 20260604

dnl Function to detect if libfsfat dependencies are available
AC_DEFUN([AX_LIBFSFAT_CHECK_LOCAL],
  [dnl Headers included in libfsfat/libfsfat_name.c
  AC_CHECK_HEADERS([wctype.h])

  dnl Functions used in libfsfat/libfsfat_name.c
  AC_CHECK_FUNCS([towupper])

  AS_IF(
    [test "x$ac_cv_func_towupper" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: towupper],
      [1])
  ])

  dnl Check for internationalization functions in libfsfat/libfsfat_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])
])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBFSFAT_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBFSFAT_DLL_EXPORT],
        ["-DLIBFSFAT_DLL_EXPORT"])

      AC_SUBST(
        [LIBFSFAT_DLL_IMPORT],
        ["-DLIBFSFAT_DLL_IMPORT"])
      ])
    ])
  ])

