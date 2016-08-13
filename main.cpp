/* 
 * File:   main.cpp
 * Author: renato
 *
 * Created on 12 de Agosto de 2016, 19:04
 */

#include <cstdlib>
#include <gtk/gtk.h>

//static void
//print_hello(GtkWidget *widget, gpointer data) {
//    g_print("Hello World\n");
//}
//
//static void
//activate(GtkApplication *app, gpointer user_data) {
//    GtkWidget *window;
//    GtkWidget *button;
//    GtkWidget *button_box;
//
//    window = gtk_application_window_new(app);
//    gtk_window_set_title(GTK_WINDOW(window), "Window");
//    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
//
//    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
//    gtk_container_add(GTK_CONTAINER(window), button_box);
//
//    button = gtk_button_new_with_label("Hello World");
//    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
//    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
//    gtk_container_add(GTK_CONTAINER(button_box), button);
//
//    gtk_widget_show_all(window);
//}
//
//int main(int argc, char **argv) {
//    GtkApplication *app;
//    int status;
//
//    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
//    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//    status = g_application_run(G_APPLICATION(app), argc, argv);
//    g_object_unref(app);
//
//    return status;
//}

void quit(GtkWidget *w, gpointer p) {
    gtk_main_quit();
}

void click(GtkWidget *w, gpointer p) {
    g_print("O botao foi clicado\n");
}

int main(int argc, char **argv) {

    GtkWidget *window;
    GtkWidget *button;
    
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hello world");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);

    button = gtk_button_new_with_label("click here");
    gtk_container_add(GTK_CONTAINER(window), button);
    g_signal_connect(button, "clicked", G_CALLBACK (click), (gpointer )window);

//    gtk_widget_show(button);
//    gtk_widget_show(window);
    gtk_widget_show_all(window);
    
    gtk_main();
    return 0;
}