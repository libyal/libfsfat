if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFSFAT_DLL_IMPORT@

pyexec_LTLIBRARIES = pyfsfat.la

pyfsfat_la_SOURCES = \
	pyfsfat.c pyfsfat.h \
	pyfsfat_datetime.c pyfsfat_datetime.h \
	pyfsfat_error.c pyfsfat_error.h \
	pyfsfat_file_attribute_flags.c pyfsfat_file_attribute_flags.h \
	pyfsfat_file_entries.c pyfsfat_file_entries.h \
	pyfsfat_file_entry.c pyfsfat_file_entry.h \
	pyfsfat_file_object_io_handle.c pyfsfat_file_object_io_handle.h \
	pyfsfat_file_system_formats.c pyfsfat_file_system_formats.h \
	pyfsfat_integer.c pyfsfat_integer.h \
	pyfsfat_libbfio.h \
	pyfsfat_libcerror.h \
	pyfsfat_libfsfat.h \
	pyfsfat_python.h \
	pyfsfat_unused.h \
	pyfsfat_volume.c pyfsfat_volume.h

pyfsfat_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyfsfat_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyfsfat_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f setup.py
	/bin/rm -f Makefile

