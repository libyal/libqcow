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
	@LIBCAES_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBQCOW_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYQCOW = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_qcowinfo.sh \
	$(TESTS_PYQCOW)

check_SCRIPTS = \
	pyqcow_test_file.py \
	pyqcow_test_support.py \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_qcowinfo.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	qcow_test_cluster_block \
	qcow_test_cluster_table \
	qcow_test_deflate \
	qcow_test_error \
	qcow_test_file \
	qcow_test_file_header \
	qcow_test_io_handle \
	qcow_test_notify \
	qcow_test_support \
	qcow_test_tools_info_handle \
	qcow_test_tools_output \
	qcow_test_tools_signal

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
	@LIBCERROR_LIBADD@

qcow_test_tools_signal_SOURCES = \
	../qcowtools/qcowtools_signal.c ../qcowtools/qcowtools_signal.h \
	qcow_test_libcerror.h \
	qcow_test_macros.h \
	qcow_test_tools_signal.c \
	qcow_test_unused.h

qcow_test_tools_signal_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

