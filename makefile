CC = gcc
CFLAGS = -Wall -g
GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

SRC = tictactoe_main.c tictactoe_gui.c tictactoe_gameops.c tictactoe_minimax.c tictactoe_twoplayers.c tictactoe_ml.c
OBJ = $(SRC:.c=.o)
TARGET = tictactoe

ifeq ($(OS),Windows_NT)
    TARGET_EXTENSION = .exe
    RM = del
else
    TARGET_EXTENSION =
    RM = rm -f
endif

all: $(TARGET)$(TARGET_EXTENSION)

$(TARGET)$(TARGET_EXTENSION): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET)$(TARGET_EXTENSION) $(OBJ) $(GTK_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(GTK_FLAGS)

clean:
	$(RM) $(OBJ)
