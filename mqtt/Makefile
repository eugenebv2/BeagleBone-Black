
#TARGET = i2ctest05
TARGET = mqtttest01

CC = gcc

LIBS = -li2c -lmosquitto -lm -lgpiod
CFLAGS := -g 

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
	rm -rf $(TARGET) $(OBJS)
