#ifndef __EQUATION_H__
#define __EQUATION_H__

#include "utils.h"
#include <stdio.h>

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

void   equation_init(equ_t*, unsigned int);
double equation_eval(equ_t*, double, double);
void   equation_print(equ_t*, FILE*);

#endif
