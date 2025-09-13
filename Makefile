# Compiler and flags
CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pg
LDFLAGS =

# Project files
TARGET = gol
SRCS = main.c game.c

# For animations with SDL3:
CFLAGS += -F/Library/Frameworks
LDFLAGS += -framework SDL3 -Wl,-rpath,/Library/Frameworks
SRCS += animations.c

OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile any .c file to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f *.o $(TARGET)