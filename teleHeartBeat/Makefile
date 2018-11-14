TINYXML_ROOT	:="tinyxml/"



################OPTION###################
CCOMPILE = g++
CPPCOMPILE = g++ -g  -fpic -Wall 
COMPILEOPTION = $(COM_MODE)
INCLUDEDIR = -I /home/pas36/include/
LINK = g++
LINKOPTION =  -Wl,-export-dynamic -fpic -g -o $(OUTPUT) $(COM_MODE)
LIBDIRS = -L /home/pas36/  
OBJS = HeartBeat.o

OBJSADD =  
       
OUTPUT = heartbeat
SHAREDLIB =  -lrt  -lpthread -ldl
APPENDLIB = /home/pas36/lib/paslibadv.a /home/pas36/lib/paslibtools.a


OTHERFILE = 

USERFLAGS = -DDEBUG
################OPTION END################
$(OUTPUT):$(OBJS) $(APPENDLIB) $(OBJSADD)
	$(LINK)  $(LINKOPTION)  $(LIBDIRS) $(OBJS) $(OBJSADD) $(SHAREDLIB) $(APPENDLIB) $(USERFLAGS) 
	cp -f $(OUTPUT) ../../VolTele/$(OUTPUT)
	rm -rf $(OBJS) 
	

clean: 
	rm -rf $(OUTPUT) $(OTHERFILE) $(OBJS) 
all: clean $(OUTPUT)
.PRECIOUS:%.cpp %.c %.C
.SUFFIXES:
.SUFFIXES:  .c .o .cpp .ecpp .pc .ec .C .cc .cxx .h .y


.cpp.o:
	$(CPPCOMPILE)  -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR) $(USERFLAGS)  $*.cpp
	
.cc.o:
	$(CCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cpp

.cxx.o:
	$(CPPCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cpp

.c.o:
	$(CCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR) $*.c

.C.o:
	$(CPPCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR) $*.C	



