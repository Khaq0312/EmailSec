CC=g++
CXXFLAGS=-c -Wall
LDFLAGS += -ljsoncpp
SOURCES= client.cpp function.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CXXFLAGS) $< -o $@

clean:s
	rm -rf *.o $(EXECUTABLE)

