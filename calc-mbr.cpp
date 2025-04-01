#include "calc-mbr.h"

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

clock_t calc_set(MbrProp* p, std::vector<std::vector<int>>* set)
{
    if (set != nullptr) *set = std::vector<std::vector<int>>(p->size_y, std::vector<int>(p->size_x));

    clock_t start_time = clock();
    for (int i = 0; i < p->size_y; ++i)
    {
        for (int j = 0; j < p->size_x; ++j)
        {
            // size_x / 2 - j = scale * (x0 - xp)
            double xp = (j - p->size_x / 2) / p->scale - p->x0, yp = (i - p->size_y / 2) / p->scale - p->y0;
            double x = 0, y = 0;
            int iter = 0;
            for (; iter < p->iters; ++iter)
            {
                double new_x = x*x - y*y + xp;
                double new_y = 2*x*y + yp;
                x = new_x;
                y = new_y;
                if (x*x + y*y >= 2) break;
            }

            if (set != nullptr) (*set)[i][j] = iter;
        }
    }

    return clock() - start_time;
}

double* time_arr(MbrProp* p, int n)
{
    double* time = (double*)calloc(n, sizeof(double));
    for (int i = 0; i < n; ++i)
    {
        clock_t ticks = calc_set(p, nullptr);
        time[i] = 1e3 * ticks / CLOCKS_PER_SEC;
    }
    return time;
}

double expectation(double* a, int n)
{
    double e = 0;
    for (int i = 0; i < n; ++i)
        e += a[i];
    return e / n;
}

double std_dev(double* a, int n, double e)
{
    double exp_of_sq = 0;
    for (int i = 0; i < n; ++i)
        exp_of_sq += a[i] * a[i];
    exp_of_sq /= n;
    return std::sqrt(exp_of_sq - e * e);
}

double exp_of_good_vals(double* a, int n, double e, double std_dev)
{
    const int good_coeff = 2;
    double good_e = 0;
    int good_cnt = 0;

    for (int i = 0; i < n; ++i)
        if (e - good_coeff * std_dev <= a[i] && a[i] <= e + good_coeff * std_dev)
        {
            good_e += a[i];
            ++good_cnt;
        }

    return good_e / good_cnt;
}

int measure_mbr_time(MbrProp* p, int n, const char* file_path)
{
    double* a = time_arr(p, n);

    FILE* file = fopen(file_path, "a");
    if (file == NULL)
    {
        free(a);
        fprintf(stderr, "Couldn't open file");
        return 1;
    }

    double e = expectation(a, n);
    double sigma = std_dev(a, n, e);
    double good_e = exp_of_good_vals(a, n, e, sigma);

    fprintf(file, "avg time: %lf\nstdndart deviation: %lf\navg time (excluding bad tests): %lf\n", e, sigma, good_e);

    fclose(file);
    free(a);
    return 0;
}
