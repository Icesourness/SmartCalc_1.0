#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define SUCCESS 1
#define FAIL 0
#define AVERAGE_DAYS_IN_MONTH 30.42
#define TAX_FREE 1000000 * 0.12

#define PAYOUT_MONTH 0
#define PAYOUT_YEAR 1

#define REPLESH_NONE 0
#define REPLESH_MONTH 1
#define REPLESH_6_MONTHS 2

#define WITHDRAW_NONE 0
#define WITHDRAW_MONTH 1
#define WITHDRAW_6_MONTHS 2


typedef enum {          /* PRIORITY */
  Num = 0,              /* num  = 0 */
  Plus = 1,             /* +,-  = 1 */
  Minus = 2,            /* *,/  = 2 */
  Multi = 3,            /* mod  = 3 */
  Division = 4,         /* ^    = 4 */
  Mod = 5,              /* fun  = 5 */
  Power = 6,            /* ()   = 6 */
  Sqrt = 7,             /* PRIORITY */
  Ln = 8,
  Log = 9,
  Sin = 10,
  Cos = 11,
  Tan = 12,
  Asin = 13,
  Acos = 14,
  Atan = 15,
  Lbracket = 16,
  Rbracket = 17,
  X = 0
} type_t;

typedef struct stack_t {
  double value;
  int priority;
  type_t type;
  struct stack_t* next;
} stack;

stack* init();
void push(stack** top, double value, int priority, type_t type);
void pop(stack**);
void delete (stack**);
void print_stack(stack* top);

void delete_spaces(const char* input_raw, char* input);
int validate_expression(const char* input_raw);
int is_real(const char*);
int is_digit(const char);
int is_bracket(const char*);
int is_permissible(const char*);
int is_function(const char*);
stack** parse(stack**, const char*);
stack* stack_reverse(stack**);
stack* polish(stack** general);
stack* copy_stack(stack* original);

double s21_smartcalc(const char* input);
void s21_graph(const char* input, int dots, double* x, double* y);
double calculation(stack* output);
void binary_calc(stack*, stack**);
void unary_calc(stack*, stack**);
void transfer(stack*, stack**);
double place_x(stack* output_for_calc, double value);

int validate_numbers(const char* input);
int is_permissible_numbers(const char* input);