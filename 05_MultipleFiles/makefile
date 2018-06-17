CC=zcc
AS=zcc
TARGET=+zx
VERBOSITY=-vn
CRT=4

PRAGMA_FILE=zpragma.inc

C_OPT_FLAGS=-SO3 --max-allocs-per-node200000

CFLAGS=$(TARGET) $(VERBOSITY) -c $(C_OPT_FLAGS) -compiler sdcc -clib=sdcc_iy -pragma-include:$(PRAGMA_FILE)
LDFLAGS=$(TARGET) $(VERBOSITY) -clib=sdcc_iy -pragma-include:$(PRAGMA_FILE)
ASFLAGS=$(TARGET) $(VERBOSITY) -c


EXEC=text.tap
EXEC_OUTPUT=text

OBJECTS = text_main.o \
          text_via_makefile.o

%.o: %.c $(PRAGMA_FILE)
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

all : $(EXEC)

$(EXEC) : $(OBJECTS)
	 $(CC) $(LDFLAGS) -startup=$(CRT) $(OBJECTS) -o $(EXEC_OUTPUT) -create-app

.PHONY: clean
clean:
	rm -f *.o *.bin *.tap *.map zcc_opt.def *~ /tmp/tmpXX*
