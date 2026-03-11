# Makefile for zziplib
# Copyright (C) 2005 Peter Wang <wanghw@my169.com>.
# To install /usr/local/lib/libz.* and /usr/local/include/zlib.h, type:
#    make install
# To install in $HOME instead of /usr/local, use:
#    make install prefix=$HOME

#CROSS-COMPILE=arm-arago-linux-gnueabi-
#CROSS  = arm-hisiv100nptl-linux-

CC=$(CROSS)gcc

CFLAGS=-O3  -DZZIP_HAVE_STDINT_H=1
#CFLAGS=-O -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7
#CFLAGS=-g -DDEBUG
#CFLAGS=-O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
#           -Wstrict-prototypes -Wmissing-prototypes

LDFLAGS=libz.a -Wl,-elf2flt
LDSHARED=$(CC)
CPP=$(CC) -E

LIBS=libz.a

AR=$(CROSS)ar rc
RANLIB=$(CROSS)ranlib
TAR=tar
SHELL=/bin/sh
EXE=
CFLAGS += -I. -I../zlib -Izziplib

prefix = /usr/local
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
mandir = ${prefix}/share/man
man3dir = ${mandir}/man3

OBJS = \
	zzip-dir.o \
	zzip-err.o \
	zzip-file.o \
	zzip-info.o \
	zzip-io.o \
	zzip-ioapi2.o \
	zzip-stat.o \
	zzip-zip2.o \
	zzip-zip.o \
	dir.o	\
	err.o	\
	file.o	\
	info.o	\
	stat.o	\
	write.o	\
	zip.o	\
	plugin.o
	
	

LIBARY_HEADERS = \
	zzip-conf.h \
	zzip-config.h \
	zzip-io.h \
	zzip-msvc.h \
	zzip-stdint.h \
	zzip.h \
	zzipformat.h \
  zziplib.h	\
  __dirent.h	\
  zzip-conf.h	\
  zzip-config.h	\
  zzip-file.h	\
  zzip-io.h	\
  zzip-ioapi2.h	\
  zzip-plugin.h	\
  zzip-stdint.h
        
all: libzzip.a

libzzip.a: $(OBJS)
	$(AR) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1
	cp libzzip.a $(ALLOCA_DIR)/Package/

zzip-dir.o: zziplib/zzip-dir.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-err.o: zziplib/zzip-err.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-file.o: zziplib/zzip-file.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-info.o: zziplib/zzip-info.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-io.o: zziplib/zzip-io.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-ioapi2.o: zziplib/zzip-ioapi2.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-stat.o: zziplib/zzip-stat.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-zip2.o: zziplib/zzip-zip2.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zzip-zip.o: zziplib/zzip-zip2.c
	$(CC) -o $@ -c $<	$(CFLAGS)
dir.o:	zzip/dir.c
	$(CC) -o $@ -c $<	$(CFLAGS)
err.o:	zzip/err.c
	$(CC) -o $@ -c $<	$(CFLAGS)
file.o:	zzip/file.c
	$(CC) -o $@ -c $<	$(CFLAGS)
info.o:	zzip/info.c
	$(CC) -o $@ -c $<	$(CFLAGS)
stat.o:	zzip/stat.c
	$(CC) -o $@ -c $<	$(CFLAGS)
write.o:	zzip/write.c
	$(CC) -o $@ -c $<	$(CFLAGS)
zip.o:	zzip/zip.c
	$(CC) -o $@ -c $<	$(CFLAGS)
plugin.o:	zzip/plugin.c
	$(CC) -o $@ -c $<	$(CFLAGS)
clean:
	rm -f *.o *~
	rm -f libzzip.a