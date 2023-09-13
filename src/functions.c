#include "GUI_new.h"

void click_deposit(GtkButton* button, struct deposit* data){
  (void)button;
  gtk_widget_set_sensitive(GTK_WIDGET(data->window), FALSE);
  GtkBuilder* builder = gtk_builder_new_from_file("./build/deposit.glade");
  GtkWidget* window_for_deposit = GTK_WIDGET(gtk_builder_get_object(builder, "deposit"));
  GtkWidget* button_calc = GTK_WIDGET(gtk_builder_get_object(builder, "button_calc"));
  deposit_data_extract(builder, data);

  g_signal_connect(G_OBJECT(window_for_deposit), "destroy", G_CALLBACK(window_close), data->window);
  g_signal_connect(G_OBJECT(button_calc), "clicked", G_CALLBACK(setup_deposit), data);
  gtk_widget_show_all(window_for_deposit);
}

int validate_deposit(const char** pool, char* text_payout_status, char* text_replesh_status, char* text_withdraw_status, int* payout_status, 
  int* replesh_status, int* withdraw_status){
    int result = SUCCESS;
 for (int i = 0; i < 6; i++){
  if (!validate_numbers(pool[i]))
    result = FAIL;
 }
 if (!check_empty(pool, 4)) result = FAIL;
 if (text_payout_status[4] == 'y') *payout_status = PAYOUT_YEAR;
  if (text_replesh_status[4] == 'm') *replesh_status = REPLESH_MONTH;
  else if (text_replesh_status[4] == '6') *replesh_status = REPLESH_6_MONTHS;
  if (text_withdraw_status[4] == 'm') *withdraw_status = WITHDRAW_MONTH;
  else if (text_withdraw_status[4] == '6') *withdraw_status = WITHDRAW_6_MONTHS;

  if(((*replesh_status == REPLESH_MONTH || *replesh_status == REPLESH_6_MONTHS) && *(pool[4]) == '\0') ||
     ((*withdraw_status == WITHDRAW_MONTH || *withdraw_status == WITHDRAW_6_MONTHS) && *(pool[5]) == '\0')) {
    result = FAIL;
  }
  return result;
}

void calc_deposit(struct deposit* data, const char** pool, int payout_status, int replesh_status, int withdraw_status, int capital_status){
  setlocale(LC_ALL, "C");
  char text_surplus[256], text_tax_waste[256], text_final_amount[256];
  double amount = atof(pool[0]);
  int period = atoi(pool[1]);
  double percent = (atof(pool[2]) / (100.0 * 12));
  double tax = (atof(pool[3]) / 100.0);
  double surplus_per_month = 0.0, surplus_summary = 0.0, tax_waste = 0.0;
  double replesh_amount = atof(pool[4]);
  double withdraw_amount = atof(pool[5]);
  for(int i = 0; i < period; i++){
    if (replesh_status == REPLESH_MONTH || ((replesh_status == REPLESH_6_MONTHS) && (i % 6 == 0)))  //  6 or 5?
      amount += replesh_amount;  
    if (withdraw_status == WITHDRAW_MONTH || ((withdraw_status == WITHDRAW_6_MONTHS) && (i % 6 == 0)))
      amount -= withdraw_amount;  
    surplus_per_month = (amount * percent);
    surplus_summary += surplus_per_month;
    if (capital_status == 1){
      if (payout_status == PAYOUT_MONTH) 
        amount += surplus_per_month;
      else if (payout_status == PAYOUT_YEAR && (i != 0) && (i % 11 == 0)) 
        amount += surplus_summary;
      surplus_per_month = 0;
    }
  }
  if (surplus_summary > TAX_FREE){
    tax_waste += (surplus_summary - TAX_FREE) * tax;
    surplus_summary -= tax_waste;
  }
  g_snprintf(text_surplus, sizeof(text_surplus), "%.7lf", surplus_summary); 
  g_snprintf(text_tax_waste, sizeof(text_tax_waste), "%.7lf", tax_waste);
  g_snprintf(text_final_amount, sizeof(text_final_amount), "%.7lf", amount);
  gtk_entry_set_text(GTK_ENTRY(data->surplus), text_surplus);
  gtk_entry_set_text(GTK_ENTRY(data->tax_waste), text_tax_waste);
  gtk_entry_set_text(GTK_ENTRY(data->final_amount), text_final_amount);
}

