CXX=g++
CXXFLAGS= -O
LDFLAGS= -lGL -lGLU -lglut
INCLUDEFLAGS= -I../../include
OFOLDER=obj
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,$(OFOLDER)/%.o,$(SOURCES))
TARGET=run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ ../../obj/*.o $(LDFLAGS)

$(OFOLDER)/%.o: %.cpp %.hpp $(OFOLDER)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(OFOLDER)/%.o: %.cpp $(OFOLDER)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(OFOLDER):
	mkdir $(OFOLDER)

clean:
	rm -f $(OFOLDER)/*.o $(TARGET)
