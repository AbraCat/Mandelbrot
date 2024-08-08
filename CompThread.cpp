#include "CompThread.h"

CompThread::CompThread(QObject* parent) : QThread(parent)
{
	finish_req = 0;
}

CompThread::~CompThread()
{

}

void CompThread::run()
{
	int lx = -size_x / 2, ly = -size_y / 2;
	std::vector<std::vector<int>> mbr((size_x / 2) - (-size_x / 2) + 1, std::vector<int>((size_y / 2) - (-size_y / 2) + 1));
	for (int x = -size_x / 2; x != size_x / 2 + 1; ++x)
	{
		for (int y = -size_y / 2; y != size_y / 2 + 1; ++y)
		{
			mbr[x - lx][y - ly] = rule(x0 + x / scale, y0 + y / scale, iters);
			if (finish_req)
			{
				return;
			}
		}
	}
	emit mbr_signal(mbr);
}

void CompThread::set_properties(const double& x0, const double& y0, const double& scale, const int& iters, const int& size_x, const int& size_y, int (*rule)(const double&, const double&, const int&))
{
	this->x0 = x0;
	this->y0 = y0;
	this->scale = scale;
	this->iters = iters;
	this->size_x = size_x;
	this->size_y = size_y;
	this->rule = rule;
}

void CompThread::finish()
{
	finish_req = 1;
}