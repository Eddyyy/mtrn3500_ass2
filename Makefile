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
MODULES=PMModule DummyModule LaserModule DisplayModule
ENTRYMODULE=PMModule
TARGET=runProject.out


all: clean $(TARGET)

$(TARGET): $(MODULES)
	ln -s $(BINDIR)/$(ENTRYMODULE) $@

#---------------Common Files Compilation----------------------------

$(ODIR)/%.o: $(COMMONDIR)/%.cpp $(INCLUDEDIR)/%.h $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR)/%.o: $(COMMONDIR)/%.cpp $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR):
	mkdir $(ODIR)

clean: $(addsuffix Clean,$(MODULES))
	rm -f $(ODIR)/*.o $(TARGET) $(BINDIR)/*

#----------------Module Specific Compilation------------------------

$(MODULES) : $(COMMONOBJS)
	cd $(MODULEDIR)/$@; \
	make all; \
	cd ../../; \
	ln $(MODULEDIR)/$@/run.out $(BINDIR)/$@;

$(addsuffix Clean, $(MODULES)):
	cd $(MODULEDIR)/$(subst Clean,,$@); \
	make clean; \
	cd ../../;
