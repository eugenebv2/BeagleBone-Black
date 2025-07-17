
appname := gpio_test

CXX := g++
CXXFLAGS := -std=c++11 -g -Wall
LDFLAGS := -lm

# CXXFLAGS += $(shell pkg-config --cflags libgpiodcxx)
# LDLIBS += $(shell pkg-config --libs libgpiodcxx)

srcfiles := $(shell find . -maxdepth 1 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	    $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS) -pthread

depend: .depend

.depend: $(srcfiles)
	    rm -f ./.depend
	        $(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	    rm -f $(objects) $(appname)

dist-clean: clean
	    rm -f *~ .depend 

include .depend
