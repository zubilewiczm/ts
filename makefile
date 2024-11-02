SHELL = /bin/bash

##################################################################

PREFIX    =.
BIN       =ts
SRCDIR    =$(PREFIX)/src
ASMDIR    =$(PREFIX)/asm
BINDIR    =$(PREFIX)
OBJDIR    =$(PREFIX)/obj
LIBDIR    =$(PREFIX)/lib
LIBS      =
IGNORE    =
	  
C         =gcc
CFLAGS    =$(INCLUDES)
CXX       =g++
CXXFLAGS  =$(INCLUDES) -fdiagnostics-color=always -std=c++17
LDFLAGS   =$(LIBRARIES)
DBGFLAGS  =-O$(OPTLEVEL) -g$(GDBLEVEL)
PROFFLAGS =-pg
DEPFLAGS  =-MMD -MP

getcppfiles =$(shell ./get-cpp-files.sh $(1))

SRC := $(call getcppfiles,$(SRCDIR))
SRC := $(filter-out $(IGNORE),$(SRC))
OBJ := $(SRC:.cpp=.o)

INCLUDES  =$(SRCDIR:%=-I"%")
LIBRARIES =$(LIBDIR:%=-L"%") $(LIBS:%=-l%)
OBJFULL   =$(addprefix $(OBJDIR)/,$(OBJ))
ASMFULL   =$(addprefix $(ASMDIR)/,$(ASM))
DEPS      =$(OBJFULL:.o=.d)

OPTLEVEL  =g
GDBLEVEL  =3

LOG =2>&1 | tee -a $(PREFIX)/make.log

##################################################################

.PHONY: release debug asm clean all print remove-logs tests

debug : CFLAGS += $(DBGFLAGS) $(PROFFLAGS)
debug : CXXFLAGS += $(DBGFLAGS) $(PROFFLAGS)
debug : LDFLAGS += $(PROFFLAGS)
debug : all

release : all

asm : $(ASM)

clean :
	@rm -f $(OBJFULL) $(BINDIR)/$(BIN)
	@echo "Cleaned"

print :
	@echo "$(OBJ)"
	
all : $(BINDIR)/$(BIN)

##################################################################

$(BINDIR)/$(BIN) : $(OBJFULL)
	@echo → $@
	@$(CXX) -o $(BINDIR)/$(BIN) $(LDFLAGS) $(OBJFULL) $(LOG)

$(OBJFULL) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo → $<
	@mkdir -p $(@D)
	@$(CXX) -c -o $@ $(CXXFLAGS) $(DEPFLAGS) $< $(LOG)

-include $(DEPS)

-include tests/make.in
