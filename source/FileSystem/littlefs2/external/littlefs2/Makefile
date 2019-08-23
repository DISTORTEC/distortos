TARGET = lfs2.a
ifneq ($(wildcard test.c main.c),)
override TARGET = lfs2
endif

CC ?= gcc
AR ?= ar
SIZE ?= size

SRC += $(wildcard *.c emubd/*.c)
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)
ASM := $(SRC:.c=.s)

TEST := $(patsubst tests/%.sh,%,$(wildcard tests/test_*))

SHELL = /bin/bash -o pipefail

ifdef DEBUG
override CFLAGS += -O0 -g3
else
override CFLAGS += -Os
endif
ifdef WORD
override CFLAGS += -m$(WORD)
endif
ifdef TRACE
override CFLAGS += -DLFS2_YES_TRACE
endif
override CFLAGS += -I.
override CFLAGS += -std=c99 -Wall -pedantic
override CFLAGS += -Wextra -Wshadow -Wjump-misses-init -Wundef
# Remove missing-field-initializers because of GCC bug
override CFLAGS += -Wno-missing-field-initializers


all: $(TARGET)

asm: $(ASM)

size: $(OBJ)
	$(SIZE) -t $^

.SUFFIXES:
test: \
	test_format \
	test_dirs \
	test_files \
	test_seek \
	test_truncate \
	test_entries \
	test_interspersed \
	test_alloc \
	test_paths \
	test_attrs \
	test_move \
	test_orphan \
	test_corrupt
	@rm test.c
test_%: tests/test_%.sh

ifdef QUIET
	@./$< | sed -nu '/^[-=]/p'
else
	./$<
endif

-include $(DEP)

lfs2: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

%.a: $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) -c -MMD $(CFLAGS) $< -o $@

%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)
	rm -f $(DEP)
	rm -f $(ASM)
