#include <math.h>

double op_1st(double x, double y)
{
   return x;
}

double op_2nd(double x, double y)
{
   return y;
}

double op_add(double x, double y)
{
   return x+y;
}

double op_sub(double x, double y)
{
   return x-y;
}

double op_mul(double x, double y)
{
   return x*y;
}

double op_div(double x, double y)
{
   if(y == 0) y = 0.0000001;
   return x/y;
}

double op_and(double x, double y)
{
   return (double)((unsigned long)x & (unsigned long)y);
}

double op_or(double x, double y)
{
   return (double)((unsigned long)x | (unsigned long)y);
}

double op_xor(double x, double y)
{
   return (double)((unsigned long)x ^ (unsigned long)y);
}

double op_sin(double x, double y)
{
   return sin(x);
}

double op_cos(double x, double y)
{
   return cos(x);
}

double op_tan(double x, double y)
{
   return tan(x);
}

double op_log(double x, double y)
{
   return log(x);
}

double op_exp(double x, double y)
{
   return exp(x);
}

double op_inv(double x, double y)
{
   if(x == 0) x = 0.0000001;
   return 1/x;
}
