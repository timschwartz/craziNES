AC_PREREQ([2.71])
AC_INIT([craziNES],[0.2],[tim@metaverse.systems])
AC_CONFIG_SRCDIR([src/nes/cpu_6502.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX
AM_INIT_AUTOMAKE([subdir-objects])

AM_OPTIONS_WXCONFIG
reqwx=3.2.0
AM_PATH_WXCONFIG($reqwx, wxWin=1,,[all])
if test "$wxWin" != 1; then
        AC_MSG_ERROR([
                wxWidgets must be installed on your system.

                Please check that wx-config is in path, the directory
                where wxWidgets libraries are installed (returned by
                'wx-config --libs' or 'wx-config --static --libs' command)
                is in LD_LIBRARY_PATH or equivalent variable and
                wxWidgets version is $reqwx or above.
                ])
fi

PKG_CHECK_MODULES([GLM], glm)

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

AM_CONDITIONAL([HAVE_DOXYGEN],
[test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([Doxyfile])])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
