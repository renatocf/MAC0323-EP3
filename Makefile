## PROGRAMS ###########################################################
CC := gcc
RM := rm -f
CP := cp -f
SED := sed -r
FMT := fmt -1
CAT := cat
MAKE += --no-print-directory
MKDIR := mkdir -p
RMDIR := rmdir --ignore-fail-on-non-empty

# DIRECTORIES ##########################################################
SRCDIR := .
OBJDIR := .
BINDIR := .
LIBDIR := .
DOCDIR := .
CONFDIR := .
TESTDIR := .
HEADDIR := .
VPATH = $(SRCDIR):$(LIBDIR):$(BINDIR):$(TESTDIR):$(HEADDIR)

## FILES ##############################################################
BIN := ep3
SRC := $(shell ls $(SRCDIR)/*.c)
LIB := $(CONFDIR)/libraries.mk
DEP := $(addprefix $(CONFDIR)/,$(SRC:.c=.d))

-include $(LIB)
OBJ := $(filter-out $(ARLIB) $(SOLIB),$(SRC)) # Tira bibliotecas
OBJ := $(patsubst %.c,%.o,$(OBJ))             # Substitui .c por .o
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))         # Adiciona diretório

## COMPILATION ########################################################
CLIBS  := -I. $(patsubst %,-I%,$(filter-out .%,$(shell find $(HEADDIR) -type d)))
CFLAGS := -ansi -Wall -pedantic -g 

## LINKAGE ############################################################
LDLIBS  := -L.
LDFLAGS := -lm
LDFLAGS += -Wl,-rpath,$(LIBDIR)
LDFLAGS += $(filter -l%,$(patsubst lib%.a,-l%,$(LIBS))) \
 		   $(filter -l%,$(patsubst lib%.so,-l%,$(LIBS)))

## BUILD ##############################################################
.PHONY: all
all: $(DEP) $(BIN) 
-include $(DEP)

.PHONY: doc
doc:
	$(MAKE) -C $(DOCDIR)

.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so
	$(RM) $(SRCDIR)/*~ $(HEADDIR)/*~
	$(RM) $(DEP)
	-$(RMDIR) $(OBJDIR) 2> /dev/null
ifneq ($(BINDIR),.)
	@-$(MAKE) clean -C $(DOCDIR)
endif

.PHONY: distclean
distclean: clean
	$(RM) $(BINDIR)/$(BIN)
	-$(RMDIR) $(BINDIR) $(LIBDIR) 2> /dev/null

## BUILD ##############################################################
$(BINDIR)/$(BIN): $(OBJ) | $(LIBS) $(BINDIR)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS)

$(OBJDIR)/%.o: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) $(CFLAGS) -c $< -o $@

$(CONFDIR)/%.d: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) -MM $< $(LDLIBS) 1> $*.d
	@$(CP)  $*.d $*.d.tmp
	@$(SED) -e 's/.*:/$(OBJDIR)\/$*.o:/' -i $*.d
	@$(SED) -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | $(FMT) | \
	  $(SED) -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@$(RM) $*.d.tmp

lib%.so: $(SRCDIR)/%.c
	$(CC) -fPIC $(CFLAGS) $(CLIBS) -c $< -o $(OBJDIR)/$*.o
	$(CC) -o $(LIBDIR)/$@ $(SOFLAGS) $(OBJDIR)/$*.o 

lib%.a: $(OBJDIR)/$(notdir %.o)
	$(AR) $(ARFLAGS) $(LIBDIR)/$@ $<

# TESTS ################################################################
%: $(TESTDIR)/%.c
	$(CC) $< $(CFLAGS) $(CLIBS) -o $(BINDIR)/test$* $(LDLIBS) $(LDFLAGS)

# GENERATED DIR ########################################################
ifneq ($(BINDIR),.)
$(BINDIR):
	@ echo Criando diretório de binários "$@"
	-$(MKDIR) $@
endif

ifneq ($(BINDIR),.)
$(OBJDIR):
	@ echo Criando diretório de objetos "$@"
	-$(MKDIR) $@
endif

ifneq ($(BINDIR),.)
$(LIBDIR):
	@ echo Criando diretório de objetos "$@"
	-$(MKDIR) $@
endif
