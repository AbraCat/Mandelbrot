#include "calc-mbr.h"

MbrProp std_mbr = {1500, 750, 256, 0, 0, 300};
const int n_tests = 64;

#include <stdio.h>
int main()
{
    if (measure_mbr_time(&std_mbr, n_tests, "./result.txt") != 0)
        return 1;
    return 0;
}