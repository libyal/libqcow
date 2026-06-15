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
	@LIBCAES_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBQCOW_DLL_IMPORT@

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
	pyqcow_test_file.py \
	pyqcow_test_support.py

check_PROGRAMS = \
	qcow_test_bit_stream \
	qcow_test_cluster_block \
	qcow_test_cluster_table \
	qcow_test_deflate \
	qcow_test_error \
	qcow_test_file \
	qcow_test_file_header \
	qcow_test_huffman_tree \
	qcow_test_io_handle \
	qcow_test_notify \
	qcow_test_support \
	qcow_test_tools_info_handle \
	qcow_test_tools_output \
	qcow_test_tools_signal

qcow_test_bit_stream_SOURCES = \
	qcow_test_bit_stream.c \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_unused.h

qcow_test_bit_stream_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_cluster_block_SOURCES = \
	qcow_test_cluster_block.c \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_unused.h

qcow_test_cluster_block_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_cluster_table_SOURCES = \
	qcow_test_cluster_table.c \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_unused.h

qcow_test_cluster_table_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_deflate_SOURCES = \
	qcow_test_deflate.c \
	qcow_test_libcerror.h \
	qcow_test_libcnotify.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_unused.h

qcow_test_deflate_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_error_SOURCES = \
	qcow_test_error.c \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_unused.h

qcow_test_error_LDADD = \
	../libqcow/libqcow.la

qcow_test_file_SOURCES = \
	qcow_test_extern.h \
	qcow_test_file.c \
	qcow_test_functions.c qcow_test_functions.h \
	qcow_test_getopt.c qcow_test_getopt.h \
	qcow_test_libbfio.h \
	qcow_test_libcerror.h \
	qcow_test_libclocale.h \
	qcow_test_libcnotify.h \
	qcow_test_libcpath.h \
	qcow_test_libqcow.h \
	qcow_test_libuna.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_rwlock.c qcow_test_rwlock.h \
	qcow_test_unused.h

qcow_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

qcow_test_file_header_SOURCES = \
	qcow_test_file_header.c \
	qcow_test_functions.c qcow_test_functions.h \
	qcow_test_libbfio.h \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_unused.h

qcow_test_file_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_huffman_tree_SOURCES = \
	qcow_test_huffman_tree.c \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_unused.h

qcow_test_huffman_tree_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_io_handle_SOURCES = \
	qcow_test_io_handle.c \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_unused.h

qcow_test_io_handle_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_notify_SOURCES = \
	qcow_test_libcerror.h \
	qcow_test_libqcow.h \
	qcow_test_macros.h \
	qcow_test_notify.c \
	qcow_test_unused.h

qcow_test_notify_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_support_SOURCES = \
	qcow_test_functions.c qcow_test_functions.h \
	qcow_test_getopt.c qcow_test_getopt.h \
	qcow_test_libbfio.h \
	qcow_test_libcerror.h \
	qcow_test_libclocale.h \
	qcow_test_libcnotify.h \
	qcow_test_libqcow.h \
	qcow_test_libuna.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_support.c \
	qcow_test_unused.h

qcow_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_tools_info_handle_SOURCES = \
	../qcowtools/byte_size_string.c ../qcowtools/byte_size_string.h \
	../qcowtools/info_handle.c ../qcowtools/info_handle.h \
	qcow_test_libcerror.h \
	qcow_test_macros.h \
	qcow_test_memory.c qcow_test_memory.h \
	qcow_test_tools_info_handle.c \
	qcow_test_unused.h

qcow_test_tools_info_handle_LDADD = \
	@LIBCLOCALE_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

qcow_test_tools_output_SOURCES = \
	../qcowtools/qcowtools_output.c ../qcowtools/qcowtools_output.h \
	qcow_test_libcerror.h \
	qcow_test_macros.h \
	qcow_test_tools_output.c \
	qcow_test_unused.h

qcow_test_tools_output_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

qcow_test_tools_signal_SOURCES = \
	../qcowtools/qcowtools_signal.c ../qcowtools/qcowtools_signal.h \
	qcow_test_libcerror.h \
	qcow_test_macros.h \
	qcow_test_tools_signal.c \
	qcow_test_unused.h

qcow_test_tools_signal_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

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

test_inputs_libqcow.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libqcow

test_inputs_pyqcow.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyqcow

test_inputs_qcowinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) qcowinfo

test_library: \
	package.m4 \
	test_inputs_libqcow.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pyqcow.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_qcowinfo.at \
	test_macros.at \
	test_tools.at

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

