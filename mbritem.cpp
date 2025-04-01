#include "mbritem.h"

#include <QPainter>
#include <QKeyEvent>

const int size_x = 1600, size_y = 600, iters = 256;

MbrItem::MbrItem(QGraphicsItem* parent) : QGraphicsObject(parent)
{
    scale = 300;
    x0 = y0 = 0;

    colors = std::vector<QColor>(iters + 1);
    setFlags(QGraphicsItem::ItemIsFocusable);
    setFocus();
    fill_colors();
    req_update();
}

MbrItem::~MbrItem()
{
    //
}

QRectF MbrItem::boundingRect() const
{
    return QRectF(-size_x / 2 - 1, -size_y / 2 - 1, size_x / 2 - 1, size_y / 2 - 1);
}

void MbrItem::req_update()
{
    MbrProp prop = {size_x, size_y, iters, x0, y0, scale};
    calc_set(&prop, &set);
    update();
}

void MbrItem::fill_colors()
{
    for (int i = 0; i < iters; ++i)
    {
        colors[i] = Qt::blue;
    }
    colors[iters] = Qt::black;
}

void MbrItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    const int x_shift = -400, y_shift = -200;
    painter->save();
    painter->setPen(QPen(QBrush(Qt::transparent), 0));

    for (int i = -size_y / 2; i < size_y - size_y / 2; ++i)
    {
        for (int j = -size_x / 2; j < size_x - size_x / 2; ++j)
        {
            painter->setBrush(colors[set[i + size_y / 2][j + size_x / 2]]);
            painter->drawRect(j + x_shift, i + y_shift, 1, 1);
        }
    }

    // painter->setBrush(Qt::red);
    // painter->drawRect(0, 0, 10, 10);

    painter->restore();
}

void MbrItem::keyPressEvent(QKeyEvent* e)
{
    const int step = 50;
    const double scale_mod = 1.5;

    switch (e->key())
    {
        case Qt::Key_Z:
            scale *= scale_mod;
            break;
        case Qt::Key_X:
            scale /= scale_mod;
            break;
        case Qt::Key_Left:
            x0 += step / scale;
            break;
        case Qt::Key_Right:
            x0 -= step / scale;
            break;
        case Qt::Key_Up:
            y0 += step / scale;
            break;
        case Qt::Key_Down:
            y0 -= step / scale;
            break;
        default:
            break;
    }

    req_update();
}
