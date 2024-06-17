# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Libraries
LIBS = -lGL -lGLEW -lglfw

# Source files
SOURCES = main.cpp

# Executable name
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LIBS)

clean:
	rm -f $(EXECUTABLE)
