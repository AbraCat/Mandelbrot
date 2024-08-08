#pragma once
#include "CompThread.h"
#include <cmath>
#include <qlabel.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>

class MbrItem : public QGraphicsObject
{
	Q_OBJECT
public:
	MbrItem(QLabel* properties_status, QLabel* settings_status);
	~MbrItem();
	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	void update_req(const double& x0, const double& y0, const double& scale);

	void set_iters_colors(const int& iters, const int& init_hue, const int& hue_cycle, const bool& hue_dir, const int& s, const int& v);
	void set_settings(const std::vector<int>& values);

	std::vector<int> get_settings();
	void display_settings();
public slots:
	void mbr_slot(const std::vector<std::vector<int>>& mbr);
protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void wheelEvent(QGraphicsSceneWheelEvent* event) override;
private:
	double x0;
	double y0;
	double scale;

	int iters;
	int init_hue;
	int hue_cycle;
	bool hue_dir;
	int s;
	int v;
	std::vector<QColor> colors;

	int size_x;
	int size_y;
	int (*rule)(const double&, const double&, const int&);

	std::vector<std::vector<int>> mbr;
	CompThread* prev_comp;
	QLabel* properties_status;
	QLabel* settings_status;

	double wheel_scale;
	double x_press_cursor;
	double y_press_cursor;
	double x_press_item;
	double y_press_item;
};

