#ifndef UNITTTESTS_H
#define UNITTTESTS_H

#include <stdio.h>

typedef int (*UnitTestType)(void);

#define RUN_UNIT_TEST(fct) printf("%80s: %d\n", #fct, fct())

#endif
