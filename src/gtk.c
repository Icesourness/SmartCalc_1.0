#include "GUI_new.h"

int main(int argc, char** argv){
  gtk_init(&argc, &argv);
  GtkBuilder* builder = gtk_builder_new_from_file("./build/window.glade");
  GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "core_window"));
  GtkWidget* core_entry = GTK_WIDGET(gtk_builder_get_object(builder, "core_entry"));
  GtkWidget* x_min = GTK_WIDGET(gtk_builder_get_object(builder, "x_min"));
  GtkWidget* x_max = GTK_WIDGET(gtk_builder_get_object(builder, "x_max"));
  GtkWidget* y_min = GTK_WIDGET(gtk_builder_get_object(builder, "y_min"));
  GtkWidget* y_max = GTK_WIDGET(gtk_builder_get_object(builder, "y_max"));
  GtkWidget* button_plot = GTK_WIDGET(gtk_builder_get_object(builder, "button_plot"));
  GtkWidget* button_credit = GTK_WIDGET(gtk_builder_get_object(builder, "button_credit"));
  GtkWidget* button_deposit = GTK_WIDGET(gtk_builder_get_object(builder, "button_deposit"));

  struct plot* info_for_plot = g_malloc(sizeof(*info_for_plot));
  info_for_plot->window = window;
  info_for_plot->entry = core_entry;
  info_for_plot->x_max = x_max;
  info_for_plot->x_min = x_min;
  info_for_plot->y_max = y_max;
  info_for_plot->y_min = y_min;
  struct credit* info_for_credit = g_malloc(sizeof(*info_for_credit));
  info_for_credit->window = window;
  struct deposit* info_for_deposit = g_malloc(sizeof(*info_for_deposit));
  info_for_deposit->window = window;

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(button_plot), "clicked", G_CALLBACK(click_plot), info_for_plot);
  g_signal_connect(G_OBJECT(button_credit), "clicked", G_CALLBACK(click_credit), info_for_credit);
  g_signal_connect(G_OBJECT(button_deposit), "clicked", G_CALLBACK(click_deposit), info_for_deposit);
  
  gtk_builder_connect_signals(builder, NULL);
  gtk_widget_show_all(window);
  gtk_main();
  g_free(info_for_plot);
  g_free(info_for_credit);
  g_free(info_for_deposit);
  g_object_unref(builder);
  return 0;
}