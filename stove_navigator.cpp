#include "stove_navigator.h"
#include "ui_stove_navigator.h"

Stove_Navigator::Stove_Navigator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Stove_Navigator)
{
    this->setWindowTitle(QString("test"));
    ui->setupUi(this);
}
Stove_Navigator::~Stove_Navigator()
{
    delete ui;
}
