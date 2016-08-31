SRC = ..\src\*.cpp ..\javni\*.cpp 
INCLUDES = ..\javni\*.h ..\h\*.h 
LIBS = ..\BCC\*.lib 
INCLUDEPATH = C:\bc31\include;..\javni;..\h;
LIBSPATH = C:\bc31\lib;..\BCC;
TARGET = KernelOS.exe
$(TARGET):	$(SRC) $(INCLUDES) configuration.xmi
	bcc.exe -mh -v -e$(TARGET) -I$(INCLUDEPATH) -L$(LIBSPATH) $(SRC) $(LIBS)
all:	$(TARGET)
clean:
	-del *.obj
	-del $(TARGET)
	-del *.asm
configuration.xmi:	
	configuration.xmi