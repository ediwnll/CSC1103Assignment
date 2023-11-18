#include <gtk/gtk.h>

int gamemode = 0;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    create_mainwindow();
    create_gamewindow();
    create_difficultywindow();
    reset_game(); // Always run to reset the game
    // Start the GTK main loop
    gtk_main();

    return 0;
}
