# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Target microcontroller
TARGET = stm32f407vet6

# Directories
SRC_DIR = Src
INC_DIR = Inc
DRIVERS_DIR = Drivers
OBJ_DIR = Build
BIN_DIR = Bin
STARTUP_DIR = Startup

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS += $(DRIVERS_DIR)/system_stm32f4xx.c

# Startup file
STARTUP = $(STARTUP_DIR)/startup_stm32f407xx.s

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS += $(OBJ_DIR)/startup_stm32f407xx.o  # Explicitly include the startup object file


# Compiler flags
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -g -O0
CFLAGS += -I$(INC_DIR) -I$(DRIVERS_DIR)
CFLAGS += -DSTM32F407xx  # Define the target device

# Linker flags
LDFLAGS = -T$(DRIVERS_DIR)/STM32F407VETX_FLASH.ld
LDFLAGS += -specs=nosys.specs -specs=nano.specs
LDFLAGS += -Wl,--gc-sections

# Output files
ELF = $(BIN_DIR)/$(TARGET).elf
BIN = $(BIN_DIR)/$(TARGET).bin
HEX = $(BIN_DIR)/$(TARGET).hex

# Rules
all: $(BIN) $(HEX)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(STARTUP_DIR)/%.s
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<


$(OBJ_DIR)/%.o: $(DRIVERS_DIR)/%.s
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean