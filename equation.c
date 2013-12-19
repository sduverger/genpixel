#include "equation.h"
#include <stdlib.h>

static lit_t litterals[] = {
   {.arity = 0, .repr = 'x', .op = op_1st},
   {.arity = 0, .repr = 'y', .op = op_2nd},
   {.arity = 2, .repr = '+', .op = op_add},
   {.arity = 2, .repr = '-', .op = op_sub},
   {.arity = 2, .repr = '*', .op = op_mul},
   {.arity = 2, .repr = '/', .op = op_div},
   {.arity = 2, .repr = '&', .op = op_and},
   {.arity = 2, .repr = '|', .op = op_or },
   {.arity = 2, .repr = '^', .op = op_xor},
   {.arity = 1, .repr = 'S', .op = op_sin},
   {.arity = 1, .repr = 'C', .op = op_cos},
   {.arity = 1, .repr = 'T', .op = op_tan},
   {.arity = 1, .repr = 'L', .op = op_log},
   {.arity = 1, .repr = 'E', .op = op_exp},
   {.arity = 1, .repr = 'I', .op = op_inv},
};

static inline lit_t* lit_rand()
{
   return &litterals[randinto(0, (sizeof(litterals)/sizeof(lit_t))-1)];
}

static inline lit_t* lit_rand_none()
{
   return &litterals[randtwo()];
}

static node_t* node_new(lit_t *lit)
{
   node_t *node = malloc(sizeof(node_t));
   if(!node)
      error("node_new");

   node->lit = lit;
   node->left = node->right = (node_t*)0;
   return node;
}

static node_t* __gen(unsigned int depth)
{
   node_t *node = node_new(lit_rand());

   /* final nodes must have arity 0 */
   if(node->lit->arity != 0 && depth == 0)
   {
      node->lit = lit_rand_none();
      return node;
   }

   /* non optimal max depth equation */
   if(node->lit->arity == 0)
      return node;

   if(node->lit->arity == 2)
      node->left = __gen(depth-1);

   node->right = __gen(depth-1);
   return node;
}

static double __eval(node_t *node, double x, double y)
{
   if(!node)
      return 0;

   switch(node->lit->arity)
   {
   case 0: return node->lit->op(x,y);
   case 1: return node->lit->op(__eval(node->right,x,y), 0);
   case 2: return node->lit->op(__eval(node->left,x,y), __eval(node->right,x,y));
   }

   error("invalid node: %c arity %d", node->lit->repr, node->lit->arity);
}

void equation_init(equ_t *eq, unsigned int depth)
{
   eq->root = __gen(depth);
   eq->min =  10000000.0;
   eq->max = -10000000.0;
}

double equation_eval(equ_t *eq, double x, double y)
{
   double value = __eval(eq->root, x, y);

   if(value < eq->min)
      eq->min = value;
   if(value > eq->max)
      eq->max = value;

   return value;
}

static void __print(node_t *node, FILE *file)
{
   if(!node) return;

   if(node->lit->arity == 2)
      fputc('(', file);

   __print(node->left, file);
   fputc(node->lit->repr, file);
   __print(node->right, file);

   if(node->lit->arity == 2)
      fputc(')', file);
}

void equation_print(equ_t *eq, FILE *file)
{
   __print(eq->root, file);
}
