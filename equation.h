#ifndef __EQUATION_H__
#define __EQUATION_H__

#include "utils.h"

#define	LIT_X      0
#define	LIT_Y      1
#define	LIT_ADD    2
#define	LIT_SUB    3
#define	LIT_MUL    4
#define	LIT_DIV    5
#define	LIT_AND    6
#define	LIT_OR     7
#define	LIT_XOR    8
#define	LIT_SIN    9
#define	LIT_COS   10
#define	LIT_TAN   11
#define	LIT_LOG   12
#define	LIT_EXP   13
#define	LIT_INV   14
#define	LIT_PL2   15
#define	LIT_PL3   16
#define	LIT_CEL1  17
#define	LIT_CEL2  18
#define	LIT_CEL3  19
#define	LIT_CEL4  20

typedef double (*operator_t)(double,double);

typedef struct litteral
{
   int	       arity;
   char        repr;
   operator_t  op;

} lit_t;

typedef struct node
{
   lit_t        *lit;
   struct node  *left,*right;

} node_t;

#define	EQU_DEPTH  5

typedef struct equation
{
   node_t   *root;
   double   min, max;

} equ_t;

void equation_init(equ_t*, unsigned int);
double equation_eval(equ_t*, double, double);

/* operators */
double op_1st(double,double);
double op_2nd(double,double);
double op_add(double,double);
double op_sub(double,double);
double op_mul(double,double);
double op_div(double,double);
double op_and(double,double);
double op_or(double,double);
double op_xor(double,double);
double op_sin(double,double);
double op_cos(double,double);
double op_tan(double,double);
double op_log(double,double);
double op_exp(double,double);
double op_inv(double,double);

#endif
