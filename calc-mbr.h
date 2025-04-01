#ifndef CALC_MBR_H
#define CALC_MBR_H

#include <time.h>
#include <vector>

struct MbrProp
{
    int size_x, size_y, iters;
    float x0, y0, scale;
};

clock_t calc_set_unopt(MbrProp* p, std::vector<std::vector<int>>* set);
clock_t calc_set_unr(MbrProp* p, std::vector<std::vector<int>>* set);
clock_t calc_set_intr(MbrProp* p, std::vector<std::vector<int>>* set);
extern clock_t (*calc_set)(MbrProp* p, std::vector<std::vector<int>>* set);

float* time_arr(MbrProp* p, int n);
int measure_mbr_time(MbrProp* p, int n, const char* file_path);

#endif // CALC_MBR_H
