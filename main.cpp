#include <gtk/gtk.h>
#include "text_editor.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    TextEditor editor;
    editor.run();

    return 0;
}
