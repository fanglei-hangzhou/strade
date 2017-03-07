#源文件，自动找所有.c和.cpp文件，并将目标定义为同名.o文件
SOURCE  := $(wildcard *.c) $(wildcard *.cc)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#target you can change test to what you want
#目标文件名，输入任意你想要的执行文件名
BASE_DIR = ./../base
LIB_DIR = ./../../../../library
USER_DIR = ./../../pub
SO_DIR = ./../../../bin/plugins
STOCKSVC = ./../../plugins
ARITHMETIC = $(USER_DIR)pub/arithmetic/
CUSTOM_THIRD_PATH = $(LIB_DIR)/third_party
TARGET  :=  $(SO_DIR)/candlestick/libcandlestick_core.so.1
#compile and lib parameter
#编译参数
CC      := g++
LIBS    :=  
LDFLAGS := 
DEFINES :=
INCLUDE := -I. -I.. 
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H -isystem -shared -fPIC -DDB_POOL_ -D_DB_POOL_ -D_DEBUG -DUSR_LOG_MSG -DPROCESS_WORK -g -Wextra
SHARE := -fPIC -shared -o
  
  
#i think you should do anything here
#下面的基本上不需要做任何改动了
.PHONY : everything objs clean veryclean rebuild
  
everything : $(TARGET)
  
all : $(TARGET)
  
objs : $(OBJS)
  
rebuild: veryclean everything
                
clean :
	rm -fr *.so
	rm -fr *.o
	rm -fr $(TARGET)
veryclean : clean
	rm -fr $(TARGET)
  
$(TARGET) : $(OBJS) 
	$(CC) $(CXXFLAGS) $(SHARE)  $@ $(OBJS) $(LDFLAGS) $(LIBS)
