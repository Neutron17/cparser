CC = clang

PNAME = c

SRC  = $(shell find src -name "*.c")
OBJ  = $(SRC:.c=.o)
BIN = build

EXEC = $(BIN)/$(PNAME)
INCFLAGS  = -Isrc/

CCFLAGS += $(INCFLAGS)
CCFLAGS += -O0
CCFLAGS += -Wall

LDFLAGS  = $(INCFLAGS)
LDFLAGS += -ldebug

all: build

run: build
	$(BIN)/cparser $*

build: $(OBJ)
	$(CC) $(CCFLAGS) -ggdb -o $(BIN)/cparser $(filter %.o,$^) $(LDFLAGS)

clean:
	rm $(BIN)/* $(OBJ)

%.o: %.c
	$(CC) -ggdb -o $@ -c $< $(CCFLAGS)

