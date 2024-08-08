#pragma once
#include <vector>
#include <qthread.h>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qpainterpath.h>
class CompThread : public QThread
{
	Q_OBJECT
public:
	CompThread(QObject* parent = nullptr);
	~CompThread();
	virtual void run() override;
	void set_properties(const double& x0, const double& y0, const double& scale, const int& iters, const int& size_x, const int& size_y, int (*rule)(const double&, const double&, const int&));
	void finish();
signals:
	void mbr_signal(const std::vector<std::vector<int>>& mbr);
private:
	double x0;
	double y0;
	double scale;
	int iters;
	int size_x;
	int size_y;
	int (*rule)(const double&, const double&, const int&);

	bool finish_req;
};