void setup_deposit(GtkButton* button, struct deposit* data){
  (void)button;
  const char* text_amount = gtk_entry_get_text(GTK_ENTRY(data->amount));
  const char* text_period = gtk_entry_get_text(GTK_ENTRY(data->period));
  const char* text_percent = gtk_entry_get_text(GTK_ENTRY(data->percent));
  const char* text_tax = gtk_entry_get_text(GTK_ENTRY(data->tax));
  const char* text_replesh_amount = gtk_entry_get_text(GTK_ENTRY(data->replesh_amount));
  const char* text_withdraw_amount = gtk_entry_get_text(GTK_ENTRY(data->withdraw_amount));
  const char* entry_pool[6] = {text_amount, text_period, text_percent, text_tax, text_replesh_amount, text_withdraw_amount};
  char* text_payout_status = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->payout_status));
  char* text_replesh_status = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->replesh_status));
  char* text_withdraw_status = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->withdraw_status));
  int capital_status = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->capitalization));
  int payout_status = PAYOUT_MONTH;
  int replesh_status = REPLESH_NONE;
  int withdraw_status = WITHDRAW_NONE;
  int status = validate_deposit(entry_pool, text_payout_status, text_replesh_status, text_withdraw_status, &payout_status, &replesh_status, &withdraw_status);
  if (status == SUCCESS)
    calc_deposit(data, entry_pool, payout_status, replesh_status, withdraw_status, capital_status);

  g_free(text_payout_status);
  g_free(text_replesh_status);
  g_free(text_withdraw_status);
}

void click_credit(GtkButton* button, struct credit* data){
  (void)button;
  gtk_widget_set_sensitive(GTK_WIDGET(data->window), FALSE);
  GtkBuilder* builder = gtk_builder_new_from_file("./build/credit.glade");
  GtkWidget* window_for_credit = GTK_WIDGET(gtk_builder_get_object(builder, "credit"));
  GtkWidget* button_calc = GTK_WIDGET(gtk_builder_get_object(builder, "button_calc"));
  credit_data_extract(builder, data);

  g_signal_connect(G_OBJECT(window_for_credit), "destroy", G_CALLBACK(window_close), data->window);
  g_signal_connect(G_OBJECT(button_calc), "clicked", G_CALLBACK(setup_credit), data);
  gtk_widget_show_all(window_for_credit);
}

void setup_credit(GtkButton* button, struct credit* data){
  (void)button;
  char text_monthly_payment[256], text_overpayment[256], text_total_contribution[256];
  double monthly_payment, overpayment;
  double total_contribution = 0.0;
  const char* text_amount = gtk_entry_get_text(GTK_ENTRY(data->amount));
  const char* text_period = gtk_entry_get_text(GTK_ENTRY(data->period));
  const char* text_percent = gtk_entry_get_text(GTK_ENTRY(data->percent));
  const char* check_pool[3] = {text_amount, text_period, text_percent};
  if (!check_empty(check_pool, 3) || !validate_numbers(text_amount) || 
      !validate_numbers(text_period) || !validate_numbers(text_percent)){
    return;
  }
  setlocale(LC_ALL, "C");
  double amount = atof(text_amount);
  int period = atoi(text_period);
  double percent = (atof(text_percent) / 100.0);
  calc_credit(data, amount, period, percent, &monthly_payment, &overpayment, &total_contribution);
  g_snprintf(text_monthly_payment, sizeof(text_monthly_payment), "%.7lf", monthly_payment);
  g_snprintf(text_overpayment, sizeof(text_overpayment), "%.7lf", overpayment);
  g_snprintf(text_total_contribution, sizeof(text_total_contribution), "%.7lf", total_contribution);
  gtk_entry_set_text(GTK_ENTRY(data->monthly_payment), text_monthly_payment);
  gtk_entry_set_text(GTK_ENTRY(data->overpayment), text_overpayment);
  gtk_entry_set_text(GTK_ENTRY(data->total_contribution), text_total_contribution);
}

void calc_credit(struct credit* data, double amount, int period, double percent, double* monthly_payment, double* overpayment, double* total_contribution){
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->radio_1))){
      percent /= 12;
      double koef = percent * pow((1.0 + percent), period) / (pow((1.0 + percent), period) - 1.0);
      *monthly_payment = koef * amount;
      *total_contribution = *monthly_payment * period;
      *overpayment = *total_contribution - amount;
    }
  else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->radio_2))){
    double P = 0.0;
    double Sp = 0.0;
    double amount_copy = amount;
    *monthly_payment = amount_copy / period;
    while(amount_copy > 0){
      Sp = (amount_copy * percent * AVERAGE_DAYS_IN_MONTH) / 365;
      amount_copy -= *monthly_payment;
      P = *monthly_payment + Sp;
      *total_contribution += P;
    }
    *overpayment = *total_contribution - amount;
  }
}

