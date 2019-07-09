CXX=g++
CXXFLAGS= -O
LDFLAGS= -lGL -lGLU -lglut
OFOLDER=obj
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,$(OFOLDER)/%.o,$(SOURCES))
TARGET=run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) 

$(OFOLDER)/%.o: %.cpp %.hpp $(OFOLDER)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OFOLDER)/%.o: %.cpp $(OFOLDER)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OFOLDER):
	mkdir $(OFOLDER)

clean:
	rm -f $(OFOLDER)/*.o $(TARGET)
