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
MODULES=PMModule CommandModule LaserModule #DisplayModule
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

PMModule: $(COMMONOBJS)
	cd $(MODULEDIR)/$@; \
	make all; \
	cd ../../; \
	ln $(MODULEDIR)/$@/run.out $(BINDIR)/$@;
	
PMModuleClean:
	cd $(MODULEDIR)/PMModule; \
	make clean; \
	cd ../../;

CommandModule: $(COMMONOBJS)
	cd $(MODULEDIR)/$@; \
	make all; \
	cd ../../; \
	ln $(MODULEDIR)/$@/run.out $(BINDIR)/$@;
	
CommandModuleClean:
	cd $(MODULEDIR)/CommandModule; \
	make clean; \
	cd ../../;

LaserModule: $(COMMONOBJS)
	cd $(MODULEDIR)/$@; \
	make all; \
	cd ../../; \
	ln $(MODULEDIR)/$@/run.out $(BINDIR)/$@;
	
LaserModuleClean:
	cd $(MODULEDIR)/LaserModule; \
	make clean; \
	cd ../../;

DisplayModule: $(COMMONOBJS)
	cd $(MODULEDIR)/$@; \
	make all; \
	cd ../../; \
	ln $(MODULEDIR)/$@/run.out $(BINDIR)/$@;

DisplayModuleClean:
	cd $(MODULEDIR)/DisplayModule; \
	make clean; \
	cd ../../;
