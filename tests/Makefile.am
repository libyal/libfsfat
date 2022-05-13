AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSEXT_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSEXT = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_fsextinfo.sh \
	$(TESTS_PYFSEXT)

check_SCRIPTS = \
	pyfsext_test_support.py \
	test_fsextinfo.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsfat_test_boot_record \
	fsext_test_error \
	fsext_test_file_entry \
	fsext_test_io_handle \
	fsext_test_notify \
	fsext_test_support \
	fsext_test_tools_info_handle \
	fsext_test_tools_output \
	fsext_test_tools_signal \
	fsext_test_volume

fsfat_test_boot_record_SOURCES = \
	fsfat_test_boot_record.c \
	fsfat_test_functions.c fsfat_test_functions.h \
	fsfat_test_libbfio.h \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_boot_record_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsext_test_error_SOURCES = \
	fsext_test_error.c \
	fsext_test_libfsext.h \
	fsext_test_macros.h \
	fsext_test_unused.h

fsext_test_error_LDADD = \
	../libfsext/libfsext.la

fsext_test_file_entry_SOURCES = \
	fsext_test_file_entry.c \
	fsext_test_libcerror.h \
	fsext_test_libfsext.h \
	fsext_test_macros.h \
	fsext_test_memory.c fsext_test_memory.h \
	fsext_test_unused.h

fsext_test_file_entry_LDADD = \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@

fsext_test_io_handle_SOURCES = \
	fsext_test_io_handle.c \
	fsext_test_libcerror.h \
	fsext_test_libfsext.h \
	fsext_test_macros.h \
	fsext_test_memory.c fsext_test_memory.h \
	fsext_test_unused.h

fsext_test_io_handle_LDADD = \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@

fsext_test_notify_SOURCES = \
	fsext_test_libcerror.h \
	fsext_test_libfsext.h \
	fsext_test_macros.h \
	fsext_test_notify.c \
	fsext_test_unused.h

fsext_test_notify_LDADD = \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@

fsext_test_support_SOURCES = \
	fsext_test_functions.c fsext_test_functions.h \
	fsext_test_getopt.c fsext_test_getopt.h \
	fsext_test_libbfio.h \
	fsext_test_libcerror.h \
	fsext_test_libclocale.h \
	fsext_test_libcnotify.h \
	fsext_test_libfsext.h \
	fsext_test_libuna.h \
	fsext_test_macros.h \
	fsext_test_memory.c fsext_test_memory.h \
	fsext_test_support.c \
	fsext_test_unused.h

fsext_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@

fsext_test_tools_info_handle_SOURCES = \
	../fsexttools/digest_hash.c ../fsexttools/digest_hash.h \
	../fsexttools/info_handle.c ../fsexttools/info_handle.h \
	fsext_test_libcerror.h \
	fsext_test_macros.h \
	fsext_test_memory.c fsext_test_memory.h \
	fsext_test_tools_info_handle.c \
	fsext_test_unused.h

fsext_test_tools_info_handle_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@

fsext_test_tools_output_SOURCES = \
	../fsexttools/fsexttools_output.c ../fsexttools/fsexttools_output.h \
	fsext_test_libcerror.h \
	fsext_test_macros.h \
	fsext_test_tools_output.c \
	fsext_test_unused.h

fsext_test_tools_output_LDADD = \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@

fsext_test_tools_signal_SOURCES = \
	../fsexttools/fsexttools_signal.c ../fsexttools/fsexttools_signal.h \
	fsext_test_libcerror.h \
	fsext_test_macros.h \
	fsext_test_tools_signal.c \
	fsext_test_unused.h

fsext_test_tools_signal_LDADD = \
	../libfsext/libfsext.la \
	@LIBCERROR_LIBADD@

fsext_test_volume_SOURCES = \
	fsext_test_functions.c fsext_test_functions.h \
	fsext_test_getopt.c fsext_test_getopt.h \
	fsext_test_libbfio.h \
	fsext_test_libcerror.h \
	fsext_test_libclocale.h \
	fsext_test_libcnotify.h \
	fsext_test_libfsext.h \
	fsext_test_libuna.h \
	fsext_test_macros.h \
	fsext_test_memory.c fsext_test_memory.h \
	fsext_test_volume.c \
	fsext_test_unused.h

fsext_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsext/libfsext.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

