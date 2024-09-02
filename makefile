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
	  
C         =gcc
CFLAGS    =$(INCLUDES) -MMD -MP
CXX       =g++
CXXFLAGS  =$(INCLUDES) -MMD -MP -fdiagnostics-color=always
LDFLAGS   =$(LIBRARIES)
DBGFLAGS  =-O$(OPTLEVEL) -g$(GDBLEVEL)

OBJ := main.o Var.o Type.o Term.o Util.o Exception.o \
			 ArgList.o Symbol.o \
			 interfaces/ITerm.o interfaces/IUIDComparable.o

INCLUDES  =$(SRCDIR:%=-I"%")
LIBRARIES =$(LIBDIR:%=-L"%") $(LIBS:%=-l%)
OBJFULL   =$(addprefix $(OBJDIR)/,$(OBJ))
ASMFULL   =$(addprefix $(ASMDIR)/,$(ASM))
DEPS      =$(OBJFULL:.o=.d)

OPTLEVEL  =0
GDBLEVEL  =3

LOG =2>&1 | tee -a $(PREFIX)/make.log

##################################################################

.PHONY: release asm clean all print remove-logs
	
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
	$(CXX) -o $(BINDIR)/$(BIN) $(LDFLAGS) $(OBJFULL) $(LOG)

$(OBJFULL) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo $< $(@D)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $(CXXFLAGS) $(DBGFLAGS) $< $(LOG)

-include $(DEPS)
