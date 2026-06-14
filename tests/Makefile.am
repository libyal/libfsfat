AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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
	@LIBFSFAT_DLL_IMPORT@

check_PYTHONdir = $(abs_top_builddir)

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_PYTHON_SCRIPTS = \
	pyfsfat_test_support.py \
	pyfsfat_test_volume.py

check_PROGRAMS = \
	fsfat_test_allocation_table \
	fsfat_test_block_descriptor \
	fsfat_test_block_tree \
	fsfat_test_block_tree_node \
	fsfat_test_boot_record \
	fsfat_test_directory \
	fsfat_test_directory_entry \
	fsfat_test_error \
	fsfat_test_extent \
	fsfat_test_file_entry \
	fsfat_test_io_handle \
	fsfat_test_name \
	fsfat_test_notify \
	fsfat_test_support \
	fsfat_test_tools_info_handle \
	fsfat_test_tools_mount_path_string \
	fsfat_test_tools_output \
	fsfat_test_tools_signal \
	fsfat_test_volume

fsfat_test_allocation_table_SOURCES = \
	fsfat_test_allocation_table.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_allocation_table_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_block_descriptor_SOURCES = \
	fsfat_test_block_descriptor.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_block_descriptor_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_block_tree_SOURCES = \
	fsfat_test_block_tree.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_block_tree_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_block_tree_node_SOURCES = \
	fsfat_test_block_tree_node.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_block_tree_node_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

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

fsfat_test_directory_SOURCES = \
	fsfat_test_directory.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_directory_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_directory_entry_SOURCES = \
	fsfat_test_directory_entry.c \
	fsfat_test_functions.c fsfat_test_functions.h \
	fsfat_test_libbfio.h \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_directory_entry_LDADD = \
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

fsfat_test_error_SOURCES = \
	fsfat_test_error.c \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_unused.h

fsfat_test_error_LDADD = \
	../libfsfat/libfsfat.la

fsfat_test_extent_SOURCES = \
	fsfat_test_extent.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_extent_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_file_entry_SOURCES = \
	fsfat_test_file_entry.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_file_entry_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_io_handle_SOURCES = \
	fsfat_test_io_handle.c \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_unused.h

fsfat_test_io_handle_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_name_SOURCES = \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_libuna.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_name.c \
	fsfat_test_unused.h

fsfat_test_name_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_notify_SOURCES = \
	fsfat_test_libcerror.h \
	fsfat_test_libfsfat.h \
	fsfat_test_macros.h \
	fsfat_test_notify.c \
	fsfat_test_unused.h

fsfat_test_notify_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_support_SOURCES = \
	fsfat_test_functions.c fsfat_test_functions.h \
	fsfat_test_getopt.c fsfat_test_getopt.h \
	fsfat_test_libbfio.h \
	fsfat_test_libcerror.h \
	fsfat_test_libclocale.h \
	fsfat_test_libcnotify.h \
	fsfat_test_libfsfat.h \
	fsfat_test_libuna.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_support.c \
	fsfat_test_unused.h

fsfat_test_support_LDADD = \
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

fsfat_test_tools_info_handle_SOURCES = \
	../fsfattools/digest_hash.c ../fsfattools/digest_hash.h \
	../fsfattools/info_handle.c ../fsfattools/info_handle.h \
	fsfat_test_libcerror.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_tools_info_handle.c \
	fsfat_test_unused.h

fsfat_test_tools_info_handle_LDADD = \
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
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@

fsfat_test_tools_mount_path_string_SOURCES = \
	../fsfattools/mount_path_string.c ../fsfattools/mount_path_string.h \
	fsfat_test_libcerror.h \
	fsfat_test_libcpath.h \
	fsfat_test_macros.h \
	fsfat_test_tools_mount_path_string.c \
	fsfat_test_unused.h

fsfat_test_tools_mount_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_tools_output_SOURCES = \
	../fsfattools/fsfattools_output.c ../fsfattools/fsfattools_output.h \
	fsfat_test_libcerror.h \
	fsfat_test_macros.h \
	fsfat_test_tools_output.c \
	fsfat_test_unused.h

fsfat_test_tools_output_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

fsfat_test_tools_signal_SOURCES = \
	../fsfattools/fsfattools_signal.c ../fsfattools/fsfattools_signal.h \
	fsfat_test_libcerror.h \
	fsfat_test_macros.h \
	fsfat_test_tools_signal.c \
	fsfat_test_unused.h

fsfat_test_tools_signal_LDADD = \
	../libfsfat/libfsfat.la \
	@LIBCERROR_LIBADD@

fsfat_test_volume_SOURCES = \
	fsfat_test_functions.c fsfat_test_functions.h \
	fsfat_test_getopt.c fsfat_test_getopt.h \
	fsfat_test_libbfio.h \
	fsfat_test_libcerror.h \
	fsfat_test_libclocale.h \
	fsfat_test_libcnotify.h \
	fsfat_test_libfsfat.h \
	fsfat_test_libuna.h \
	fsfat_test_macros.h \
	fsfat_test_memory.c fsfat_test_memory.h \
	fsfat_test_volume.c \
	fsfat_test_unused.h

fsfat_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsfat/libfsfat.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libfsfat.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libfsfat

test_inputs_pyfsfat.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyfsfat

test_inputs_fsfatinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) fsfatinfo

test_inputs_fsfatinfo_fs.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) fsfatinfo_fs

test_library: package.m4 test_inputs_libfsfat.at test_library.at test_macros.at

test_manpages: package.m4 test_manpages.at

test_python_module: package.m4 test_inputs_pyfsfat.at test_python_module.at

test_tools: package.m4 test_inputs_fsfatinfo.at test_inputs_fsfatinfo_fs.at test_macros.at test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_PYTHON_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

