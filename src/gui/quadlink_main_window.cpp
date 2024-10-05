#include "quadlink_main_window.h"
#include "./ui_quadlink_main_window.h"

quadlink_main_window::quadlink_main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quadlink_main_window)
{
    ui->setupUi(this);
}

quadlink_main_window::~quadlink_main_window()
{
    delete ui;
}
