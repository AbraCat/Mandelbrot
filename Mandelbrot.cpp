#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::MandelbrotClass())
{
    ui->setupUi(this);

    settings_act = new QAction;
    settings_act->setText("Settings");
    QObject::connect(settings_act, &QAction::triggered, this, &Mandelbrot::settings_dialog);
    ui->mainToolBar->addAction(settings_act);

    properties_status = new QLabel;
    settings_status = new QLabel;
    ui->statusBar->addWidget(properties_status);
    ui->statusBar->addWidget(settings_status);

    view = new QGraphicsView;
    scene = new QGraphicsScene;
    view->setScene(scene);
    setCentralWidget(view);

    mbr_item = new MbrItem(properties_status, settings_status);
    scene->addItem(mbr_item);
}

Mandelbrot::~Mandelbrot()
{
    delete ui;
    delete scene;
    delete settings_act;
}

void Mandelbrot::settings_dialog()
{
    std::vector<int> values = mbr_item->get_settings();
    SettingsDialog dialog(values);
    QObject::connect(&dialog, &SettingsDialog::settings_signal, mbr_item, &MbrItem::set_settings);
    dialog.exec();
}
