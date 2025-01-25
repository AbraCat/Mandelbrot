#include "MbrItem.h"

MbrItem::MbrItem(QLabel* properties_status, QLabel* settings_status) : QGraphicsObject()
{
	this->properties_status = properties_status;
	this->settings_status = settings_status;

	pixel_size = 2;
	size_x = 1940 / pixel_size;
	size_y = 1100 / pixel_size;

	rule = [](const double& x, const double& y, const int& iters)
		{
			double xp = 0, yp = 0;
			for (int i = 0; i != iters; ++i)
			{
				double x_prev = xp, y_prev = yp;
				xp = x_prev * x_prev - y_prev * y_prev + x;
				yp = 2 * x_prev * y_prev + y;
				if (xp * xp + yp * yp > 4) return i;
			}
			return iters;
		};

	mbr = std::vector<std::vector<int>>((size_x / 2) - (-size_x / 2) + 1, std::vector<int>((size_y / 2) - (-size_y / 2) + 1));

	iters = 100;
	set_iters_colors(100, 0, 100, 0, 255, 255);

	setAcceptedMouseButtons(Qt::LeftButton);
	x_press_cursor = y_press_cursor = x_press_item = y_press_item = 0;
	wheel_scale = 1.25;

	prev_comp = nullptr;
	properties_status->setText("x0: 0, y0: 0, scale: 100");
	display_settings();
	update_req(0, 0, 100.0 / pixel_size);
}

MbrItem::~MbrItem()
{
	// :]
}

QRectF MbrItem::boundingRect() const
{
	return QRectF(-size_x / 2 - 1, -size_y / 2 - 1, size_x + 2, size_y + 2);
}

void MbrItem::update_req(const double& x0, const double& y0, const double& scale)
{
	this->x0 = x0;
	this->y0 = y0;
	this->scale = scale;
	properties_status->setText("x0: " + QString::number(x0) + ", y0: " + QString::number(y0) + 
		", scale: " + QString::number((long long)(scale * pixel_size)));

	CompThread* thread = new CompThread;
	thread->set_properties(x0, y0, scale, iters, size_x, size_y, rule);
	QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
	QObject::connect(thread, &CompThread::mbr_signal, this, &MbrItem::mbr_slot);

	if (prev_comp != nullptr)
	{
		prev_comp->finish();
	}

	thread->start();
	prev_comp = thread;
}

void MbrItem::mbr_slot(const std::vector<std::vector<int>>& mbr)
{
	prev_comp = nullptr;
	this->mbr = mbr;
	setPos(0, 0);
	QTransform transform;
	transform.scale(pixel_size, pixel_size);
	setTransform(transform, 0);
	update();
}

void MbrItem::set_iters_colors(const int& iters, const int& init_hue, const int& hue_cycle, const bool& hue_dir, const int& s, const int& v)
{
	int prev_iters = this->iters;

	this->iters = iters;
	this->init_hue = init_hue;
	this->hue_cycle = hue_cycle;
	this->hue_dir = hue_dir;
	this->s = s;
	this->v = v;
	colors = std::vector<QColor>(iters + 1, QColor());
	colors[iters] = QColor::fromHsv(0, 255, 0);
	for (int i = 0; i != iters; ++i)
	{
		colors[i] = QColor::fromHsv((int(init_hue + (1 - 2 * hue_dir) * 360.0 * i / hue_cycle) % 360 + 360) % 360, s, v);
	}
	for (int i = iters + 1; i < prev_iters + 1; ++i)
	{
		colors.push_back(QColor::fromHsv(0, 255, 0));
	}
}

