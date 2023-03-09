ODIR := obj
IDIR := .
SRC := src
OBJS := $(addprefix $(ODIR)/,args.o command.o history.o)
create_dir = $(shell [ ! -d $(@D) ] && mkdir -p $(@D))

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS := -g -std=gnu99 $(WARNINGS) -I$(IDIR)
CC := gcc

# -c  Compile or assemble the source files, but do not link.  The
# linking stage simply is not done.
# $@ = $(ODIR)/%.o
# $< = $(SRC)/%.c
$(ODIR)/%.o: $(SRC)/%.c $(IDIR)/include/%.h
	$(create_dir)
	$(CC) -c $< -o $@ $(CFLAGS)

# $^ = $(OBJS) osh.c
build/osh: $(OBJS) osh.c
	$(create_dir)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf $(ODIR)
	@rm -f osh