#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Mandelbrot.h"
#include "MbrItem.h"
#include "SettingsDialog.h"
#include <qgraphicsview.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MandelbrotClass; };
QT_END_NAMESPACE

class Mandelbrot : public QMainWindow
{
    Q_OBJECT

public:
    Mandelbrot(QWidget *parent = nullptr);
    ~Mandelbrot();
public slots:
    void settings_dialog();
private:
    Ui::MandelbrotClass *ui;
    QGraphicsView* view;
    QGraphicsScene* scene;
    MbrItem* mbr_item;

    QAction* settings_act;
    QLabel* properties_status;
    QLabel* settings_status;
};
