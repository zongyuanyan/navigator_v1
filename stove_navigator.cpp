#include "stove_navigator.h"
#include "ui_stove_navigator.h"

#include <iostream>

#include <QTime>
#include <QLabel>
#include <QTextEdit>
#include <QCalendarWidget>
#include <QFrame>
#include <QTreeView>
#include <QFileSystemModel>
#include <QBoxLayout>
#include <QSettings>
#include <QDockWidget>
#include <QDebug>
#include <QResizeEvent>
#include <QAction>
#include <QWidgetAction>
#include <QComboBox>
#include <QInputDialog>
#include <QRubberBand>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QScreen>
#include <QStyle>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QToolBar>
#include <QPointer>
#include <QMap>
#include <QElapsedTimer>
#include <QQuickWidget>
#include <QImage>

/**
 * Helper function to create an SVG icon
 */
static QIcon svgIcon(const QString& File)
{
    // This is a workaround, because in item views SVG icons are not
    // properly scaled and look blurry or pixelate
    QIcon SvgIcon(File);
    SvgIcon.addPixmap(SvgIcon.pixmap(92));
    return SvgIcon;
}

//============================================================================
/**
 * Private data class pimpl
 */

struct MainWindowPrivate
{
    Stove_Navigator* _this;
    Ui::Stove_Navigator ui;
    MainWindowPrivate(Stove_Navigator* _public) : _this(_public) {}

    double t_bed_left_1;
    double t_bed_right_1;
    double t_bed_left_2;
    double t_bed_right_2;
    double t_fur_left_mid;
    double t_fur_right_mid;
    double t_fur_left_top;
    double t_fur_right_top;

    QString filename;
    /**
     * Creates the toolbar actions
     */
    void createActions();

    /**
     * Fill the dock manager with dock widgets
     */
    void createContent();

    /**
     * add images to panels during initialization
     */
    void load_comubstion_heat_map();

    /**
     * add TEMP setting
     */

    void set_temp();

    /**
     * get stove temp
     */

    void get_stove_temp();

    /**
     * switch stove images
     */
    void switch_stove_images();
};

//============================================================================
void MainWindowPrivate::createActions()
{
    QAction *a;
    ui.toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui.action->setIcon(svgIcon(":/resources/images/stove_icon2.svg"));
    ui.action_2->setIcon(svgIcon(":/resources/images/stove_icon3.svg"));
    ui.action_3->setIcon(svgIcon(":/resources/images/stove_icon4.svg"));
    ui.action_4->setIcon(svgIcon(":/resources/images/stove_icon5.svg"));
    ui.action_5->setIcon(svgIcon(":/resources/images/stove_icon6.svg"));

    a = ui.action_5;
    _this->connect(a, &QAction::triggered, _this, &Stove_Navigator::ui_slot_switch_image);
}

//============================================================================
void MainWindowPrivate::load_comubstion_heat_map()
{
    QImage image(":/resources/images/H_H_H_H.png");
    ui.label_35->setPixmap(QPixmap::fromImage(image));
}

//============================================================================
void MainWindowPrivate::set_temp()
{

}

//============================================================================
void MainWindowPrivate::createContent()
{

}

//============================================================================
void MainWindowPrivate::get_stove_temp()
{
    t_fur_left_top = ui.doubleSpinBox->value();
    t_fur_right_top = ui.doubleSpinBox_2->value();
    t_fur_left_mid = ui.doubleSpinBox_3->value();
    t_fur_right_mid = ui.doubleSpinBox_4->value();
    t_bed_left_1 = ui.doubleSpinBox_5->value();
    t_bed_right_1 = ui.doubleSpinBox_6->value();
    t_bed_left_2 = ui.doubleSpinBox_7->value();
    t_bed_right_2 = ui.doubleSpinBox_8->value();
    qDebug("t_fur_left_top: %lf", t_fur_left_top);
    qDebug("t_fur_right_top: %lf", t_fur_right_top);
    qDebug("t_fur_left_mid: %lf", t_fur_left_mid);
    qDebug("t_fur_right_mid: %lf", t_fur_right_mid);
    qDebug("t_bed_left_1: %lf", t_bed_left_1);
    qDebug("t_bed_right_1: %lf", t_bed_right_1);
    qDebug("t_bed_left_2: %lf", t_bed_left_2);
    qDebug("t_bed_right_2: %lf", t_bed_right_2);
}

void MainWindowPrivate::switch_stove_images()
{
    QString status_A;
    QString status_B;
    QString status_C;
    QString status_D;

    if (((t_bed_left_1 + t_bed_right_1) / 2) > 600) {
        status_A = "H";
    } else {
        status_A = "L";
    }

    if (((t_bed_left_2 + t_bed_right_2) / 2) > 600) {
        status_B = "H";
    } else {
        status_B = "L";
    }

    if (((t_fur_left_mid + t_fur_left_top) / 2) > 850) {
        status_C = "H";
    } else {
        status_C = "L";
    }

    if (((t_fur_right_top + t_fur_right_top) / 2) > 850) {
        status_D = "H";
    } else {
        status_D = "L";
    }

    filename = ":/resources/images/" + status_A + "_" +
               status_B + "_" + status_C + "_" + status_D + ".png";
    qDebug() << filename;

    QImage image(filename);
    ui.label_35->setPixmap(QPixmap::fromImage(image));
}

Stove_Navigator::Stove_Navigator(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(this))
{
    d->ui.setupUi(this);

    d->createActions();

    d->createContent();
    d->load_comubstion_heat_map();
    d->set_temp();

    // Default window geometry - center on screen
    resize(1280, 720);
}

Stove_Navigator::~Stove_Navigator()
{
    delete d;
}

void Stove_Navigator::ui_slot_switch_image()
{
    qDebug() << "Stove_Navigator::on_actionrun_triggered";
    d->get_stove_temp();
    d->switch_stove_images();
}