void MbrItem::set_settings(const std::vector<int>& values)
{
	wheel_scale = values[1] / 1000.0;
	set_iters_colors(values[0], values[5], values[6], values[7], values[8], values[9]);

	int old_pixel_size = pixel_size;
	pixel_size = values[2];
	int new_size_x = values[3] / pixel_size, new_size_y = values[4] / pixel_size;

	if (size_x != new_size_x || size_y != new_size_y)
	{
		for (int x = 0; x != (size_x / 2) - (-size_x / 2) + 1; ++x)
		{
			for (int y = (size_y / 2) - (-size_y / 2) + 1; y < (new_size_y / 2) - (-new_size_y / 2) + 1; ++y)
				mbr[x].push_back(0);
		}
		for (int x = (size_x / 2) - (-size_x / 2) + 1; x < (new_size_x / 2) - (-new_size_x / 2) + 1; ++x)
		{
			mbr.push_back(std::vector<int>(std::max((size_y / 2) - (-size_y / 2) + 1, (new_size_y / 2) - (-new_size_y / 2) + 1)));
		}

		prepareGeometryChange();
		size_x = new_size_x;
		size_y = new_size_y;
	}

	display_settings();
	update_req(x0, y0, scale * old_pixel_size / pixel_size);
}

void MbrItem::display_settings()
{
	settings_status->setText("iters: " + QString::number(iters) + ", wheel sens: " + QString::number(int(wheel_scale * 1000)) +
		", pixel size: " + QString::number(pixel_size) + 
		", size x: " + QString::number(size_x * pixel_size) + ", size y: " + QString::number(size_y * pixel_size) + 
		", init hue: " + QString::number(init_hue) + ", hue cycle: " + QString::number(hue_cycle) + ", hue dir: " + QString::number(hue_dir) + 
		", satur: " + QString::number(s) + ", value: " + QString::number(v));
}

std::vector<int> MbrItem::get_settings()
{
	return std::vector<int>({ iters, int(wheel_scale * 1000), pixel_size, size_x * pixel_size, size_y * pixel_size, init_hue, hue_cycle, hue_dir, s, v });
}

void MbrItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();
	painter->setPen(QPen(QBrush(Qt::transparent), 0));

	int lx = -size_x / 2, ly = -size_y / 2;
	for (int x = -size_x / 2; x != size_x / 2 + 1; ++x)
	{
		for (int y = -size_y / 2; y != size_y / 2 + 1; ++y)
		{
			painter->setBrush(QBrush(colors[mbr[x - lx][y - ly]]));
			painter->drawRect(x, -y, 1, 1);
		}
	}

	painter->restore();
}

void MbrItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	x_press_cursor = event->scenePos().x();
	y_press_cursor = event->scenePos().y();
	x_press_item = x();
	y_press_item = y();
}

void MbrItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	setPos(x_press_item + event->scenePos().x() - x_press_cursor, y_press_item + event->scenePos().y() - y_press_cursor);
}

void MbrItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	/*
	x_new - x0 = (x_press - x_release) / scale
	x_new = x0 + (x_press - x_release) / scale
	*/
	double outer_scale = scale * pixel_size;
	setPos(x_press_item + event->scenePos().x() - x_press_cursor, y_press_item + event->scenePos().y() - y_press_cursor);
	update_req(x0 + (x_press_cursor - event->scenePos().x()) / outer_scale, y0 - (y_press_cursor - event->scenePos().y()) / outer_scale, scale);
	x_press_cursor = y_press_cursor = x_press_item = y_press_item = 0;
}

void MbrItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	/*
	x - x0 = x_scene / scale
	x - x0_new = (x - x0) / event_scale
	x = x0 + x_scene / scale
	x0_new = x - (x - x0) / event_scale
	*/
	double event_scale = std::pow(wheel_scale, event->delta() / 120);
	double outer_scale = scale * pixel_size;

	double x_scene = event->scenePos().x(), y_scene = -event->scenePos().y();
	double x = x0 + x_scene / outer_scale, y = y0 + y_scene / outer_scale;
	double x0_new = x - (x - x0) / event_scale, y0_new = y - (y - y0) / event_scale;

	double x_item = event->pos().x(), y_item = event->pos().y();
	QTransform transform;
	transform.translate(x_item, y_item);
	transform.scale(event_scale, event_scale);
	transform.translate(-x_item, -y_item);
	setTransform(transform, 1);

	update_req(x0_new, y0_new, scale * event_scale);
}