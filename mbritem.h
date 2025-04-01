#ifndef MBRITEM_H
#define MBRITEM_H

#include "calc-mbr.h"

#include <QGraphicsObject>
#include <QObject>

class MbrItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MbrItem(QGraphicsItem* parent = nullptr);
    ~MbrItem();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    virtual void keyPressEvent(QKeyEvent* e) override;

    void fill_colors();
    void req_update();

private:
    double x0, y0, scale;

    std::vector<std::vector<int>> set;
    std::vector<QColor> colors;
};

#endif // MBRITEM_H