void click_plot(GtkButton* button, struct plot* data){
  (void)button;
  gtk_widget_set_sensitive(GTK_WIDGET(data->window), FALSE);
  GtkBuilder* builder = gtk_builder_new_from_file("./build/plot.glade");
  GtkWidget* window_for_plot = GTK_WIDGET(gtk_builder_get_object(builder, "plot"));
  GtkWidget* darea = GTK_WIDGET(gtk_builder_get_object(builder, "darea"));
  g_signal_connect(G_OBJECT(window_for_plot), "destroy", G_CALLBACK(window_close), data->window);
  g_signal_connect(GTK_DRAWING_AREA(darea), "draw", G_CALLBACK(draw_plot), data);
  gtk_widget_show_all(window_for_plot);
}

void draw_plot(GtkWidget *darea, cairo_t *cr, struct plot* data){
  (void)darea;
  int status = SUCCESS;
  const char* text_entry = gtk_entry_get_text(GTK_ENTRY(data->entry));
  const char* text_x_max = gtk_entry_get_text(GTK_ENTRY(data->x_max));
  const char* text_x_min = gtk_entry_get_text(GTK_ENTRY(data->x_min));
  const char* text_y_max = gtk_entry_get_text(GTK_ENTRY(data->y_max));
  const char* text_y_min = gtk_entry_get_text(GTK_ENTRY(data->y_min));
  const char* check_pool[5] = {text_entry, text_x_max, text_x_min, text_y_max, text_y_min};
  if (!check_empty(check_pool, 5))
    status = FAIL;
  else if (!validate_expression(text_entry) || !validate_numbers(text_x_max) || !validate_numbers(text_x_min) || !validate_numbers(text_y_max) || !validate_numbers(text_y_min)){
    gtk_entry_set_text(GTK_ENTRY(data->entry), "Incorrect input");
    status = FAIL;
  }
  if (status == SUCCESS){
    draw_axis(cr);
    draw_function(cr, data);   
  } 
}

void draw_function(cairo_t *cr, struct plot* data){
  const char *text = gtk_entry_get_text(GTK_ENTRY(data->entry));
  double x_max, x_min, y_max, y_min;
  scope_and_range(cr, data, &x_max, &x_min, &y_max, &y_min);
  if (x_max == x_min || y_max == y_min) return;
  double scale_x = 1000 / fabsl(x_max - x_min);
  double scale_y = 1000 / fabsl(y_max - y_min);
  int dots = 200000;
  double step = fabsl(x_max - x_min) / dots;
  double* x = malloc(sizeof(double) * dots);
  double* y = malloc(sizeof(double) * dots);
  double* p_x = x;
  double* p_y = y;
  double value = x_min;
  for (int i = 0; i < dots; i++, p_x++, value += step)   
    *p_x = value; 
  s21_graph(text, dots, x, y); 
  p_x = x;
  while(!(*p_y >= y_min) || !(*p_y <= y_max)){
    p_x++;
    p_y++;
    dots--;
  }
  for(int i = 0; (i < dots) && (*p_x <= x_max); i++, p_x++, p_y++){
    if (*p_y >= y_min && *p_y <= y_max && *p_x >= x_min && *p_x <= x_max){
      cairo_move_to(cr, (*p_x + fabsl(x_min)) * scale_x + 25.0, -1 * (*p_y - fabsl(y_max)) * scale_y + 25.0);
      cairo_close_path(cr);
    }
  }
  cairo_stroke(cr);
  free(x);
  free(y);
}

void scope_and_range(cairo_t *cr, struct plot* data, double* x_max, double* x_min, double* y_max, double* y_min){
  const char* text_x_max = gtk_entry_get_text(GTK_ENTRY(data->x_max));
  const char* text_x_min = gtk_entry_get_text(GTK_ENTRY(data->x_min));
  const char* text_y_max = gtk_entry_get_text(GTK_ENTRY(data->y_max));
  const char* text_y_min = gtk_entry_get_text(GTK_ENTRY(data->y_min));
  const char* text[4] = {text_x_max, text_x_min, text_y_max, text_y_min};
  double* xy[4] = {x_max, x_min, y_max, y_min};
  setlocale(LC_ALL, "C");
  for (int i = 0; i < 4; i++)
    *(double*)(xy[i]) = atof(text[i]);
  swap_max_min(x_max, x_min, y_max, y_min);
  setup_values(cr, *x_max, *x_min, *y_max, *y_min); 
}

