################################################################################
#
#  Makefile
#
#  This is the common part of the makefile that builds the FxLADSPA.so
#
################################################################################

#Define Compiler
CCC=gcc

#define Target
TARGET_NAME=libFxLADSPA
TARGET=$(TARGET_NAME).so -o $(TARGET_NAME).so

#define source path
FX_SOURCE=../../Src
COMMON_INCLUDE=../../../../Include

#define FxEngine library path
FXENGINE_LIB=../../../../Lib

#define FxEngine bin path
FXENGINE_BIN=../../../../Bin/

.SUFFIXES: .o .cpp

OBJECTS=$(FX_SOURCE)/FxLADSPA.o\
$(FX_SOURCE)/LADSPAWrapper.o\
$(FX_SOURCE)/load.o

#define C Flags
CFLAGS=-D_REENTRANT -fPIC -O3 -I$(FX_SOURCE) -I$(COMMON_INCLUDE) `pkg-config --cflags gtk+-2.0`

#define Libraries Flags
LDFLAGS=-shared -Wl,-soname,$(TARGET) -lFxEngine -L$(FXENGINE_LIB) `pkg-config --libs gtk+-2.0`

.cpp.o:
	@echo Compiling... $<
	@$(CCC) $(CFLAGS) -c $< -o $@

all: $(OBJECTS)
	@echo Linking ...
	@$(CCC) $(CFLAGS) $(LDFLAGS) $(OBJECTS)
	@echo Copying...
	@cp ./$(TARGET_NAME).so $(FXENGINE_BIN)

clean:
	@echo Cleaning...
	@-rm $(OBJECTS)
	@if ( test -f *.so* ) then rm *.so*; fi

 
