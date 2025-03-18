ROOTOBJDIR=obj
OBJDIR=$(ROOTOBJDIR)/$(PLATFORM)/$(CONFIGURATION)
INCDIR =../../include
SRCDIR =.
SRC =$(notdir $(wildcard $(SRCDIR)/*.cpp))
OBJ=$(addprefix $(OBJDIR)/, $(SRC:.cpp=.o)) 
CC =g++
CFLAGS =-I$(INCDIR) -DLICENSING_STATIC -fpermissive -fPIC
AR =ar
ARFLAGS =rcs
VPATH =$(SRCDIR)

all: $(OUTDIR)/licensing.a $(OUTDIR)/licensing.so
	@true

clean: 
	rm -f $(OBJ) $(OUTDIR)/licensing.a
	rm -Rf $(ROOTOBJDIR)

$(OUTDIR)/licensing.a: $(OBJ) | $(OUTDIR) 
	$(AR) $(ARFLAGS) $@ $?

$(OUTDIR)/licensing.so: $(OBJ) | $(OUTDIR) 
	$(CC) -shared -lcurl -lssl -lcrypto -o $@ -fPIC $?

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(GLOBALCFLAGS)

$(OUTDIR):
	mkdir -p $@

$(OBJDIR):
	mkdir -p $@
