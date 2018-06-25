
# General
CC			= gcc
LD			= ld
RM			= rm -rf
RMDIR		= rmdir
INSTALL		= install
DEBUG		= -ggdb -O1 -march=native -ftrapv

## CHANGE THIS ##
#Define the targets
TARGETMF	= myMinifilter.sys
TARGET1 	= MFManager
TARGET2 	= Logical

#define the sources. The seperation is needed because minifilter and others go through different kind of compilation process
SRCDIRMF 	= src/Minifilter
SRCDIR 		= src/MFManager

#define the destination of the objects files
OBJDIR		= obj

#define for each target it's needed objects in order to start
OBJMF	= list.o minifilter.o filter_commun.o
OBJ1	= #TODO write needed objects
OBJ2 	=

#append to each object file the OBJDIR
OBJMF 	:= $(addprefix $(OBJDIR)/, $(OBJMF))
OBJ1 	:= $(addprefix $(OBJDIR)/, $(OBJ1))
OBJ2 	:= $(addprefix $(OBJDIR)/, $(OBJ2))

#where the targets should be saved
BINDIR		= bin

#where the headers are stored
HEADER 	 	= includes
## CHANGE THIS ##

# CFLAGS, LDFLAGS, CPPFLAGS, PREFIX can be overriden on CLI
CFLAGS		:= $(DEBUG) 
LDFLAGS		:=
PREFIX		:= 
TARGET_ARCH :=


# Compiler Flags
CFLAGSEXE	:= $(CFLAGS)
CFLAGSEXE	+= -Wall -Wextra -pedantic -ansi
CFLAGSEXE	+= -fno-strict-aliasing
CFLAGSEXE	+= -Wuninitialized -Winit-self -Wfloat-equal
CFLAGSEXE	+= -Wundef -Wshadow -Wc++-compat -Wcast-qual -Wcast-align
CFLAGSEXE	+= -Wconversion -Wsign-conversion -Wjump-misses-init
CFLAGSEXE	+= -Wno-multichar -Wpacked -Wstrict-overflow -Wvla
CFLAGSEXE	+= -Wformat -Wno-format-zero-length -Wstrict-prototypes
CFLAGSEXE	+= -Wno-unknown-warning-option

CFLAGSSYS 	:= $(CFLAGS)
CFLAGSSYS   += -I /mingw/i686-w64-mingw32/include/ddk -DMINGW -std=c99 -Wall -m64 -shared

# Preprocessor Flags

# Linker Flags
ALL_LDFLAGS		:= $(LDFLAGS)
ALL_LDLIBS		:= -lc
LD_SYS_FLAGS    := -Wl,--subsystem,native -Wl,--image-base,0x10000 \
LD_SYS_FLAGS	+= -Wl,--file-alignment,0x1000 -Wl,
LD_SYS_FLAGS	+= --section-alignment,0x1000 \
LD_SYS_FLAGS	+= -Wl,--entry,DriverEntry@8 -Wl,--stack,0x40000
LD_SYS_FLAGS	+= -Wl,--dynamicbase -Wl,--nxcompat \
-nostartfiles -nostdlib -lntoskrnl -lhal



# Source, Binaries, Dependencies

SRC	 		:= $(shell find $(SRCDIR) -type f -name '*.c')

DEP			:= $(OBJ:.o=.d)
BINMF		:= $(BINDIR)/$(TARGETMF)
BIN1		:= $(BINDIR)/$(TARGET1)
BIN2		:= $(BINDIR)/$(TARGET2)
-include $(DEP)


# Verbosity Control, ala automake
V 			= 0

# Verbosity for CC
REAL_CC 	:= $(CC)
CC_0 		= @echo "CC $<"; $(REAL_CC)
CC_1 		= $(REAL_CC)
CC 			= $(CC_$(V))

# Verbosity for LD
REAL_LD 	:= $(LD)
LD_0 		= @echo "LD $@"; $(REAL_LD)
LD_1 		= $(REAL_LD)
LD 			= $(LD_$(V))

# Verbosity for RM
REAL_RM 	:= $(RM)
RM_0 		= @echo "Cleaning..."; $(REAL_RM)
RM_1 		= $(REAL_RM)
RM 			= $(RM_$(V))

# Verbosity for RMDIR
REAL_RMDIR 	:= $(RMDIR)
RMDIR_0 	= @$(REAL_RMDIR)
RMDIR_1 	= $(REAL_RMDIR)
RMDIR 		= $(RMDIR_$(V))



# Build Rules
.PHONY: clean
.DEFAULT_GOAL := all

all: setup $(BIN1) $(BIN2) $(BINMF)
setup: dir
remake: clean all

dir:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


$(BIN1): $(OBJ1) 
	$(LD) $(ALL_LDFLAGS) $^ $(ALL_LDLIBS) -o $@

$(BIN2): $(OBJ2)
	$(LD) $(ALL_LDFLAGS) $^ $(ALL_LDLIBS) -o $@

$(BINMF): $(OBJMF)
	$(LD) $(LD_SYS_FLAGS) $^ $(ALL_LDLIBS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGSEXE) -c -I$(HEADER) -MMD -MP -o $@ $< 


$(OBJDIR)/%.o: $(SRCDIRMF)/%.c
	$(CC) $(CFLAGSSYS) -c -I$(HEADER) -MMD -MP -o $@ $<


#TODO: write this later if needed
#install: $(BIN1) $(BIN2) $(BINMF)
#	$(INSTALL) -d $(PREFIX)/bin
#	$(INSTALL) $(BIN) $(PREFIX)/bin


#TODO: maybe write it later
#clean: rewrite
#//	$(RM) $(OBJ) $(DEP) $(BIN)
#	$(RMDIR) $(OBJDIR) $(BINDIR) 2> /dev/null; true
