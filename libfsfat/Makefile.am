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
	@PTHREAD_CPPFLAGS@ \
	@LIBFSFAT_DLL_EXPORT@

lib_LTLIBRARIES = libfsfat.la

libfsfat_la_SOURCES = \
	fsfat_boot_record.h \
	fsfat_directory_entry.h \
	libfsfat.c \
	libfsfat_allocation_table.c libfsfat_allocation_table.h \
	libfsfat_block_descriptor.c libfsfat_block_descriptor.h \
	libfsfat_block_tree.c libfsfat_block_tree.h \
	libfsfat_block_tree_node.c libfsfat_block_tree_node.h \
	libfsfat_boot_record.c libfsfat_boot_record.h \
	libfsfat_cluster_block_data.c libfsfat_cluster_block_data.h \
	libfsfat_date_time.c libfsfat_date_time.h \
	libfsfat_debug.c libfsfat_debug.h \
	libfsfat_definitions.h \
	libfsfat_directory.c libfsfat_directory.h \
	libfsfat_directory_entry.c libfsfat_directory_entry.h \
	libfsfat_error.c libfsfat_error.h \
	libfsfat_extent.c libfsfat_extent.h \
	libfsfat_extern.h \
	libfsfat_file_entry.c libfsfat_file_entry.h \
	libfsfat_file_system.c libfsfat_file_system.h \
	libfsfat_io_handle.c libfsfat_io_handle.h \
	libfsfat_libbfio.h \
	libfsfat_libcdata.h \
	libfsfat_libcerror.h \
	libfsfat_libclocale.h \
	libfsfat_libcnotify.h \
	libfsfat_libcthreads.h \
	libfsfat_libfcache.h \
	libfsfat_libfdata.h \
	libfsfat_libfdatetime.h \
	libfsfat_libfguid.h \
	libfsfat_libuna.h \
	libfsfat_name.c libfsfat_name.h \
	libfsfat_notify.c libfsfat_notify.h \
	libfsfat_support.c libfsfat_support.h \
	libfsfat_types.h \
	libfsfat_unused.h \
	libfsfat_volume.c libfsfat_volume.h

libfsfat_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@PTHREAD_LIBADD@

libfsfat_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfsfat_definitions.h.in \
	libfsfat.rc \
	libfsfat.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsfat_definitions.h
	/bin/rm -f libfsfat.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libfsfat ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfsfat_la_SOURCES)

