AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
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
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSFAT_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsfatinfo \
	fsfatmount

fsfatinfo_SOURCES = \
	digest_hash.c digest_hash.h \
	fsfatinfo.c \
	fsfattools_getopt.c fsfattools_getopt.h \
	fsfattools_i18n.h \
	fsfattools_libbfio.h \
	fsfattools_libcerror.h \
	fsfattools_libclocale.h \
	fsfattools_libcnotify.h \
	fsfattools_libfcache.h \
	fsfattools_libfdata.h \
	fsfattools_libfdatetime.h \
	fsfattools_libfguid.h \
	fsfattools_libfsfat.h \
	fsfattools_libhmac.h \
	fsfattools_libuna.h \
	fsfattools_output.c fsfattools_output.h \
	fsfattools_signal.c fsfattools_signal.h \
	fsfattools_unused.h \
	info_handle.c info_handle.h

fsfatinfo_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

fsfatmount_SOURCES = \
	fsfatmount.c \
	fsfattools_getopt.c fsfattools_getopt.h \
	fsfattools_i18n.h \
	fsfattools_libbfio.h \
	fsfattools_libcerror.h \
	fsfattools_libclocale.h \
	fsfattools_libcnotify.h \
	fsfattools_libcpath.h \
	fsfattools_libfsfat.h \
	fsfattools_libuna.h \
	fsfattools_output.c fsfattools_output.h \
	fsfattools_signal.c fsfattools_signal.h \
	fsfattools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

fsfatmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on fsfatinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsfatinfo_SOURCES)

