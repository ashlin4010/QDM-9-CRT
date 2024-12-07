# MCU and programmer
MCU = atmega328p
PROGRAMMER = arduino
F_CPU = 16000000UL
PORT = /dev/ttyUSB0
BAUD = 57600

# Files
TARGET = main
BUILD_DIR = build
CPP_FILES = main.cpp

# Compiler and flags
CC = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -g
LDFLAGS = -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref


# Rules
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).lss

# Generate the ELF file
$(BUILD_DIR)/$(TARGET).elf: $(CPP_FILES)
	$(CC) $(CFLAGS) $(CPP_FILES) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).elf

# Convert ELF to HEX
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

# Generate assembly listing for review (automatically after build)
$(BUILD_DIR)/$(TARGET).lss: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -S -d $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).lss

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Upload to the microcontroller
upload: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(BUILD_DIR)/$(TARGET).elf:e

# Clean the build
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).lss $(BUILD_DIR)/$(TARGET).map
