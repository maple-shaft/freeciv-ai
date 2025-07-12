#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([FREECIVAI], [0.0.0])
AC_CONFIG_SRCDIR([maple/ai_stub.c])
AC_CONFIG_HEADERS([maple/ai_stub.h])

# Checks for programs.
AC_PROG_CC
LT_INIT([shared])
AM_INIT_AUTOMAKE

# Checks for libraries.

PKG_CHECK_MODULES([CURL], [libcurl])
PKG_CHECK_MODULES([JANSSON], [jansson])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_ARG_WITH([freecivbld],
  AS_HELP_STRING([--with-freecivbdl], [path to freeciv build tree]),
            [FCBLDPATH=$withval],
            [FCBLDPATH="/home/dustin/git3/freeciv/build"])
AC_ARG_WITH([freecivsrc],
  AS_HELP_STRING([--with-freecivsrc], [patch to freeciv source tree]),
            [FCSRCPATH=$withval],
            [FCSRCPATH="/home/dustin/git3/freeciv"])

AC_SUBST([FCBLDPATH])
AC_SUBST([FCSRCPATH])

AC_ARG_WITH([aimoddir],
  AS_HELP_STRING([--with-aimoddir], [freeciv AI module directory]),
            [aimoduledir="$withval"],
            [aimoduledir="${libdir}/fcai"])

AC_SUBST([aimoduledir])

AC_CONFIG_FILES([
        Makefile
        maple/Makefile])
AC_OUTPUT