#ifndef CALC_MBR_H
#define CALC_MBR_H

#include <time.h>
#include <vector>

struct MbrProp
{
    int size_x, size_y, iters;
    double x0, y0, scale;
};

clock_t calc_set(MbrProp* p, std::vector<std::vector<int>>* set);
double* time_arr(MbrProp* p, int n);
int measure_mbr_time(MbrProp* p, int n, const char* file_path);

#endif // CALC_MBR_H
