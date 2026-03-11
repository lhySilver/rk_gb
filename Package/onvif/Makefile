
include config

TOPDIR = ./
SRCDIR = ./

#CFLAGS	+= -I$(TOPDIR)../../Include
CFLAGS	+= -I$(SRCDIR)inc
CFLAGS	+= -I$(SRCDIR)
CFLAGS  += -Wall -O2 -Os 


AFLAGS	+= -rv


LIB_TARGET= $(LIBDIR)libOnvif.a

LIB_OBJS = base64.o \
			commfun.o \
			hxml.o \
			onvif.o \
			onvif_discovery.o \
			onvif_error_respones.o \
			onvif_function.o \
			onvif_serve.o \
			onvif_threadpool.o \
			sha1.o \
			xml_get_data.o \
			xml_node.o \
#TEST_OBJS = Test/Main.o	

#TEST_TARGET = $(BINDIR)/Log

#TARGET	= $(LIB_TARGET) $(TEST_TARGET)
TARGET	= $(LIB_TARGET)
all: $(TARGET)
	
$(LIB_TARGET): $(LIB_OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $^
#	cp $@ $(ALLOCA_DIR)/Package/

#$(TEST_TARGET): $(TEST_OBJS) $(EXTLIBS) $(LIB_TARGET)
#	$(CPP) -static -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(LIB_OBJS) $(TEST_OBJS);

