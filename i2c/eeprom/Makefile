TARGET = i2cee

CC = gcc

LIBS = -li2c 
CFLAGS := -g -Wall 

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
.PRECIOUS: $(TARGETS) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	rm -rf i2cee $(OBJECTS)
