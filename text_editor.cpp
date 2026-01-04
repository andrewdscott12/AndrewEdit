#include "text_editor.h"
#include <pango/pango.h>

TextEditor::TextEditor() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Set initial title
    gtk_window_set_title(GTK_WINDOW(window), "AndrewEdit");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *menubar = gtk_menu_bar_new();

    /* Pack menubar at the top */
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    text_view = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    GtkWidget *file_menu = gtk_menu_item_new_with_label("File");
    GtkMenu *file_popup = GTK_MENU(gtk_menu_new());
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu), GTK_WIDGET(file_popup));

    GtkWidget *open_item = gtk_menu_item_new_with_label("Open");
    g_signal_connect(open_item, "activate", G_CALLBACK(on_open), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_popup), open_item);

    GtkWidget *save_item = gtk_menu_item_new_with_label("Save");
    g_signal_connect(save_item, "activate", G_CALLBACK(on_save), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_popup), save_item);

    GtkWidget *save_as_item = gtk_menu_item_new_with_label("Save As...");
    g_signal_connect(save_as_item, "activate", G_CALLBACK(on_save_as), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_popup), save_as_item);

    GtkWidget *exit_item = gtk_menu_item_new_with_label("Exit");
    g_signal_connect(exit_item, "activate", G_CALLBACK(on_exit), GTK_WINDOW(window));
    gtk_menu_shell_append(GTK_MENU_SHELL(file_popup), exit_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu);

    GtkWidget *edit_menu = gtk_menu_item_new_with_label("Edit");
    GtkMenu *edit_popup = GTK_MENU(gtk_menu_new());
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_menu), GTK_WIDGET(edit_popup));

    GtkWidget *copy_item = gtk_menu_item_new_with_label("Copy");
    g_signal_connect(copy_item, "activate", G_CALLBACK(on_copy), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_popup), copy_item);

    GtkWidget *paste_item = gtk_menu_item_new_with_label("Paste");
    g_signal_connect(paste_item, "activate", G_CALLBACK(on_paste), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_popup), paste_item);

    GtkWidget *search_item = gtk_menu_item_new_with_label("Search...");
    g_signal_connect(search_item, "activate", G_CALLBACK(on_search), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_popup), search_item);

    GtkWidget *replace_item = gtk_menu_item_new_with_label("Replace...");
    g_signal_connect(replace_item, "activate", G_CALLBACK(on_replace), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_popup), replace_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit_menu);

    /* View menu */
    GtkWidget *view_menu = gtk_menu_item_new_with_label("View");
    GtkMenu *view_popup = GTK_MENU(gtk_menu_new());
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_menu), GTK_WIDGET(view_popup));

    GtkWidget *font_item = gtk_menu_item_new_with_label("Font Properties...");
    g_signal_connect(font_item, "activate", G_CALLBACK(on_font_properties), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_popup), font_item);

    GtkWidget *wrap_item = gtk_check_menu_item_new_with_label("Word Wrap");
    g_signal_connect(wrap_item, "toggled", G_CALLBACK(on_toggle_wrap), this);
    if (gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(text_view)) != GTK_WRAP_NONE) {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(wrap_item), TRUE);
    }
    gtk_menu_shell_append(GTK_MENU_SHELL(view_popup), wrap_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view_menu);

    /* Help menu */
    GtkWidget *help_menu = gtk_menu_item_new_with_label("Help");
    GtkMenu *help_popup = GTK_MENU(gtk_menu_new());
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_menu), GTK_WIDGET(help_popup));

    GtkWidget *about_item = gtk_menu_item_new_with_label("About");
    g_signal_connect(about_item, "activate", G_CALLBACK(on_about), this);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_popup), about_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_menu);

    setup_ui();
    connect_signals();
    update_title();
}

void TextEditor::update_title() {
    std::string title = "AndrewEdit";
    if (!current_filename.empty()) {
        title += " - ";
        title += current_filename;
    }
    gtk_window_set_title(GTK_WINDOW(window), title.c_str());
}

void TextEditor::run() {
    gtk_widget_show_all(window);
    gtk_main();
}

void TextEditor::setup_ui() {
    // Placeholder for UI setup if needed
}

