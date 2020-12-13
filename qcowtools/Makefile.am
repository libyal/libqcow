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
	@LIBFUSE_CPPFLAGS@ \
	@LIBQCOW_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	qcowinfo \
	qcowmount

qcowinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	info_handle.c info_handle.h \
	qcowinfo.c \
	qcowtools_getopt.c qcowtools_getopt.h \
	qcowtools_i18n.h \
	qcowtools_libbfio.h \
	qcowtools_libcdata.h \
	qcowtools_libcerror.h \
	qcowtools_libclocale.h \
	qcowtools_libcnotify.h \
	qcowtools_libqcow.h \
	qcowtools_libuna.h \
	qcowtools_output.c qcowtools_output.h \
	qcowtools_signal.c qcowtools_signal.h \
	qcowtools_unused.h

qcowinfo_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

qcowmount_SOURCES = \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h \
	qcowmount.c \
	qcowtools_getopt.c qcowtools_getopt.h \
	qcowtools_i18n.h \
	qcowtools_libbfio.h \
	qcowtools_libcdata.h \
	qcowtools_libcerror.h \
	qcowtools_libclocale.h \
	qcowtools_libcnotify.h \
	qcowtools_libcpath.h \
	qcowtools_libqcow.h \
	qcowtools_libuna.h \
	qcowtools_output.c qcowtools_output.h \
	qcowtools_signal.c qcowtools_signal.h \
	qcowtools_unused.h

qcowmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libqcow/libqcow.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on qcowinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(qcowinfo_SOURCES)
	@echo "Running splint on qcowmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(qcowmount_SOURCES)