void swap_max_min(double* x_max, double* x_min, double* y_max, double* y_min){
  double tmp;
  if (*x_max < *x_min){
  tmp = *x_min;
  *x_min = *x_max;
  *x_max = tmp;
  }
  if (*y_max < *y_min){
    tmp = *y_min;
    *y_min = *y_max;
    *y_max = tmp;
  }
}

void setup_values(cairo_t* cr, double x_max, double x_min, double y_max, double y_min){
  double step_x = (x_max - x_min) / 4;
  double step_y = (y_max - y_min) / 4;
  double values_x[5]; 
  double values_y[5]; 
  char txt_values_x[4][256];
  char txt_values_y[4][256];
  double value_x = x_min;
  double value_y = y_min;
  for (int i = 0; i < 5; i++, value_x += step_x, value_y += step_y){
    values_x[i] = value_x; 
    values_y[i] = value_y; 
    sprintf(txt_values_x[i], "%.2lf", values_x[i]);
    sprintf(txt_values_y[i], "%.2lf", values_y[i]);
  }
  draw_values(cr, txt_values_x, txt_values_y);
}

void draw_values(cairo_t* cr, char (*txt_values_x)[256], char (*txt_values_y)[256]){
  cairo_move_to(cr, 25, 550);
  cairo_show_text(cr, txt_values_x[0]);
  cairo_move_to(cr, 275, 550);
  cairo_show_text(cr, txt_values_x[1]);
  cairo_move_to(cr, 525, 550);
  cairo_show_text(cr, txt_values_x[2]);
  cairo_move_to(cr, 775, 550);
  cairo_show_text(cr, txt_values_x[3]);
  cairo_move_to(cr, 975, 550);
  cairo_show_text(cr, txt_values_x[4]);

  cairo_move_to(cr, 535, 35);
  cairo_show_text(cr, txt_values_y[0]);
  cairo_move_to(cr, 535, 282);
  cairo_show_text(cr, txt_values_x[1]);
  cairo_move_to(cr, 535, 520);
  cairo_show_text(cr, txt_values_x[2]);
  cairo_move_to(cr, 535, 782);
  cairo_show_text(cr, txt_values_x[3]);
  cairo_move_to(cr, 535, 1030);
  cairo_show_text(cr, txt_values_x[4]);
}

void draw_axis(cairo_t *cr){
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_line_width(cr, 5);  
  cairo_set_source_rgb(cr, 1.0, 1.0, 001.0);
  cairo_move_to(cr, 25, 525);
  cairo_line_to(cr, 1025, 525);
  cairo_move_to(cr, 525, 25);
  cairo_line_to(cr, 525, 1025);

  cairo_move_to(cr, 25, 520);
  cairo_line_to(cr, 25, 530);
  cairo_move_to(cr, 275, 520);
  cairo_line_to(cr, 275, 530);
  cairo_move_to(cr, 775, 520);
  cairo_line_to(cr, 775, 530);
  cairo_move_to(cr, 1025, 520);
  cairo_line_to(cr, 1025, 530);

  cairo_move_to(cr, 520, 25);
  cairo_line_to(cr, 530, 25);
  cairo_move_to(cr, 520, 275);
  cairo_line_to(cr, 530, 275);
  cairo_move_to(cr, 520, 775);
  cairo_line_to(cr, 530, 775);
  cairo_move_to(cr, 520, 1025);
  cairo_line_to(cr, 530, 1025);

  cairo_stroke(cr);
  cairo_set_font_size(cr, 24);
  cairo_move_to(cr, 975, 480);
  cairo_show_text(cr, "x");
  cairo_move_to(cr, 480, 25);
  cairo_show_text(cr, "y");
  cairo_set_line_width(cr, 10);  
}

