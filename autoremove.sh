#!/bin/zsh
make distclean
rm -rf .cache/ autom4te.cache/ build-aux/ m4/ tests/.deps/ src/.deps/
rm Makefile.in aclocal.m4 config.h.in configure src/Makefile.in tests/Makefile.in
