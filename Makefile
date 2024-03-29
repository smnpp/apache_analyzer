CD=cd
RM=rm
RMFLAGS=-f
MKDIR=mkdir
MKDFLAGS=-p
COMP=g++
CCFLAGS=-ansi -pedantic -Wall -std=c++11 -g # -DMAP
LINK=g++
LNFLAGS=$(CCFLAGS)
LIBS=


BUILD=build
EXEDIR=bin
EXE=$(EXEDIR)/Analog
OBJDIR=obj
REAL=main.cpp Analyzer.cpp LogStream.cpp
OBJ=$(addprefix $(OBJDIR)/, $(REAL:.cpp=.o))

TEST=test
TESTDIR=Tests
TESTFILE=mktest.sh
TESTEXEDIR=../$(EXEDIR)

CLEAN=clean

$(BUILD): $(OBJ)
	$(MKDIR) $(MKDFLAGS) $(EXEDIR)
	$(LINK) -o $(EXE) $(OBJ) $(LIBS) $(LNFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(MKDIR) $(MKDFLAGS) $(OBJDIR)
	$(COMP) -o $@ -c $< $(CCFLAGS)

$(TEST):$(BUILD)
	$(CD) $(TESTDIR) && ./$(TESTFILE) $(TESTEXEDIR)

.PHONY:$(CLEAN)
$(CLEAN):
	$(RM) $(RMFLAGS) $(OBJDIR)/* $(EXEDIR)/* 