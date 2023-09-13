#include <gtk/gtk.h>
#include "s21_smartcalc.h"

struct plot{
  GtkWidget* window;
  GtkWidget* entry;
  GtkWidget* x_max;
  GtkWidget* x_min;
  GtkWidget* y_max;
  GtkWidget* y_min;
};

struct credit{
  GtkWidget* window;
  GtkWidget* amount;
  GtkWidget* period;
  GtkWidget* percent;
  GtkWidget* monthly_payment;
  GtkWidget* overpayment;
  GtkWidget* total_contribution;
  GtkWidget* radio_1;
  GtkWidget* radio_2;
};

struct deposit{
  GtkWidget* window;
  GtkWidget* amount;
  GtkWidget* period;
  GtkWidget* percent;
  GtkWidget* tax;
  GtkWidget* payout_status;
  GtkWidget* capitalization;
  GtkWidget* replesh_status;
  GtkWidget* withdraw_status;
  GtkWidget* replesh_amount;
  GtkWidget* withdraw_amount;
  GtkWidget* surplus;
  GtkWidget* tax_waste;
  GtkWidget* final_amount;
};

void click_0(GtkButton* button, gpointer data);
void click_1(GtkButton* button, gpointer data);
void click_2(GtkButton* button, gpointer data);
void click_3(GtkButton* button, gpointer data);
void click_4(GtkButton* button, gpointer data);
void click_5(GtkButton* button, gpointer data);
void click_6(GtkButton* button, gpointer data);
void click_7(GtkButton* button, gpointer data);
void click_8(GtkButton* button, gpointer data);
void click_9(GtkButton* button, gpointer data);
void click_lb(GtkButton* button, gpointer data);
void click_rb(GtkButton* button, gpointer data);
void click_plus(GtkButton* button, gpointer data);
void click_minus(GtkButton* button, gpointer data);
void click_multi(GtkButton* button, gpointer data);
void click_div(GtkButton* button, gpointer data);
void click_coma(GtkButton* button, gpointer data);
void click_mod(GtkButton* button, gpointer data);
void click_pow(GtkButton* button, gpointer data);
void click_sqrt(GtkButton* button, gpointer data);
void click_sin(GtkButton* button, gpointer data);
void click_cos(GtkButton* button, gpointer data);
void click_tan(GtkButton* button, gpointer data);
void click_asin(GtkButton* button, gpointer data);
void click_acos(GtkButton* button, gpointer data);
void click_atan(GtkButton* button, gpointer data);
void click_eq(GtkButton* button, gpointer data);
void click_del(GtkButton *button, gpointer data);
void click_ln(GtkButton* button, gpointer data);
void click_log10(GtkButton* button, gpointer data);
void click_plot(GtkButton* button, struct plot* data);
void click_credit(GtkButton* button, struct credit* data);
void click_deposit(GtkButton* button, struct deposit* data);
void draw_plot(GtkWidget *draw_area, cairo_t *cr, struct plot* data);
void draw_axis(cairo_t *cr);
void draw_function(cairo_t *cr, struct plot* data);
void scope_and_range(cairo_t *cr, struct plot* data, double* x_max, double* x_min, double* y_max, double* y_min);
void window_close(GtkWidget* widget, GtkWidget* data);
void setup_credit(GtkButton* button, struct credit* data);
void setup_deposit(GtkButton* button, struct deposit* data);
int check_empty(const char** pool, size_t size);
void credit_data_extract(GtkBuilder* builder, struct credit* data);
void deposit_data_extract(GtkBuilder* builder, struct deposit* data);
int check_tan(const char* text);
void calc_credit(struct credit*, double amount, int period, double percent, double* monthly_payment, double* overpayment, double* total_contribution);
void setup_values(cairo_t* cr, double x_max, double x_min, double y_max, double y_min);
void draw_values(cairo_t* cr, char (*txt_values_x)[256], char (*txt_values_y)[256]);
void swap_max_min(double* x_max, double* x_min, double* y_max, double* y_min);
int validate_deposit(const char** pool, char* text_payout_status, char* text_replesh_status, char* text_withdraw_status, int* payout_status, 
int* replesh_status, int* withdraw_status);
void calc_deposit(struct deposit* data, const char** pool, int payout_status, int replesh_status, int withdraw_status, int capital_status);