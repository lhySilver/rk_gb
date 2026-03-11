AUTOMAKE_OPTIONS = 1.4 foreign
AUTOTOOL_VERSION=autoconf-2.52 automake-1.5 libtool-1.4.2
DEFAULT_INCLUDES = # nothing - no default of -I. -I$(srcdir)
DEFS = @DEFS@ -I$(top_builddir) -I$(top_srcdir) # also for automake 1.4

lib_LTLIBRARIES = libzzip.la
zzipdir = ${includedir}/zzip
zzip_HEADERS = $(libzzip_la_headers)
noinst_HEADERS = __debug.h __hints.h __mmap.h __dirent.h
#
VERSION_INFO=@VERSION_INFO@
RELEASE_INFO=@RELEASE_INFO@
THREAD_SAFE=@THREAD_SAFE@
#

libzzip_la_SOURCES = \
	zip.c \
	file.c \
	dir.c \
	stat.c \
	info.c \
	err.c \
	plugin.c \
	write.c
libzzip_la_headers = \
	stdint.h \
	file.h \
	plugin.h \
	lib.h \
	zzip.h \
	format.h \
	conf.h \
	_config.h \
        _msvc.h
libzzip_la_LDFLAGS= @ZZIPLIB_LDFLAGS@ $(RELEASE_INFO) $(VERSION_INFO) \
                    $(THREAD_SAFE)
libzzip_la_LIBADD= -lz @RESOLVES@

WCC10_DIST = zziplib.wpj
WCC10_CLEAN = *.obj *.lk1 *.mk1 *.mk *.sym *.tgt

EXTRA_DIST =  $(WCC10_DIST) _msvc.sed 
CONFIG_CLEAN_FILES = _config.h 
MAINTAINERCLEANFILES = _msvc.h
CLEANFILES = $(WCC10_CLEAN)  *.pc *.omf

# here is some backward compatibility with version 0.10.* of zziplib
install-data-local : install-zzipHEADERS
	echo '#ifndef ZZIP_WARNING'   > $(DESTDIR)$(includedir)/zzip.h
	echo '#define ZZIP_WARNING 1' >>$(DESTDIR)$(includedir)/zzip.h
	echo '#ifdef __GNUC__'        >>$(DESTDIR)$(includedir)/zzip.h
	echo '#warning do no not use <zzip.h>, update to include <zzip/lib.h>'\
	                              >>$(DESTDIR)$(includedir)/zzip.h
	echo '#endif'                 >>$(DESTDIR)$(includedir)/zzip.h
	echo '#endif'                 >>$(DESTDIR)$(includedir)/zzip.h
	echo '#include "zzip/lib.h"'  >>$(DESTDIR)$(includedir)/zzip.h
	sed -e 's|zzip.h|zziplib.h|' -e 's|zzip/lib.h|zzip/zzip.h|' \
	$(DESTDIR)$(includedir)/zzip.h >$(DESTDIR)$(includedir)/zziplib.h
	sed -e 's|zzip.h|zzip-io.h|' -e 's|zzip/lib.h|zzip/plugin.h|' \
	$(DESTDIR)$(includedir)/zzip.h >$(DESTDIR)$(includedir)/zzip-io.h
	sed -e 's|zzip.h|zzipwrap.h|' -e 's|zzip/lib.h|zzip/wrap.h|' \
	$(DESTDIR)$(includedir)/zzip.h >$(DESTDIR)$(includedir)/zzipwrap.h
	sed -e 's|zzip.h|zzipformat.h|' -e 's|zzip/lib.h|zzip/format.h|' \
	$(DESTDIR)$(includedir)/zzip.h >$(DESTDIR)$(includedir)/zzipformat.h

uninstall-local :
	(cd $(DESTDIR)$(includedir) && rm zziplib.h  zzip.h  zzip-io.h)
	(cd $(DESTDIR)$(includedir) && rm zzipwrap.h zzipformat.h)


# ----------------------------------------------------------------------
pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_HEADERS= zzip-zlib-config.pc zziplib.pc

zzip-zlib-config.pc : Makefile
	@ echo "zlib_libs=@ZLIB_LDIR@ -lz" >$@
	@ echo "zlib_cflags=@ZLIB_INCL@" >>$@
	@ echo " " >>$@
	@ echo "Name: zzip-zlib-config" >>$@
	@ echo "Version: @ZLIB_VERSION@" >>$@
	@ echo "Description: ZLib Config (for ZZipLib)" >>$@
	echo 'Libs: $${zlib_libs}' >>$@
	echo 'Cflags: $${zlib_cflags}' >>$@

zziplib.pc : Makefile
	@ echo "prefix=$(prefix)" >$@
	@ echo "libdir=$(libdir)" >>$@
	@ echo "includedir=$(includedir)" >>$@
	@ echo "largefile=$(LARGEFILE_CFLAGS)" >>$@
	@ echo "zlib_libs=@ZLIB_LDIR@ -lz" >>$@
	@ echo "zlib_cflags=@ZLIB_INCL@" >>$@
	@ echo " " >>$@
	@ echo "Name: zziplib" >>$@
	@ echo "Version: $(VERSION)" >>$@
	@ head $(top_srcdir)/$(PACKAGE).spec \
	| grep Summary | sed -e s,Summary,Description, >> $@
	echo 'Requires: zzip-zlib-config' >>$@
	echo 'Libs: -L$${libdir} -lzzip' >>$@
	echo 'Cflags: $${largefile} -I$${includedir}' >>$@
