CC=g++
CXXFLAGS=-c -Wall
LDFLAGS += -ljsoncpp -lPocoNet -lPocoFoundation
SOURCES= client.cpp function.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CXXFLAGS) $< -o $@

clean:s
	rm -rf *.o $(EXECUTABLE)

