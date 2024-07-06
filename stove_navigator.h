#ifndef STOVE_NAVIGATOR_H
#define STOVE_NAVIGATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Stove_Navigator;
}
QT_END_NAMESPACE

class Stove_Navigator : public QMainWindow
{
    Q_OBJECT

public:
    Stove_Navigator(QWidget *parent = nullptr);
    ~Stove_Navigator();

private:
    Ui::Stove_Navigator *ui;
};
#endif // STOVE_NAVIGATOR_H
