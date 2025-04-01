#include "Mandelbrot.h"
#include <QtWidgets/QApplication>
#include "calc-mbr.h"

#include <string.h>

MbrProp std_mbr = {1500, 750, 256, 0, 0, 300};
const int n_tests = 100;

int main(int argc, char* argv[])
{
    if (0) // if (argc >= 2 && strcmp(argv[1], "-m") == 0)
    {
        if (measure_mbr_time(&std_mbr, n_tests, "../../result.txt") != 0)
            return 1;
        return 0;
    }

    QApplication a(argc, argv);
    Mandelbrot w;
    w.show();
    return a.exec();
}
