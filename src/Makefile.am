noinst_LTLIBRARIES = libio.la libdata.la libmath.la libutils.la
noinst_PROGRAMS = mlalgos

# Only list .h files so that they get distributed
libio_la_SOURCES = io.c io.h
libdata_la_SOURCES = data.c data.h
libmath_la_SOURCES = custom_math.c custom_math.h
libutils_la_SOURCES = utils.c utils.h

mlalgos_SOURCES = main.c gradient_descent.c gradient_descent.h

# Additional compiler flags
# Can add mlalgos_CFLAGS to be more specific
mlalgos_LDADD = libio.la libdata.la libmath.la libutils.la
AM_CFLAGS = -Wall -Wextra -std=c89

# Check if AM_LDFLAGS = -lcsv is needed
# mlalgos_LDADD = -lcsv