void TextEditor::connect_signals() {
    // Clipboard signals not required; copy/paste handled via menu actions
}

void TextEditor::on_open(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                   GTK_WINDOW(editor->window),
                                                   GTK_FILE_CHOOSER_ACTION_OPEN,
                                                   "_Cancel",
                                                   GTK_RESPONSE_CANCEL,
                                                   "_Open",
                                                   GTK_RESPONSE_ACCEPT,
                                                   NULL);
    int response;
    while (true) {
        response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response != GTK_RESPONSE_ACCEPT) break;

        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gchar *content = NULL;
        gsize length = 0;
        GError *error = NULL;

        if (g_file_get_contents(filename, &content, &length, &error)) {
            gboolean valid_utf8 = g_utf8_validate(content, length, NULL);

            if (!valid_utf8) {
                const guint8 *bytes = (const guint8*)content;
                const char *detected = "Unknown or binary";

                if (length >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) {
                    detected = "UTF-8 with BOM";
                } else if (length >= 2 && bytes[0] == 0xFF && bytes[1] == 0xFE) {
                    detected = "UTF-16 LE (BOM)";
                } else if (length >= 2 && bytes[0] == 0xFE && bytes[1] == 0xFF) {
                    detected = "UTF-16 BE (BOM)";
                } else if (length >= 4 && bytes[0] == 0x00 && bytes[1] == 0x00 && bytes[2] == 0xFE && bytes[3] == 0xFF) {
                    detected = "UTF-32 BE (BOM)";
                } else if (length >= 4 && bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
                    detected = "UTF-32 LE (BOM)";
                } else {
                    int zeros_even = 0, zeros_odd = 0;
                    for (gsize i = 0; i + 1 < length; ++i) {
                        if (bytes[i] == 0) {
                            if ((i % 2) == 0) zeros_even++; else zeros_odd++;
                        }
                    }
                    if (zeros_even > (int)(length / 10)) detected = "Probable UTF-16 BE";
                    else if (zeros_odd > (int)(length / 10)) detected = "Probable UTF-16 LE";
                    else detected = "Unknown (possibly binary or legacy single-byte encoding)";
                }

                gchar *msg = g_strdup_printf("File is not supported text (not UTF-8/ASCII).\nDetected format: %s", detected);
                GtkWidget *err = gtk_message_dialog_new(GTK_WINDOW(editor->window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", msg);
                gtk_dialog_run(GTK_DIALOG(err));
                gtk_widget_destroy(err);
                g_free(msg);

                g_free(content);
                g_free(filename);
                if (error) g_error_free(error);

                /* keep the file chooser open so the user can pick another file */
                continue;
            }

            /* valid UTF-8 (includes ASCII) */
            gtk_text_buffer_set_text(editor->buffer, content, (gint)length);
            g_free(content);
            editor->current_filename = filename;
            editor->update_title();
            g_free(filename);
            break;
        } else {
            g_printerr("Error opening file: %s\n", error ? error->message : "unknown");
            if (error) g_error_free(error);
            g_free(filename);
            break;
        }
    }

    gtk_widget_destroy(dialog);
}

void TextEditor::on_save(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
                                                   GTK_WINDOW(editor->window),
                                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                                   "_Cancel",
                                                   GTK_RESPONSE_CANCEL,
                                                   "_Save",
                                                   GTK_RESPONSE_ACCEPT,
                                                   NULL);
    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GtkTextBuffer *buffer = editor->buffer;
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        gchar *content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        GError *error = NULL;
        if (!g_file_set_contents(filename, content, -1, &error)) {
            g_printerr("Error saving file: %s\n", error ? error->message : "unknown");
            if (error) g_error_free(error);
        } else {
            editor->current_filename = filename;
            editor->update_title();
        }

        g_free(content);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void TextEditor::on_save_as(GtkMenuItem *menuitem, gpointer user_data) {
    // Same as on_save for this simple editor
    on_save(menuitem, user_data);
}

void TextEditor::on_exit(GtkMenuItem *menuitem, gpointer user_data) {
    gtk_main_quit();
}

void TextEditor::on_copy(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->text_view)));
    GtkTextIter start, end;
    if (gtk_text_buffer_get_selection_bounds(buffer, &start, &end)) {
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), text, -1);
        g_free(text);
    }
}

