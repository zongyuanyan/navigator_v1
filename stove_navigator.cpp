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

};

//============================================================================
void MainWindowPrivate::createActions()
{
    ui.toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui.action->setIcon(svgIcon(":/resources/images/stove_icon2.svg"));
    ui.action_2->setIcon(svgIcon(":/resources/images/stove_icon3.svg"));
    ui.action_3->setIcon(svgIcon(":/resources/images/stove_icon4.svg"));
    ui.action_4->setIcon(svgIcon(":/resources/images/stove_icon5.svg"));
    ui.action_5->setIcon(svgIcon(":/resources/images/stove_icon6.svg"));
}

//============================================================================
void MainWindowPrivate::load_comubstion_heat_map()
{
    QImage image(":/resources/images/H_H_H_H.png");
    ui.label_35->setPixmap(QPixmap::fromImage(image));
}

void MainWindowPrivate::set_temp()
{
    // QLabel *T_Bed_LEFT_1 = new QLabel(_this);
    // T_Bed_LEFT_1->setGeometry(QRect(280, -50, 250, 329));
    // T_Bed_LEFT_1->setText("左侧床层前端温度:");
    // T_Bed_LEFT_1->setScaledContents(true);

    // QLineEdit *T_Bed_LEFT_EDIT_1 = new QLineEdit(_this);
    // T_Bed_LEFT_EDIT_1->setGeometry(QRect(385, 105, 30, 20));
    // T_Bed_LEFT_EDIT_1->setText("655");
}

//============================================================================
void MainWindowPrivate::createContent()
{

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
