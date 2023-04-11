include makefile.conf


BUILD_DIR = bin
OBJ = obj

LIBS=libs
SRC=src
INC=-Iinc

SRCS := $(shell find $(SRC) -name '*.c')
OBJS += $(SRCS:$(SRC)%.c=$(BUILD_DIR)%.o)
OBJS += $(BUILD_DIR)/startup_ARMCM4.o

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

all: $(PROJECT).bin

$(BUILD_DIR)/%.o: ld/%.S
	mkdir -p $(dir $@)
	$(TOOLCHAIN) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(TOOLCHAIN) $(CFLAGS) $(INC) -c -o $@ $<

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).elf: $(OBJS)
	$(TOOLCHAIN) $(LDFLAGS) $^ $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)
	rm terminal*

load:
	$(FLASHER) write $(PROJECT).bin 0x08000000

erase:
	$(FLASHER) erase
