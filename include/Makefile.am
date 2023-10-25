include_HEADERS = \
	libfsfat.h

pkginclude_HEADERS = \
	libfsfat/codepage.h \
	libfsfat/definitions.h \
	libfsfat/error.h \
	libfsfat/extern.h \
	libfsfat/features.h \
	libfsfat/types.h

EXTRA_DIST = \
	libfsfat.h.in \
	libfsfat/definitions.h.in \
	libfsfat/features.h.in \
	libfsfat/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libfsfat.h
	-rm -f libfsfat/definitions.h
	-rm -f libfsfat/features.h
	-rm -f libfsfat/types.h
	-rm -f Makefile