void click_0(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "0", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_1(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "1", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_2(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "2", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_3(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "3", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_4(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "4", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_5(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "5", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_6(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "6", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

void click_7(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "7", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_8(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "8", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_9(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "9", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_lb(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "(", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_rb(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), ")", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_plus(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "+", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_minus(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "-", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_multi(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "*", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_div(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "/", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_coma(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), ".", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_mod(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "mod", 3, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 3);
}

 void click_pow(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "^", 1, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 1);
}

 void click_sqrt(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "sqrt()", 6, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 5);
}

 void click_sin(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "sin()", 5, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 4);
}

 void click_cos(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "cos()", 5, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 4);
}

 void click_tan(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "tan()", 5, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 4);
}

 void click_asin(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "asin()", 6, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 5);
}

 void click_acos(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "acos()", 6, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 5);
}

 void click_atan(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "acos()", 6, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 5);
}

 void click_ln(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "ln()", 4, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 3);
}

 void click_log10(GtkButton* button, gpointer data) {
  (void)button;
  gint cursor_position = gtk_editable_get_position(GTK_EDITABLE(data));
  gtk_editable_insert_text(GTK_EDITABLE(data), "log()", 5, &cursor_position);
  gtk_editable_set_position(GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) + 4);
}

void click_eq(GtkButton *button, gpointer data){
  (void)button;
  setlocale(LC_ALL, "C");
	const char *text = gtk_entry_get_text(GTK_ENTRY(data));
  char result[256];
  if (!validate_expression(text) || strchr(text, 'x') || strchr(text, 'X')){
	  gtk_entry_set_text(GTK_ENTRY(data), "Incorrect input");
    gtk_editable_set_position(GTK_EDITABLE(data),
            gtk_editable_get_position(GTK_EDITABLE(data)) + 15);
  }
  else if (*text == '\0'){
    g_snprintf(result, sizeof(result), "%.7lf", 0.0);
    gtk_entry_set_text(GTK_ENTRY(data), result);
    gtk_editable_set_position(GTK_EDITABLE(data),
            gtk_editable_get_position(GTK_EDITABLE(data)) + strlen(result));
  }
  else {
    g_snprintf(result, sizeof(result), "%.7lf", (s21_smartcalc(text)));
    gtk_entry_set_text(GTK_ENTRY(data), result);
    gtk_editable_set_position(GTK_EDITABLE(data),
            gtk_editable_get_position(GTK_EDITABLE(data)) + strlen(result));
  }
}

void click_del(GtkButton *button, gpointer data) {
  (void)button;
  gtk_editable_delete_text(
      GTK_EDITABLE(data), gtk_editable_get_position(GTK_EDITABLE(data)) - 1, gtk_editable_get_position(GTK_EDITABLE(data)));
}

void click_clr(GtkButton *button, gpointer data) {
  (void)button;
  gtk_entry_set_text(GTK_ENTRY(data), "");
}

void window_close(GtkWidget* widget, GtkWidget* data) {
  (void)widget;
  gtk_widget_set_sensitive(GTK_WIDGET(data), TRUE);  
}

int check_empty(const char** pull, size_t size){
  int result = SUCCESS;
  for(size_t i = 0; i < size; i++)
    if (*(pull[i]) == '\0')
      result = FAIL;  
  return result;
}

void credit_data_extract(GtkBuilder* builder, struct credit* data){
  data->amount = GTK_WIDGET(gtk_builder_get_object(builder, "amount"));
  data->period = GTK_WIDGET(gtk_builder_get_object(builder, "period"));
  data->percent = GTK_WIDGET(gtk_builder_get_object(builder, "percent"));
  data->monthly_payment = GTK_WIDGET(gtk_builder_get_object(builder, "monthly payment"));
  data->overpayment = GTK_WIDGET(gtk_builder_get_object(builder, "overpayment"));
  data->total_contribution = GTK_WIDGET(gtk_builder_get_object(builder, "total_contribution"));
  data->radio_1 = GTK_WIDGET(gtk_builder_get_object(builder, "annuity"));
  data->radio_2 = GTK_WIDGET(gtk_builder_get_object(builder, "differentiated"));
}

void deposit_data_extract(GtkBuilder* builder, struct deposit* data){
  data->amount = GTK_WIDGET(gtk_builder_get_object(builder, "amount"));
  data->period = GTK_WIDGET(gtk_builder_get_object(builder, "period"));
  data->percent = GTK_WIDGET(gtk_builder_get_object(builder, "percent"));
  data->tax = GTK_WIDGET(gtk_builder_get_object(builder, "tax"));
  data->payout_status = GTK_WIDGET(gtk_builder_get_object(builder, "payout"));
  data->capitalization = GTK_WIDGET(gtk_builder_get_object(builder, "capitalization"));
  data->replesh_status = GTK_WIDGET(gtk_builder_get_object(builder, "replesh_status"));
  data->withdraw_status = GTK_WIDGET(gtk_builder_get_object(builder, "withdraw_status"));
  data->replesh_amount = GTK_WIDGET(gtk_builder_get_object(builder, "replesh_amount"));
  data->withdraw_amount = GTK_WIDGET(gtk_builder_get_object(builder, "withdraw_amount"));
  data->surplus = GTK_WIDGET(gtk_builder_get_object(builder, "surplus"));
  data->tax_waste = GTK_WIDGET(gtk_builder_get_object(builder, "tax_waste"));
  data->final_amount = GTK_WIDGET(gtk_builder_get_object(builder, "final_amount"));
}