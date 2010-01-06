#ifndef MATH_H
#define MATH_H

#define PI 3.14159265

outside cdecl: double cos(double x);
outside cdecl: double floor(double x);
outside cdecl: int isdigit ( int c );

double round( double value )
{
  return floor( value + 0.5 );
}

#endif
