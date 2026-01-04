#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <gtk/gtk.h>
#include <string>

class TextEditor {
public:
    TextEditor();
    void run();

private:
    GtkWidget *window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;

    void setup_ui();
    void connect_signals();
    void update_title();

    std::string current_filename;

    static void on_open(GtkMenuItem *menuitem, gpointer user_data);
    static void on_save(GtkMenuItem *menuitem, gpointer user_data);
    static void on_save_as(GtkMenuItem *menuitem, gpointer user_data);
    static void on_exit(GtkMenuItem *menuitem, gpointer user_data);
    static void on_copy(GtkMenuItem *menuitem, gpointer user_data);
    static void on_paste(GtkMenuItem *menuitem, gpointer user_data);
    static void on_search(GtkMenuItem *menuitem, gpointer user_data);
    static void on_replace(GtkMenuItem *menuitem, gpointer user_data);
    static void on_about(GtkMenuItem *menuitem, gpointer user_data);
    static void on_font_properties(GtkMenuItem *menuitem, gpointer user_data);
    static void on_toggle_wrap(GtkCheckMenuItem *menuitem, gpointer user_data);
};

#endif // TEXT_EDITOR_H
