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
MODULES=PMModule DummyModule LaserModule DisplayModule GPSModule XboxModule VehicleModule
ENTRYMODULE=PMModule
TARGET=runProject.out


all: $(TARGET)

$(TARGET): $(MODULES)
	ln -fs $(BINDIR)/$(ENTRYMODULE) $@

#---------------Common Files Compilation----------------------------

$(ODIR)/%.o: $(COMMONDIR)/%.cpp $(INCLUDEDIR)/%.h | $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR)/%.o: $(COMMONDIR)/%.cpp | $(ODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c -o $@ $<

$(ODIR):
	-mkdir $(ODIR)

.PHONY: clean
clean: $(addsuffix Clean,$(MODULES))
	rm -rf $(ODIR)/*.o $(TARGET) $(BINDIR)/* $(ODIR)

#----------------Module Specific Compilation------------------------

$(MODULES) : $(COMMONOBJS)
	cd $(MODULEDIR)/$@ && $(MAKE) all
	ln -f $(MODULEDIR)/$@/run.out $(BINDIR)/$@

.PHONY: $(addsuffix Clean, $(MODULES))
$(addsuffix Clean, $(MODULES)):
	cd $(MODULEDIR)/$(subst Clean,,$@) &&$(MAKE) clean
