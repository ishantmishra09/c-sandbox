CC = gcc
CFLAGS = -Wall -Wextra -Werror

FLOAT_TARGET = dvoid_float
STRUCT_TARGET = dvoid_struct

COMMON_SRC = src/dynarr_void.c

all: $(FLOAT_TARGET) $(STRUCT_TARGET)

$(FLOAT_TARGET): examples/dvoid_float.c $(COMMON_SRC)
	$(CC) $(CFLAGS) $^ -o $@

$(STRUCT_TARGET): examples/dvoid_struct.c $(COMMON_SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(FLOAT_TARGET) $(STRUCT_TARGET)

.PHONY: all clean
