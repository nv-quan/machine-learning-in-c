# Prelude
# Require minimum autoconf version of 2.72
AC_PREREQ([2.72])

# Autoconf init
AC_INIT([mlalgos], [0.1], [quannguyenvan1701@gmail.com])

# All auxiliary files will be put into build-aux directory
AC_CONFIG_AUX_DIR([build-aux])

# For LT_INIT
AC_CONFIG_MACRO_DIR([m4])

AM_PROG_AR

# Libtool init (to build cross-platform libs)
LT_INIT


# Automake init
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Verify that src/main.c exists -> check correct directory
AC_CONFIG_SRCDIR([src/main.c])

# Use libraries
# AC_PROG_RANLIB

# Compiler check
AC_PROG_CC

PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])

# Check if libcsv exists
# lcsv will be added automatically
AC_CHECK_LIB([csv], [csv_init])

# Create config.h from config.h.in
AC_CONFIG_HEADERS([config.h])

# Automake creates FILE.in for each FILE that has FILE.am
# Autoconf creates FILE from all FILE.in
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])

AC_OUTPUT
