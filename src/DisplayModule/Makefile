CXX=g++
CXXFLAGS= -O
LDFLAGS= -lGL -lGLU -lglut
INCLUDEFLAGS= -I../../include
ODIR=obj
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,$(ODIR)/%.o,$(SOURCES))
TARGET=run.out

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ ../../obj/*.o $(LDFLAGS)

$(ODIR)/%.o: %.cpp %.hpp | $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR)/%.o: %.cpp | $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR):
	-mkdir $(ODIR)

.PHONY: clean
clean:
	rm -rf $(ODIR)/*.o $(TARGET) $(ODIR)
