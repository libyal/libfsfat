# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir3)"
pyexecdir = $(pyexecdir3)

if HAVE_PYTHON3
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
	@LIBFGUID_CPPFLAGS@ \
	@LIBFSFAT_DLL_IMPORT@

am_pyfsfat_la_rpath = -rpath $(pyexecdir3)

BUILT_SOURCES = \
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

pyexec_LTLIBRARIES = pyfsfat.la

nodist_pyfsfat_la_SOURCES = $(BUILT_SOURCES)

pyfsfat_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyfsfat_la_CPPFLAGS = $(PYTHON3_CPPFLAGS)
pyfsfat_la_LDFLAGS  = -module -avoid-version $(PYTHON3_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyfsfat/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyfsfat*.[ch]

distclean: clean
	/bin/rm -f Makefile

