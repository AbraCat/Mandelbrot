#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "mbritem.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class Mandelbrot;
}
QT_END_NAMESPACE

class Mandelbrot : public QMainWindow
{
    Q_OBJECT

public:
    Mandelbrot(QWidget *parent = nullptr);
    ~Mandelbrot();

private:
    Ui::Mandelbrot *ui;

    QGraphicsView *view;
    QGraphicsScene *scene;
    MbrItem* mbr_item;
    QLabel* label1;
};
#endif // MANDELBROT_H
