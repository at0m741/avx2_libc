LIB_NAME = SIMPL_libc.a
SO_NAME = SIMPL_libc.so
OBJ_DIR = obj
SRC_DIR = src
ASM_SRC_DIR = asm_src
INCLUDE_DIR = include
TEST_SRC = benchmark.c
TEST_BIN = benchmark

NASM_FLAGS = -g -F dwarf -I $(INCLUDE_DIR)
AS = nasm
CC = clang
AR = ar

# Define compiler and linker flags
CFLAGS = -O3 -mavx2 -masm=intel -mtune=native
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -MMD -MP -m64
PIC_FLAGS = -fPIC

# Define linker flags and version script
# CFLAGS += -fvisibility=hidden
LDFLAGS = -nostartfiles -nodefaultlibs
LDFLAGS += -Wl,--version-script=version.map -v

ifeq ($(TARGET), apple)
	LDFLAGS += -lc -lSystem
	NASM_FLAGS += -f macho64
else
	LDFLAGS += -lc
	NASM_FLAGS += -f elf64
endif

ifeq ($(VERBOSE), true)
	CFLAGS += -D VERBOSE
endif

# Source files
SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_FILES = $(shell find $(ASM_SRC_DIR) -type f -name '*.s')

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
ASM_OBJ = $(patsubst $(ASM_SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_FILES))

# Include directories
INCLUDE = -I $(INCLUDE_DIR)
INCLUDE += -I $(SRC_DIR)/config

all: $(OBJ_DIR) $(LIB_NAME) $(SO_NAME)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< into $@..."
	$(CC) $(CFLAGS) $(PIC_FLAGS) $(INCLUDE) -c $< -o $@
	@test -f $@ || (echo "Error: $@ is not a file! Something went wrong." && exit 1)

# Assemble .s files
$(OBJ_DIR)/%.o: $(ASM_SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	@echo "Assembling $< into $@..."
	$(AS) $(NASM_FLAGS) -o $@ $<
	@test -f $@ || (echo "Error: $@ is not a file! Something went wrong." && exit 1)

# Build static library
$(LIB_NAME): $(OBJ) $(ASM_OBJ)
	@echo "Building static library $(LIB_NAME)..."
	@$(AR) rcs $@ $(filter %.o,$^)

# Build shared library with version script
$(SO_NAME): $(OBJ) $(ASM_OBJ)
	@echo "Building shared library $@..."
	@$(CC) $(LDFLAGS) $(PIC_FLAGS) $(INCLUDE) -shared -o $@ $(OBJ) $(ASM_OBJ)
	@strip --strip-all $@

# Clean object files
clean:
	@echo "Removing object files..."
	@rm -rf $(OBJ_DIR)

# Full clean of libraries and executables
fclean: clean
	@echo "Removing libraries and executables..."
	@rm -f $(LIB_NAME) $(SO_NAME) $(TEST_BIN)

# Rebuild all
re: fclean all

.PHONY: all clean fclean re
