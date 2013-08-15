#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([x11-ts-cal], [0.0], [https://github.com/fourdollars/x11-ts-cal/issues], [x11-ts-cal], [https://fourdollars.github.io/x11-ts-cal/])
AC_CONFIG_SRCDIR([x11-ts-cal.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

LT_PREREQ([2.4.2])
LT_INIT

AM_MAINTAINER_MODE([enable])
AM_INIT_AUTOMAKE([dist-xz no-dist-gzip silent-rules foreign])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.
PKG_CHECK_MODULES(X11, [x11 xi inputproto])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT