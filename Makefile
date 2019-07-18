CXX=g++
CXXFLAGS= -O

INCLUDEDIR=include
COMMONDIR=common
BINDIR=bin
ODIR=obj

LDFLAGS=
INCLUDEFLAGS= -I$(INCLUDEDIR)
COMMONSRCS=$(wildcard $(COMMONDIR)/*.cpp)
COMMONOBJS=$(patsubst $(COMMONDIR)/%.cpp,$(ODIR)/%.o,$(COMMONSRCS))

MODULEDIR=src
MODULES=DisplayModule #PMModule LaserModule
ENTRYMODULE=DisplayModule
TARGET=runProject


all: $(TARGET)

#---------------Common Files Compilation----------------------------

$(TARGET): $(MODULES)
	ln -s $(BINDIR)/$(ENTRYMODULE) $@

$(ODIR)/%.o: $(COMMONDIR)/%.cpp $(INCLUDEDIR)/%.h $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR)/%.o: $(COMMONDIR)/%.cpp $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR):
	mkdir $(ODIR)

clean: $(addsuffix Clean,$(MODULES))
	rm -f $(ODIR)/*.o $(TARGET) $(BINDIR)/*

#----------------Module Specific Compilation------------------------

DisplayModule: $(COMMONOBJS)
	cd $(MODULEDIR)/$@; \
	make all; \
	cd ../../; \
	ln $(MODULEDIR)/$@/run $(BINDIR)/$@;

DisplayModuleClean:
	cd $(MODULEDIR)/DisplayModule; \
	make clean; \
	cd ../../;
