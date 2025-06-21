# Makefile para proyecto MCDU con fuentes en scr/ y headers en include/

TARGET = mcdu

SRCDIR = scr
INCDIR = include
BUILDDIR = build
BINDIR = bin

CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -I$(INCDIR)
LDFLAGS = -lncursesw -lhidapi-hidraw

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)/*.o $(BINDIR)/$(TARGET)
