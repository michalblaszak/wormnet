CPPFLAGS := ""

CFLAGS := -Wall -g -std=c++20 -mthreads -DSHARED=1 -DUNICODE=1
CC = g++
INCLUDE =
LIBS := -lm -lws2_32 -lwsock32
#-lKERNEL32 -lUSER32 -lGDI32 -lCOMDLG32 -lWINSPOOL -lSHELL32 -lSHLWAPI -lOLE32 -lOLEAUT32 -lUUID -lADVAPI32 -lVERSION -lCOMCTL32 -lRPCRT4 -lWSOCK32 -lWININET -lWINMM -lODBC32 -lODBCCP32

LDFLAGS := -L . -L C:/msys64/mingw64/lib

INCLUDES := -I ./src/include -I C:/msys64/mingw64/include  -I C:/msys64/mingw64/include/asio -I C:/msys64/usr/local/include
#-I c:/wxWidgets/lib/gcc1220_x64_dll/mswud

#INC := src/orbits.h \
#    src/Configuration.h \

CPP := src/wormnet.cpp

OBJ := $(patsubst %.cpp,%.o,$(CPP))

%.o: %.cpp
	@echo $@
	$(CC) ${CFLAGS} ${INCLUDES} -c -o $@ $<
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

compile: $(OBJ)
	@echo "$@ : $<"
	${CC} ${CFLAGS} ${LDFLAGS} -o wormnet.exe ${OBJ} ${LIBS}

clean:
	rm -f wormnet.exe
	rm -f $(OBJ)

all: compile
	@echo "Making wormnet.exe"