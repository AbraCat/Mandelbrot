#include "mandelbrot.h"
#include "./ui_mandelbrot.h"

Mandelbrot::Mandelbrot(QWidget *parent) : QMainWindow(parent), ui(new Ui::Mandelbrot)
{
    ui->setupUi(this);

    view = new QGraphicsView;
    scene = new QGraphicsScene;
    view->setScene(scene);
    setCentralWidget(view);

    mbr_item = new MbrItem();
    scene->addItem(mbr_item);

    label1 = new QLabel;
    label1->setText("Hello");
    ui->statusbar->addWidget(label1);
}

Mandelbrot::~Mandelbrot()
{
    delete ui;
    delete scene;
}
