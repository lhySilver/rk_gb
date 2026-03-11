###MW_ROOT := $(shell pwd)

DIR_INC = ..
DIR_SRC = .

SRC = $(wildcard $(DIR_SRC)/*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

TARGET = daemon


CROSS = arm-rockchip830-linux-uclibcgnueabihf-

CC = $(CROSS)gcc
CXX = $(CROSS)g++
STRIP = $(CROSS)strip

CFLAGS += -g
#CFLAGS += -I ./include

#LIBTI_DIR = ./lib

#LDFLAGS	+= $(LIBTI_DIR)/libInfra.a
#LDFLAGS	+= $(LIBTI_DIR)/libjson.a
#LDFLAGS	+= $(LIBTI_DIR)/libzzip.a
#LDFLAGS	+= $(LIBTI_DIR)/libz.a
#LDFLAGS	+= $(LIBTI_DIR)/libLog.a
#LDFLAGS	+= $(LIBTI_DIR)/libDebugInfo.a
LDFLAGS = -ldl
LDFLAGS += -lpthread
LDFLAGS += -lstdc++




$(TARGET) : $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	chmod +x $@
	$(STRIP) $@

$(DIR_SRC)/%.o : $(DIR_SRC)/%.cpp
	echo $(CXX)
	$(CXX) $(CFLAGS) -c $< -o $@

clean :
	-rm *.o
	-rm $(TARGET)