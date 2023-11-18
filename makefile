CC = gcc
CFLAGS = -Wall -g
LIBS = `pkg-config --cflags --libs gtk+-3.0`

# Separate variables for compilation and linking flags
GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

SRC = tictactoe_main.c tictactoe_gui.c tictactoe_gameops.c tictactoe_minimax.c tictactoe_twoplayers.c
OBJ = $(SRC:.c=.o)
TARGET = tictactoe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(GTK_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(GTK_CFLAGS)

clean:
	rm -f $(OBJ)
