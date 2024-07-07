#ifndef STOVE_NAVIGATOR_H
#define STOVE_NAVIGATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Stove_Navigator;
}
QT_END_NAMESPACE

struct MainWindowPrivate;

class Stove_Navigator : public QMainWindow
{
    Q_OBJECT
private:
    MainWindowPrivate* d; ///< private data - pimpl
    friend struct MainWindowPrivate;
public:
    Stove_Navigator(QWidget *parent = nullptr);
    ~Stove_Navigator();

private:
    Ui::Stove_Navigator *ui;

private slots:
    void ui_slot_switch_image();
    void ui_slot_initial_params();

};
#endif // STOVE_NAVIGATOR_H
