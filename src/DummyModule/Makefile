CXX=g++
CXXFLAGS= -Wall -Werror -O
LDFLAGS=
INCLUDEFLAGS= -I../../include
ODIR=obj
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,$(ODIR)/%.o,$(SOURCES))
TARGET=run.out

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ ../../obj/*.o $(LDFLAGS)

$(ODIR)/%.o: %.cpp %.h $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR)/%.o: %.cpp $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR):
	mkdir $(ODIR)

clean:
	rm -f $(ODIR)/*.o $(TARGET)
