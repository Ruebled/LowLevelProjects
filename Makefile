include makefile.conf

BUILD_DIR = bin
SRC=src
PROJ=proj
INC=inc

SRCS := $(shell find $(SRC) -name '*.c')
PROJS += $(shell find $(PROJ) -name '$(PROJECT).c')

OBJS += $(BUILD_DIR)/startup_ARMCM4.o
OBJS += $(SRCS:$(SRC)/%.c=$(BUILD_DIR)/%.o)
OBJS += $(PROJS:$(PROJ)/$(PROJECT)%.c=$(BUILD_DIR)/$(PROJECT).o)

INC_DIRS := $(shell find $(INC) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

all: $(PROJECT).bin

$(BUILD_DIR)/%.o: ld/%.S
	mkdir -p $(dir $@)
	$(TOOLCHAIN) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(TOOLCHAIN) $(CFLAGS) -Iinc -c -o $@ $<

$(BUILD_DIR)/%.o: $(PROJ)/%.c
	mkdir -p $(dir $@)
	$(TOOLCHAIN) $(CFLAGS) -Iinc -c -o $@ $<

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).elf: $(OBJS)
	$(TOOLCHAIN) $(LDFLAGS) $^ $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)
	rm $(PROJECT)*

load:
	$(FLASHER) --reset write $(PROJECT).bin 0x08000000

erase:
	$(FLASHER) erase