void TextEditor::on_paste(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->text_view)));
    gchar *text = (gchar*)gtk_clipboard_wait_for_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
    if (text) {
        GtkTextIter insert_pos;
        gtk_text_buffer_get_iter_at_mark(buffer, &insert_pos, gtk_text_buffer_get_insert(buffer));
        gtk_text_buffer_insert(buffer, &insert_pos, text, -1);
        g_free(text);
    }
}

void TextEditor::on_search(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Search",
                                                   GTK_WINDOW(((TextEditor*)user_data)->window),
                                                   GTK_DIALOG_MODAL,
                                                   "_Cancel",
                                                   GTK_RESPONSE_CANCEL,
                                                   "Find Next",
                                                   GTK_RESPONSE_ACCEPT,
                                                   NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(dialog), vbox);

    GtkWidget *label = gtk_label_new("Search for:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
        GtkTextBuffer *buffer = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(((TextEditor*)user_data)->text_view)));
        GtkTextIter iter;
        gtk_text_buffer_get_start_iter(buffer, &iter);
        GtkTextIter match_start, match_end;

        if (gtk_text_iter_forward_search(&iter, text, GTK_TEXT_SEARCH_TEXT_ONLY, &match_start, &match_end, NULL)) {
            gtk_text_buffer_select_range(buffer, &match_start, &match_end);
            gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(((TextEditor*)user_data)->text_view), &match_start, 0.0, FALSE, 0.0, 0.0);
            gtk_widget_grab_focus(((TextEditor*)user_data)->text_view);
        }
    }

    gtk_widget_destroy(dialog);
}

void TextEditor::on_replace(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->text_view)));
    GtkTextIter start, end;

    if (gtk_text_buffer_get_selection_bounds(buffer, &start, &end)) {
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Replace",
                                                       GTK_WINDOW(editor->window),
                                                       GTK_DIALOG_MODAL,
                                                       "_Cancel",
                                                       GTK_RESPONSE_CANCEL,
                                                       "Replace",
                                                       GTK_RESPONSE_ACCEPT,
                                                       NULL);
        GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *entry = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(content), entry, TRUE, TRUE, 0);
        gtk_widget_show_all(dialog);

        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response == GTK_RESPONSE_ACCEPT) {
            const gchar *new_text = gtk_entry_get_text(GTK_ENTRY(entry));
            gint start_offset = gtk_text_iter_get_offset(&start);
            gtk_text_buffer_delete(buffer, &start, &end);
            GtkTextIter insert_iter;
            gtk_text_buffer_get_iter_at_offset(buffer, &insert_iter, start_offset);
            gtk_text_buffer_insert(buffer, &insert_iter, new_text, -1);
        }

        gtk_widget_destroy(dialog);
    }
}

void TextEditor::on_about(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(editor->window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "AndrewEdit\nBuilt with GTK+ 3");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void TextEditor::on_font_properties(GtkMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    GtkWidget *dialog = gtk_font_chooser_dialog_new("Select Font", GTK_WINDOW(editor->window));
    int res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_OK) {
        char *fontname = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(dialog));
        if (fontname) {
            GtkTextBuffer *buffer = editor->buffer;
            GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
            GtkTextTag *tag = gtk_text_tag_table_lookup(table, "user_font");

            if (tag) {
                g_object_set(tag, "font", fontname, NULL);
            } else {
                tag = gtk_text_buffer_create_tag(buffer, "user_font", "font", fontname, NULL);
            }

            GtkTextIter start, end;
            gtk_text_buffer_get_bounds(buffer, &start, &end);
            gtk_text_buffer_apply_tag_by_name(buffer, "user_font", &start, &end);
            g_free(fontname);
        }
    }

    gtk_widget_destroy(dialog);
}

void TextEditor::on_toggle_wrap(GtkCheckMenuItem *menuitem, gpointer user_data) {
    TextEditor *editor = (TextEditor*)user_data;
    gboolean active = gtk_check_menu_item_get_active(menuitem);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(editor->text_view), active ? GTK_WRAP_WORD : GTK_WRAP_NONE);
}
