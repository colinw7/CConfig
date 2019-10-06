CC = g++
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(BIN_DIR)/CConfigTest

SRC = \
CConfigTest.cpp

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++14 \
-I$(INC_DIR) \
-I.

LFLAGS = \
-L$(LIB_DIR) \
-L../../CFile/lib \
-L../../CStrUtil/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-lCConfig -lCFile -lCStrUtil -lCUtil -lCOS

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CConfigTest

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CConfigTest: $(OBJS) $(LIB_DIR)/libCConfig.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CConfigTest $(OBJS) $(LFLAGS)
