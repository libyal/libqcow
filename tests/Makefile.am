AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBQCOW_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYQCOW  = \
	test_pyqcow_open_close.sh
endif

TESTS = \
	test_open_close.sh \
	test_seek.sh \
	test_read.sh \
	test_qcowinfo.sh \
	$(TESTS_PYQCOW)

check_SCRIPTS = \
	pyqcow_test_open_close.py \
	test_open_close.sh \
	test_pyqcow_open_close.sh \
	test_qcowinfo.sh \
	test_read.sh \
	test_runner.sh \
	test_seek.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	qcow_test_open_close \
	qcow_test_read \
	qcow_test_seek

qcow_test_open_close_SOURCES = \
	qcow_test_libcerror.h \
	qcow_test_libcstring.h \
	qcow_test_libqcow.h \
	qcow_test_open_close.c \
	qcow_test_unused.h

qcow_test_open_close_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

qcow_test_read_SOURCES = \
	qcow_test_libcerror.h \
	qcow_test_libcstring.h \
	qcow_test_libqcow.h \
	qcow_test_read.c \
	qcow_test_unused.h

qcow_test_read_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

qcow_test_seek_SOURCES = \
	qcow_test_libcerror.h \
	qcow_test_libcstring.h \
	qcow_test_libqcow.h \
	qcow_test_seek.c \
	qcow_test_unused.h

qcow_test_seek_LDADD = \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

