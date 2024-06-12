TESTS = test_prg
check_PROGRAMS = test_prg
test_prg_SOURCES = test.c
test_prg_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../src
test_prg_LDADD = @CHECK_LIBS@ $(top_builddir)/src/libio.la $(top_builddir)/src/libdata.la $(top_builddir)/src/libmath.la $(top_builddir)/src/libutils.la
