#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "doOperator.h"
#include "tokenStack.h"
#include "lexical.h"

static int op_quit(struct tokenStack *stack);
static int op_print(struct tokenStack *stack);
static int op_dump(struct tokenStack *stack);
static int op_add(struct tokenStack *stack);
static int op_diff(struct tokenStack *stack);
static int op_product(struct tokenStack *stack);
static int op_quotient(struct tokenStack *stack);
static int op_gt(struct tokenStack *stack);
static int op_lt(struct tokenStack *stack);
static int op_ge(struct tokenStack *stack);
static int op_le(struct tokenStack *stack);
static int op_eq(struct tokenStack *stack);
static int op_mod(struct tokenStack *stack);
static int op_if(struct tokenStack *stack);
static int op_s(struct tokenStack *stack);
static int op_help(struct tokenStack *stack);

static struct operator_struct {
  char *name;
  int (*fn_ptr)(struct tokenStack *);
} ops[] = {
  {"quit", op_quit},
  {"print", op_print},
  {"dump", op_dump},
  {"+", op_add},
  {"-", op_diff},
  {"*", op_product},
  {"/", op_quotient},
  {"GT", op_gt},
  {"LT", op_lt},
  {"GE", op_ge},
  {"LE", op_le},
  {"EQ", op_eq},
  {"MOD", op_mod},
  {"IF", op_if},
  {"S", op_s},
  {"HELP", op_help},
  {(char *)NULL, (int(*)(struct tokenStack *)) NULL}
};

/* YOU WRITE THIS */
static int popInt(struct tokenStack *s)
{
  int negative = 0, result = 0, digits = 0, i = 0, power = 1;
  struct lexToken *token = popTokenStack(s);
  
  if(token->symbol[i] == '-' || token->symbol[i] == '+'){
    negative = token->symbol[i] == '-' ? 1 : 0;
    i++;
  }
  
  while (token->symbol[i] != '\0') {
    digits++;
    i++;
  }
  i--;
  for(;digits > 0; digits--, i--){
    result += ((token->symbol[i]) - 48) * power;
    power *= 10;
  }
  
  if(negative == 1) result = -result;
  
  freeToken(token);
  return result;
}

/* YOU WRITE THIS */
static void pushInt(struct tokenStack *s, int v)
{
  int power = 10, i = 0;
  struct lexToken *token = allocToken();
  token->kind = LEX_TOKEN_NUMBER;
  
  if(v < 0){
    v = -v;
    token->symbol[0] = '-';
    i = 1;
  }
  
  while (power <= v) {
    power *= 10;
  }
  
  while (power > 1){
    power /= 10;
    token->symbol[i] = (v/power + 48);
    v %= power; 
    i++;
  }
  
  token->symbol[i] = '\0';
  pushTokenStack(s, token);
}

/* execute the operator */
int doOperator(struct tokenStack *stack, char *o) 
{
  struct operator_struct *op = ops;
  for(op=ops;op->name != (char *)NULL; op++) {
    if(!strcmp(op->name, o))
      return op->fn_ptr(stack);
  }
  return(-1);
}

/*ARGSUSED*/
static int op_quit(struct tokenStack *stack)
{
  printf("[quit]\n");
  exit(0);
  /*NOTREACHED*/
}

static int op_print(struct tokenStack *stack)
{
  struct lexToken *t = popTokenStack(stack);
  printToken(stdout, t);
  freeToken(t);
  return(0);
}

static int op_dump(struct tokenStack *stack)
{
  struct lexToken *t = popTokenStack(stack);
  dumpToken(stdout, t);
  freeToken(t);
  return(0);
}

static int op_add(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1+v2);
  return(0);
}

static int op_diff(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v2-v1);
  return(0);
}

static int op_product(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1*v2);
  return 0;
}

static int op_quotient(struct tokenStack *stack){
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v2/v1);
  return 0;
}

static int op_gt(struct tokenStack *stack){
  int n2, n1;
  n2 = popInt(stack);
  n1 = popInt(stack);
  pushInt(stack, n1 > n2 ? 1 : 0);
  return 0;
}

static int op_lt(struct tokenStack *stack){
  int n2, n1;
  n2 = popInt(stack);
  n1 = popInt(stack);
  pushInt(stack, n1 < n2 ? 1 : 0);
  return 0;
}


static int op_ge(struct tokenStack *stack){
  int n2, n1;
  n2 = popInt(stack);
  n1 = popInt(stack);
  pushInt(stack, n1 >= n2 ? 1 : 0);
  return 0;
}

static int op_le(struct tokenStack *stack){
  int n2, n1;
  n2 = popInt(stack);
  n1 = popInt(stack);
  pushInt(stack, n1 <= n2 ? 1 : 0);
  return 0;
}

static int op_eq(struct tokenStack *stack){
  int n2, n1;
  n2 = popInt(stack);
  n1 = popInt(stack);
  pushInt(stack, n1 == n2 ? 1 : 0);
  return 0;
}

static int op_mod(struct tokenStack *stack){
  int n1;
  n1 = popInt(stack);
  pushInt(stack, n1);
  pushInt(stack, n1);
  return 0;
}

static int op_if(struct tokenStack *stack){
  int v, n2, n1;
  v = popInt(stack);
  n2 = popInt(stack);
  n1 = popInt(stack);
  pushInt(stack, v == 0 ? n2 : n1);
  return 0;
}

static int op_s(struct tokenStack *stack){
  struct tokenStack *temp = createTokenStack();
  
  while(!emptyTokenStack(stack))
    pushTokenStack(temp, popTokenStack(stack));
  
  printf("all tokens: \n");
  while (!emptyTokenStack(temp)) {
    struct lexToken *t = popTokenStack(temp);
    printToken(stdout, t);
    pushTokenStack(stack, t);
  }
  printf("\n");
  
  free(temp);
  return 0;
}

static int op_help(struct tokenStack *stack)
{
  printf("
    - - (n1 n2 - diff)\n
    - * (n1 n2 - product) - push n1*n2 \n
    - / (n1 n2 - quotient) - push n1/n2 \n
    - GT (n1 n2 — gt) -push 1 if n1 > n2 and 0 otherwise\n
    - LT (n1 n2 — lt) -push 1 if n1 < n2 and 0 otherwise\n
    - GE (n1 n2 — ge) -push 1 if n1 >= n2 and 0 otherwise\n
    - LE (n1 n2 — le) -push 1 if n1 <= n2 and 0 otherwise\n
    - EQ (n1 n2 — eq) -push 1 if n1 == n2 and 0 otherwise\n
    - MOD (n1 - n1 n1) - push two copies of n1 onto the stack\n
    - IF (n1 n2 v — x) - if v is not zero then push n1 otherwise n2\n
    - S (—) - print all elements on the stack non destructively\n
  ");
  return 0;
}
