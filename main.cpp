#include <gtk/gtk.h>
#include "text_editor.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    TextEditor editor;
    if (argc > 1) {
        if (!editor.open_file(argv[1])) {
            g_printerr("Failed to open file: %s\n", argv[1]);
        }
    }
    editor.run();

    return 0;
}
