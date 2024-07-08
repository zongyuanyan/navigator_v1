#include "stove_navigator.h"
#include "ui_stove_navigator.h"
#include "params.h"

#include <iostream>
#include <cstring>

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

    /* temp label value */
    double t_bed_left_1;
    double t_bed_right_1;
    double t_bed_left_2;
    double t_bed_right_2;
    double t_fur_left_mid;
    double t_fur_right_mid;
    double t_fur_left_top;
    double t_fur_right_top;

    /* image name will switch */
    QString filename;

    /* control which module open */
    bool RUN_Module1 = true;
    bool RUN_Module2 = true;
    bool RUN_Module3 = true;
    bool RUN_Module4 = true;
    bool RUN_Module5 = true;
    bool RUN_Module6 = true;

    /* config params */
    double real_capacity;
    double real_pa;
    double real_sa;
    double real_openings[4];
    double real_push_vals[4];
    double real_pause_vals[4];
    double real_ratio_sa_front;
    double real_ratio_sa_rear;

    /* Fuel adjustment parameters */
    double Delta_FU_PUSH;
    double Delta_FU_PAUSE;

    /* PA adjustment parameters */
    double DELTA_PA;
    double NEXT_PA;

    /* SA adjustment parameters */
    double DELTA_SA;
    double NEXT_SA;

    /* OPENINGs adjustment parameters */
    double adjust_vals[NUM_GATE];
    double NEXT_values[NUM_GATE];

    /* Delta_PUSH_VALUEs Delta_PAUSE_VALUEsadjustment parameters */
    double adj_PUSH_VALUEs[NUM_GATE_VEL];
    double NEXT_PUSH_values[NUM_GATE_VEL];
    double adj_PAUSE_VALUEs[NUM_GATE_VEL];
    double NEXT_PAUSE_values[NUM_GATE_VEL];

    /* */
    double DELTA_SA_RATIO_FRONT;
    double NEXT_SA_RATIO_FRONT;
    double DELTA_SA_RATIO_REAR;
    double NEXT_SA_RATIO_REAR;

    /* add qcharts */

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

    /**
     * get control module
     */
    void get_control_module_status();

    /**
     * get control params
     */
    void get_config_params();

    /**
     * run stove calc
     */
    void run_stove_calc();

    /**
     * calc_adjust_input_fuel
     */
    void calc_adjust_input_fuel();
    double calc_fuel_input_k_capacity_values();
    void calc_fuel_input_k_tbed_values(double *k);
    void calc_fuel_input_k_to2_values(double *k);


    /**
     * calc adjust total pa
     */
    void calc_adjust_total_pa();
    void calc_total_pa_k_o2_vals(double *input, double *k);
    void calc_total_pa_k_tbd_vals(double *k);

    /**
     * calc adjust total sa
     */
    void calc_adjust_total_sa();
    void calc_total_sa_k_tfur_vals(double *k);
    void calc_total_sa_k_o2_vals(double *k);


    /**
     * calc adjust delta_pa_ratio
     */
    void calc_pa_dis_adjust_vals();
    void calc_target_pa_disribution(double *k);
    void calc_pa_disribution_tbed_vals(double *k);

    /**
     * calc adjust grate_vel
     */
    void calc_grate_vel_adjust_vals();
    void calc_target_grate_move_vals( double *push_vals,  double *pause_vals);
    void calc_grate_adj_vals(double *push_adratio, double *pause_adratio);

    /**
     * calc adjust DELTA_SA_RATIO_Front, DELTA_SA_RATIO_Front
     */
    void calc_adjust_sa_distribution_ratio();
    void calc_sa_distribution_k_tfur_vals(double *kfront, double *krear);

    /**
     * Plotting primary fan frequency comparisons
     */

    void plot_prim_fan_bar_chart();

    /**
     * Plotting secondary fan frequency comparisons
     */

    void plot_second_fan_bar_chart();

    /**
     * Plotting Pusher motor frequency comparisons
     */

    void plot_push_motor_bar_chart();

    /**
     * Plotting Pusher Air silo opening comparisons
     */

    void plot_pusher_air_silo_bar_chart();

    /**
     * Plotting Pusher motor intermittent comparisons
     */

    void plot_pusher_motor_inter_bar_chart();

    /**
     * Plotting sa ratio front comparisons
     */

    void plot_sa_ratio_front_bar_chart();

    /**
     * Plotting sa ratio rear comparisons
     */

    void plot_sa_ratio_rear_bar_chart();

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

    a = ui.action;
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
    QBarSet *set0 = new QBarSet("目标值");
    QBarSet *set1 = new QBarSet("实际值");

    *set0 << 4 ;// Jane 6个月份的值
    *set1 << 5 ;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->setBarWidth(0.5);

    // 增加顶部提示
    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);    //往Chart里添加series
    // chart->setTitle("Simple barchart example");  //设置表格头
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    // 创建X轴底部提示
    QStringList categories;
    categories << "周一";

    // QBarCategoryAxis *axis = new QBarCategoryAxis();

    // axis->append(categories);
    // chart->createDefaultAxes();
    // chart->addAxis(axis, Qt::AlignBottom);
    // chart->setAxisX(axis, series);
    int x = ui.graphicsView->x();
    int y = ui.graphicsView->y();
    int w = ui.graphicsView->width();
    int h = ui.graphicsView->height();
    // chart->resize(w,h);
    qDebug("w: %d, h: %d\n", w , h);

    // QStringList categories;  //创建字符串List
    // categories << "Jan";
    QBarCategoryAxis *axis = new QBarCategoryAxis();   //实例化坐标轴文字标识轴
    // axis->append(categories);   //将字符串列表append到文字标识轴里
    chart->createDefaultAxes();//创建默认的坐标轴（笛卡尔坐标），一定要有这一句
    chart->setAxisX(axis, series);//设置坐标轴
    chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 10);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    // chart->legend()->setGeometry(QRectF(x + w, y + h, 200, 200));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    chart->legend()->update();
    // chart->legend()->setPos(x,y);

    // 将参数设置到画布
    ui.graphicsView->setChart(chart);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView->chart()->setTheme(QChart::ChartTheme(0));
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

//============================================================================
void MainWindowPrivate::get_config_params()
{
    real_capacity = ui.doubleSpinBox_9->value();
    real_pa = ui.doubleSpinBox_10->value();
    real_sa = ui.doubleSpinBox_11->value();

    real_ratio_sa_front = ui.doubleSpinBox_15->value();
    real_ratio_sa_rear = ui.doubleSpinBox_16->value();

    real_push_vals[0] = ui.doubleSpinBox_24->value();
    real_push_vals[1] = ui.doubleSpinBox_25->value();
    real_push_vals[2] = ui.doubleSpinBox_26->value();
    real_push_vals[3] = ui.doubleSpinBox_27->value();

    real_pause_vals[0] = ui.doubleSpinBox_28->value();
    real_pause_vals[1] = ui.doubleSpinBox_29->value();
    real_pause_vals[2] = ui.doubleSpinBox_30->value();
    real_pause_vals[3] = ui.doubleSpinBox_31->value();

    real_openings[0] = ui.doubleSpinBox_32->value();
    real_openings[1] = ui.doubleSpinBox_33->value();
    real_openings[2] = ui.doubleSpinBox_34->value();
    real_openings[3] = ui.doubleSpinBox_35->value();
}

//============================================================================
void MainWindowPrivate::get_control_module_status()
{
    /* 进料速度控制 */
    if ( ui.checkBox->isChecked() == true )
        RUN_Module1 = true;
    else
        RUN_Module1 = false;

    /* 一次风量控制 */
    if ( ui.checkBox_2->isChecked() == true )
        RUN_Module2 = true;
    else
        RUN_Module2 = false;

    /* 二次风量控制 */
    if ( ui.checkBox_3->isChecked() == true )
        RUN_Module3 = true;
    else
        RUN_Module3 = false;

    /* 一次风门控制 */
    if ( ui.checkBox_4->isChecked() == true )
        RUN_Module4 = true;
    else
        RUN_Module4 = false;

    /* 炉排运动控制 */
    if ( ui.checkBox_5->isChecked() == true )
        RUN_Module5 = true;
    else
        RUN_Module5 = false;

    /* 二次风前后墙控制 */
    if ( ui.checkBox_6->isChecked() == true )
        RUN_Module6 = true;
    else
        RUN_Module6 = false;
}

//============================================================================
double MainWindowPrivate::calc_fuel_input_k_capacity_values()
{
    double k_1 = 0;
    if (real_capacity > 1.25 * Target_Capacity)
        k_1 = -1.5;
    else if (real_capacity > 1.10 * Target_Capacity)
         k_1 = -1.0;
    else if (real_capacity < 0.9 * Target_Capacity)
        k_1 = 1.0;
    else
        k_1 = 0.0;

    return k_1;
}

void MainWindowPrivate::calc_fuel_input_k_tbed_values(double *k)
{
    double Ave_T_Bed_1, Ave_T_Bed_2;

    Ave_T_Bed_1 = (t_bed_left_1 + t_bed_right_1) / 2;
    Ave_T_Bed_2 = (t_bed_left_2 + t_bed_right_2) / 2;

    /* K_1 对应【连续进料模式】 */
    if (Ave_T_Bed_1 > 1.20 * Target_T_Bed_Mean)
        k[0] = 0.2;
    else if (Ave_T_Bed_2 < 0.3)
        k[0] = 0.65;
    else
        k[0] = 0.35;

    /* K_2 对应【间歇进料模式】 */
    if (Ave_T_Bed_1 > 1.20 * Target_T_Bed_Mean)
        k[1] = 1.5;
    else if (Ave_T_Bed_2 < 0.3)
        k[1] = 0.65;
    else
        k[1] = 1;
}

void MainWindowPrivate::calc_fuel_input_k_to2_values(double *k)
{
    /* K_1 对应【连续进料模式】 */
    if ((Real_Oxy_Out > 1.2 * Target_Oxy_Out) && (real_capacity < 0.9 * Target_Capacity))
        k[0] = 1.15;
    else if ((Real_Oxy_Out < 0.8 * Target_Oxy_Out) && (real_capacity > 1.1 * Target_Capacity))
        k[0] = 0.85;
    else
        k[0] = 1.0;

    /* K_2 对应【间歇进料模式】 */
    if ((Real_Oxy_Out > 1.20 * Target_Oxy_Out) && (real_capacity < 0.9 * Target_Capacity))
        k[1] = 0.75;
    else if ((Real_Oxy_Out < 0.80 * Target_Oxy_Out) && (real_capacity > 1.1 * Target_Capacity))
        k[1] = 1.25;
    else
        k[1] = 1.0;
}

void MainWindowPrivate::calc_adjust_input_fuel()
{
    double k_capacity;
    double k_t_bed[2];
    double k_o2[2];
    double Delta_FU_PUSH;


    k_capacity = calc_fuel_input_k_capacity_values();
    calc_fuel_input_k_tbed_values(k_t_bed);
    calc_fuel_input_k_to2_values(k_o2);

    Delta_FU_PUSH = Delta_FU_PUSH_default * k_capacity * k_t_bed[0] * k_o2[0];
    Delta_FU_PAUSE = FU_PAUSE_default * k_t_bed[1] * k_o2[1];

    Delta_FU_PUSH = qRound(Delta_FU_PUSH * 1e4) / 1e4f;
    Delta_FU_PAUSE = qRound(Delta_FU_PAUSE * 1e4) / 1e4f;
}

void MainWindowPrivate::calc_total_pa_k_o2_vals(double *input, double *k)
{
    if ((Real_Oxy_Out > 1.2 * Target_Oxy_Out) && (*input > 0))
        *k = 0.75;
    else if ((Real_Oxy_Out < 0.8 * Target_Oxy_Out) && (*input < 0))
        *k =0.75;
    else
        *k = 1.0;
}

void MainWindowPrivate::calc_total_pa_k_tbd_vals(double *k)
{
    double Ave_T_Bed_1 = (t_bed_left_1 + t_bed_right_1) / 2;
    double Ave_T_Bed_2 = (t_bed_left_2 + t_bed_right_2) / 2;

    if (Ave_T_Bed_1 < Target_T_Bed_Mean * 0.75)
        *k = -1.5;
    else if (Ave_T_Bed_1 < Target_T_Bed_Mean * 0.9)
        *k = -1.0;
    else if (Ave_T_Bed_2 < Target_T_Bed_Mean * 0.75)
        *k = -1.0;
    else if (Ave_T_Bed_2 > Target_T_Bed_Mean * 1.25)
        *k = 1.0;
    else
        *k = 0.0;
}

void MainWindowPrivate::calc_adjust_total_pa()
{
    double k_t_bed, k_o2;
    calc_total_pa_k_tbd_vals(&k_t_bed);
    calc_total_pa_k_o2_vals(&k_t_bed, &k_o2);

    DELTA_PA = Delta_PA_default * k_t_bed * k_o2;
    NEXT_PA = real_pa + DELTA_PA;

    if ((NEXT_PA < Total_PA * 0.3) || (NEXT_PA > Total_PA * 0.90)) {
        DELTA_PA = 0.0;
        NEXT_PA = real_pa;
    }
}

void MainWindowPrivate::calc_total_sa_k_o2_vals(double *k)
{
    if (Real_Oxy_Out > 1.5 * Target_Oxy_Out)
        *k = -1.5;
    else if (Real_Oxy_Out > 1.2 * Target_Oxy_Out)
        *k = -1.0;
    else if (Real_Oxy_Out < 0.85 * Target_Oxy_Out)
        *k = 1.0;
    else if (Real_Oxy_Out < 0.70 * Target_Oxy_Out)
        *k = 1.5;
    else
        *k = 0.0;
}

void MainWindowPrivate::calc_total_sa_k_tfur_vals(double *k)
{
    double Ave_T_fur_MID, Ave_T_fur_TOP;
    Ave_T_fur_MID = (t_fur_left_mid + t_fur_right_mid) / 2;
    Ave_T_fur_TOP = (t_fur_left_top + t_fur_right_top) / 2;

    if (Real_Oxy_Out < 0.8 * Target_Oxy_Out) {
        if ((Ave_T_fur_MID > 1.1 * Target_T_Fur_MID) || (Ave_T_fur_TOP > 1.1 * Target_T_Fur_TOP)) {
            *k = 1.15;
        } else if ((Ave_T_fur_MID < 0.9 * Target_T_Fur_MID) || (Ave_T_fur_TOP < 0.9 * Target_T_Fur_TOP)) {
            *k = 0.85;
        } else {
            *k = 1.0;
        }
    } else if (Real_Oxy_Out > 1.20 * Target_Oxy_Out) {
        if ((Ave_T_fur_MID > 1.1 * Target_T_Fur_MID) || (Ave_T_fur_TOP > 1.1 * Target_T_Fur_TOP)) {
            *k = 0.85;
        } else if ((Ave_T_fur_MID < 0.9 * Target_T_Fur_MID) || (Ave_T_fur_TOP < 0.9 * Target_T_Fur_TOP)) {
            *k = 1.15;
        } else {
            *k = 1.0;
        }
    } else {
        *k = 1.0;
    }
}

void MainWindowPrivate::calc_adjust_total_sa()
{
    double k_t_fur, k_o2;
    calc_total_sa_k_tfur_vals(&k_t_fur);
    calc_total_sa_k_o2_vals(&k_o2);
    qDebug("k_t_fur: %f, k_o2: %f\n", k_t_fur, k_o2);
    DELTA_SA = qRound(Delta_SA_default * k_t_fur * k_o2 * 1e4) / 1e4f;
    NEXT_SA = real_sa + DELTA_SA;

    if ((NEXT_SA < Total_SA * 0.1) || (NEXT_SA > Total_SA * 0.9)) {
        DELTA_SA = 0.0;
        NEXT_SA = real_sa;
    }

}

void MainWindowPrivate::calc_pa_disribution_tbed_vals(double *k)
{
    double Ave_T_Bed_1;
    double Ave_T_Bed_2;

    if (NUM_GATE == 3) {
        std::memcpy(k, ini_ratios_1, NUM_GATE * sizeof(double));
    } else if (NUM_GATE == 4) {
        std::memcpy(k, ini_ratios_2, NUM_GATE * sizeof(double));
    } else if (NUM_GATE == 5) {
        std::memcpy(k, ini_ratios_3, NUM_GATE * sizeof(double));
    } else {
        qDebug("风门数量设置不在系统范围内: %d, 应为3、4或5", NUM_GATE);
    }

    Ave_T_Bed_1 = (t_bed_left_1 + t_bed_right_1) / 2;
    Ave_T_Bed_2 = (t_bed_left_2 + t_bed_right_2) / 2;

    if ((Ave_T_Bed_1 < Target_T_Bed_Mean * 0.75) || (Ave_T_Bed_1 > Target_T_Bed_Mean * 1.25)) {
        k[0] = 0.35;
        if (NUM_GATE > 3) {
            k[1] = 0.35;
        }
    } else if ((Ave_T_Bed_1 < Target_T_Bed_Mean * 0.85) || (Ave_T_Bed_1 > Target_T_Bed_Mean * 1.15)) {
        k[0] = 0.35;
        if (NUM_GATE > 3) {
            k[1] = 0.55;
        }
    }

    if ((Ave_T_Bed_2 < Target_T_Bed_Mean * 0.75) || (Ave_T_Bed_2 > Target_T_Bed_Mean * 1.25)) {
        k[NUM_GATE - 1] = 0.35;
        if (NUM_GATE > 3) {
            k[NUM_GATE - 2] = 0.35;
        }
    } else if ((Ave_T_Bed_2 < Target_T_Bed_Mean * 0.85) || (Ave_T_Bed_2 > Target_T_Bed_Mean * 1.15)) {
        k[NUM_GATE - 1] = 0.55;
        if (NUM_GATE > 3) {
            k[NUM_GATE - 2] = 0.55;
        }
    }
}

void MainWindowPrivate::calc_target_pa_disribution(double *k)
{
    double load_percentage = real_capacity / Target_Capacity;
    if (NUM_GATE == 3) {
        if (load_percentage > 0.7) {
            std::memcpy(k, openings_1, NUM_GATE * sizeof(double));
        } else {
            std::memcpy(k, openings_2, NUM_GATE * sizeof(double));
        }
    } else if (NUM_GATE == 4){
        if (load_percentage > 0.7) {
            std::memcpy(k, openings_3, NUM_GATE * sizeof(double));
        } else {
            std::memcpy(k, openings_4, NUM_GATE * sizeof(double));
        }
    } else if (NUM_GATE == 5){
        if (load_percentage > 0.7) {
            std::memcpy(k, openings_5, NUM_GATE * sizeof(double));
        } else {
            std::memcpy(k, openings_6, NUM_GATE * sizeof(double));
        }
    } else {
        qDebug("风门数量设置不在系统范围内: %d，应为3、4或5\n", NUM_GATE);
    }
}

void MainWindowPrivate::calc_pa_dis_adjust_vals()
{
    double target_opening[NUM_GATE];
    double adjust_ratios[NUM_GATE];

    calc_target_pa_disribution(target_opening);
    calc_pa_disribution_tbed_vals(adjust_ratios);

    for(int i = 0; i < NUM_GATE; i++) {
        qDebug("target_opening[i]: %f\n", target_opening[i]);
        qDebug("adjust_ratios[i]: %f\n", adjust_ratios[i]);
    }

    for(int i = 0; i < NUM_GATE; i ++)
    {
        adjust_vals[i] = (target_opening[i] - real_openings[i]) * adjust_ratios[i];
        adjust_vals[i] = qRound(adjust_vals[i] * 1e4) / 1e4f;
        NEXT_values[i] = qRound((real_openings[i] + adjust_vals[i]) * 1e2) / 1e2f;
    }
}

void MainWindowPrivate::calc_target_grate_move_vals(double * push_vals, double * pause_vals)
{
    qDebug() << "@@@@@@@@@@@@@@@\n";
    if (NUM_GATE_VEL == 3) {
        std::memcpy(push_vals, ini_PUSH_VALUEs_1, NUM_GATE * sizeof(double));
        std::memcpy(pause_vals, ini_PAUSE_VALUEs_1, NUM_GATE * sizeof(double));
    } else if (NUM_GATE_VEL == 4) {
        qDebug() << "@@@@@@@@@@@@@@@\n";
        std::memcpy(push_vals, ini_PUSH_VALUEs_2, NUM_GATE * sizeof(double));
        std::memcpy(pause_vals, ini_PAUSE_VALUEs_2, NUM_GATE * sizeof(double));
    } else if (NUM_GATE_VEL == 5) {
        std::memcpy(push_vals, ini_PUSH_VALUEs_3, NUM_GATE * sizeof(double));
        std::memcpy(pause_vals, ini_PAUSE_VALUEs_3, NUM_GATE * sizeof(double));
    } else {
        qDebug("炉排控速段数量设置不在系统范围内: %d, 应为3、4或5\n", NUM_GATE_VEL);
    }
    qDebug() << "#$$$$$$$$$$$$$$$$$$$$$$$\n";
    qDebug() << *push_vals;
    qDebug() << *pause_vals;
    for(int i = 0; i < NUM_GATE; i++) {
        qDebug("push_vals[%d]: %f\n", i, push_vals[i]);
        qDebug("pause_vals[%d]: %f\n", i, push_vals[i]);
    }
}

void MainWindowPrivate::calc_grate_adj_vals(double *push_adratio, double *pause_adratio)
{
    double Ave_T_Bed_1, Ave_T_Bed_2;
    if (NUM_GATE_VEL == 3) {
        std::memcpy(push_adratio, ini_PUSH_AdRatio_1, NUM_GATE * sizeof(double));
        std::memcpy(pause_adratio, ini_PAUSE_AdRatio_1, NUM_GATE * sizeof(double));
    } else if (NUM_GATE_VEL == 4) {
        std::memcpy(push_adratio, ini_PUSH_AdRatio_2, NUM_GATE * sizeof(double));
        std::memcpy(pause_adratio, ini_PAUSE_AdRatio_2, NUM_GATE * sizeof(double));
    } else if (NUM_GATE_VEL == 5) {
        std::memcpy(push_adratio, ini_PUSH_AdRatio_3, NUM_GATE * sizeof(double));
        std::memcpy(pause_adratio, ini_PAUSE_AdRatio_3, NUM_GATE * sizeof(double));
    } else {
        qDebug("炉排控速段数量设置不在系统范围内: %d, 应为3、4或5\n", NUM_GATE_VEL);
    }

    Ave_T_Bed_1 = (t_bed_left_1 + t_bed_right_1) / 2;
    Ave_T_Bed_2 = (t_bed_right_2 + t_bed_right_2) / 2;

    if (Ave_T_Bed_1 < Target_T_Bed_Mean * 0.85) {
        push_adratio[0] = 0.80;
        pause_adratio[0] = 1.20;
        if (NUM_GATE_VEL > 3) {
            push_adratio[1] = 0.90;
            pause_adratio[1] = 1.10;
        }
    } else if (Ave_T_Bed_1 > Target_T_Bed_Mean * 1.25) {
        push_adratio[0] = 1.20;
        pause_adratio[0] = 0.8;
        if (NUM_GATE_VEL > 3) {
            push_adratio[1] = 1.10;
            pause_adratio[1] = 0.90;
        }
    }

    if (Ave_T_Bed_2 < Target_T_Bed_Mean * 0.85) {
        push_adratio[NUM_GATE_VEL - 1] = 1.20;
        pause_adratio[NUM_GATE_VEL - 1] = 0.8;
        if (NUM_GATE_VEL > 3) {
            push_adratio[NUM_GATE_VEL - 2] = 1.10;
            pause_adratio[NUM_GATE_VEL - 2] = 0.90;
        }
    } else if (Ave_T_Bed_2 > Target_T_Bed_Mean * 1.25) {
        push_adratio[NUM_GATE_VEL - 1] = 0.8;
        pause_adratio[NUM_GATE_VEL - 1] = 1.20;
        if (NUM_GATE_VEL > 3) {
            push_adratio[NUM_GATE_VEL - 2] = 0.9;
            pause_adratio[NUM_GATE_VEL - 2] = 1.10;
        }
    }
}

void MainWindowPrivate::calc_grate_vel_adjust_vals()
{
    double *push_vals = new double[NUM_GATE];
    double *pause_vals = new double[NUM_GATE];
    double *push_adratio = new double[NUM_GATE];
    double *pause_adratio = new double[NUM_GATE];

    calc_target_grate_move_vals(push_vals, pause_vals);


    calc_grate_adj_vals(push_adratio, pause_adratio);

    for(int i = 0; i < NUM_GATE_VEL; i++) {
        adj_PUSH_VALUEs[i] = push_vals[i] * push_adratio[i] - real_push_vals[i];
        adj_PAUSE_VALUEs[i] = pause_vals[i] * pause_adratio[i] - real_pause_vals[i];
        NEXT_PUSH_values[i] = qRound((real_push_vals[i] + adj_PUSH_VALUEs[i]) * 1e4) / 1e4f;
        NEXT_PAUSE_values[i] = qRound((real_pause_vals[i] + adj_PAUSE_VALUEs[i]) * 1e4) / 1e4f;
    }
}

void MainWindowPrivate::calc_sa_distribution_k_tfur_vals(double *kfront, double *krear)
{
    double Ave_T_fur_LEFT, Ave_T_fur_RIGHT;
    Ave_T_fur_LEFT = (t_fur_left_mid + t_fur_left_top) / 2;
    Ave_T_fur_RIGHT = (t_fur_right_mid + t_fur_right_top) / 2;

    if ((Ave_T_fur_LEFT - Ave_T_fur_RIGHT) > 300 ) {
        *kfront = 1.5;
        *krear  = -1.5;
    } else if ((Ave_T_fur_LEFT - Ave_T_fur_RIGHT) > 200 ) {
        *kfront = 1.15;
        *krear  = -1.15;
    } else if ((Ave_T_fur_LEFT - Ave_T_fur_RIGHT) > 100 ) {
        *kfront = 0.75;
        *krear  = -0.75;
    } else if ((Ave_T_fur_LEFT - Ave_T_fur_RIGHT) < -100 ) {
        *kfront = -0.75;
        *krear  = 0.75;
    } else if ((Ave_T_fur_LEFT - Ave_T_fur_RIGHT) < -200 ) {
        *kfront = -1.15;
        *krear  = 1.15;
    } else if ((Ave_T_fur_LEFT - Ave_T_fur_RIGHT) < -300 ) {
        *kfront = -1.5;
        *krear  = 1.5;
    } else {
        *kfront = 0;
        *krear  = 0;
    }
}

void MainWindowPrivate::calc_adjust_sa_distribution_ratio()
{
    double kfront, krear;
    calc_sa_distribution_k_tfur_vals(&kfront, &krear);
    qDebug("kfront: %f, krear:%f\n",kfront, krear);
    DELTA_SA_RATIO_FRONT = qRound(default_RATIO_SA_STEP * kfront * 1e4) / 1e4f;
    DELTA_SA_RATIO_REAR = qRound(default_RATIO_SA_STEP * krear * 1e4) / 1e4f;

    if ((real_ratio_sa_front > 0.3 * default_RATIO_SA_Front) && (real_ratio_sa_front < 0.95)) {
        NEXT_SA_RATIO_FRONT = qRound((real_ratio_sa_front + DELTA_SA_RATIO_FRONT) * 1e4) / 1e4f;
    } else {
        DELTA_SA_RATIO_FRONT = 0.0;
        NEXT_SA_RATIO_FRONT = qRound(real_ratio_sa_front * 1e4) / 1e4f;
    }

    if ((real_ratio_sa_rear > 0.3 * default_RATIO_SA_Rear) && (real_ratio_sa_rear < 0.95)) {
        NEXT_SA_RATIO_REAR = qRound((real_ratio_sa_rear + DELTA_SA_RATIO_REAR) * 1e4) / 1e4f;
    } else {
        DELTA_SA_RATIO_REAR = 0.0;
        NEXT_SA_RATIO_REAR = qRound(real_ratio_sa_rear * 1e4) / 1e4f;
    }
}

void MainWindowPrivate::plot_prim_fan_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << qRound(real_pa * 1e2) / 1e2f;
    *set1 << qRound(NEXT_PA * 1e2) / 1e2f;

    series->append(set0);
    series->append(set1);
    series->setBarWidth(0.8);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    int x = ui.graphicsView->x();
    int y = ui.graphicsView->y();
    int w = ui.graphicsView->width();
    int h = ui.graphicsView->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 30);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(9);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0,0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 8);

    // 将参数设置到画布
    ui.graphicsView->setChart(chart);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView->chart()->setTheme(QChart::ChartTheme(0));
}

void MainWindowPrivate::plot_second_fan_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << qRound(real_sa * 1e2) / 1e2f;
    *set1 << qRound(NEXT_SA * 1e2) / 1e2f;;

    series->append(set0);
    series->append(set1);
    series->setBarWidth(0.8);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    QBarCategoryAxis *axis = new QBarCategoryAxis();

    chart->createDefaultAxes();

    chart->addAxis(axis, Qt::AlignBottom);
    chart->setAxisX(axis, series);
    chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 30);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    int x = ui.graphicsView_2->x();
    int y = ui.graphicsView_2->y();
    int w = ui.graphicsView_2->width();
    int h = ui.graphicsView_2->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(9);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0, 0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 2);

    // 将参数设置到画布
    ui.graphicsView_2->setChart(chart);
    ui.graphicsView_2->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_2->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView_2->chart()->setTheme(QChart::ChartTheme(0));
}

void MainWindowPrivate::plot_push_motor_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << real_push_vals[0] << real_push_vals[1] << real_push_vals[2] << real_push_vals[3];
    *set1 << NEXT_PUSH_values[0] << NEXT_PUSH_values[1] << NEXT_PUSH_values[2] << NEXT_PUSH_values[3];

    series->append(set0);
    series->append(set1);
    series->setBarWidth(1);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    // 创建X轴底部提示
    QStringList categories;
    categories << "1" << "2" << "3" << "4";

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();

    QFont font;
    font.setPixelSize(9);
    axis->setLabelsFont(font);

    chart->addAxis(axis, Qt::AlignBottom);
    chart->setAxisX(axis, series);
    // chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 30);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    int x = ui.graphicsView_3->x();
    int y = ui.graphicsView_3->y();
    int w = ui.graphicsView_3->width();
    int h = ui.graphicsView_3->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(8);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0, 0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 2);

    // 将参数设置到画布
    ui.graphicsView_3->setChart(chart);
    ui.graphicsView_3->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_3->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView_3->chart()->setTheme(QChart::ChartTheme(0));
}

void MainWindowPrivate::plot_pusher_air_silo_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << real_openings[0] << real_openings[1] << real_openings[2] << real_openings[3];
    *set1 << NEXT_values[0] << NEXT_values[1] << NEXT_values[2] << NEXT_values[3];

    series->append(set0);
    series->append(set1);
    series->setBarWidth(1);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    // 创建X轴底部提示
    QStringList categories;
    categories << "1" << "2" << "3" << "4";

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();

    QFont font;
    font.setPixelSize(9);
    axis->setLabelsFont(font);

    chart->addAxis(axis, Qt::AlignBottom);
    chart->setAxisX(axis, series);
    // chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 1);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    int x = ui.graphicsView_4->x();
    int y = ui.graphicsView_4->y();
    int w = ui.graphicsView_4->width();
    int h = ui.graphicsView_4->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(8);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0, 0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 2);

    // 将参数设置到画布
    ui.graphicsView_4->setChart(chart);
    ui.graphicsView_4->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_4->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView_4->chart()->setTheme(QChart::ChartTheme(0));
}

void MainWindowPrivate::plot_pusher_motor_inter_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << real_pause_vals[0] << real_pause_vals[1] << real_pause_vals[2] << real_pause_vals[3];
    *set1 << NEXT_PAUSE_values[0] << NEXT_PAUSE_values[1] << NEXT_PAUSE_values[2] << NEXT_PAUSE_values[3];

    series->append(set0);
    series->append(set1);
    series->setBarWidth(1);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    // 创建X轴底部提示
    QStringList categories;
    categories << "1" << "2" << "3" << "4";

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();

    QFont font;
    font.setPixelSize(9);
    axis->setLabelsFont(font);

    chart->addAxis(axis, Qt::AlignBottom);
    chart->setAxisX(axis, series);
    // chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 45);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    int x = ui.graphicsView_5->x();
    int y = ui.graphicsView_5->y();
    int w = ui.graphicsView_5->width();
    int h = ui.graphicsView_5->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(8);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0, 0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 2);

    // 将参数设置到画布
    ui.graphicsView_5->setChart(chart);
    ui.graphicsView_5->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_5->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView_5->chart()->setTheme(QChart::ChartTheme(0));
}

void MainWindowPrivate::plot_sa_ratio_front_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << qRound(real_ratio_sa_front * 1e2) / 1e2f;
    *set1 << qRound(NEXT_SA_RATIO_FRONT * 1e2) / 1e2f;;

    series->append(set0);
    series->append(set1);
    series->setBarWidth(0.7);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    QBarCategoryAxis *axis = new QBarCategoryAxis();

    chart->createDefaultAxes();

    chart->addAxis(axis, Qt::AlignBottom);
    chart->setAxisX(axis, series);
    chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 1);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    int x = ui.graphicsView_6->x();
    int y = ui.graphicsView_6->y();
    int w = ui.graphicsView_6->width();
    int h = ui.graphicsView_6->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(9);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    // 设置图例字体大小
    QFont legendFont = chart->legend()->font();
    legendFont.setPointSize(6); // 设置字体大小为12点
    chart->legend()->setFont(legendFont);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0, 0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 2);

    // 将参数设置到画布
    ui.graphicsView_6->setChart(chart);
    ui.graphicsView_6->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_6->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView_6->chart()->setTheme(QChart::ChartTheme(0));
}

void MainWindowPrivate::plot_sa_ratio_rear_bar_chart()
{
    QBarSet *set0 = new QBarSet("当前值");
    QBarSet *set1 = new QBarSet("目标值");

    QBarSeries *series = new QBarSeries();

    *set0 << qRound(real_ratio_sa_rear * 1e2) / 1e2f;
    *set1 << qRound(NEXT_SA_RATIO_REAR * 1e2) / 1e2f;;

    series->append(set0);
    series->append(set1);
    series->setBarWidth(0.7);

    QChart *chart = new QChart(); // 实例化QChart
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    QBarCategoryAxis *axis = new QBarCategoryAxis();

    chart->createDefaultAxes();

    chart->addAxis(axis, Qt::AlignBottom);
    chart->setAxisX(axis, series);
    chart->removeAxis(axis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 1);
    chart->setAxisY(yAxis);
    series->attachAxis(yAxis);
    chart->removeAxis(yAxis);

    int x = ui.graphicsView_7->x();
    int y = ui.graphicsView_7->y();
    int w = ui.graphicsView_7->width();
    int h = ui.graphicsView_7->height();
    qDebug("x: %d, y:%d, w: %d, h: %d\n",x, y, w , h);

    // 在每个柱形图上方显示数值
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase);
    series->setLabelsVisible(true);

    set0->setLabelColor(Qt::black);
    set1->setLabelColor(Qt::black);
    QFont font1;
    font1.setPixelSize(9);
    set0->setLabelFont(font1);
    set1->setLabelFont(font1);

    // 设置图例字体大小
    QFont legendFont = chart->legend()->font();
    legendFont.setPointSize(6); // 设置字体大小为12点
    chart->legend()->setFont(legendFont);

    chart->legend()->detachFromChart();
    chart->legend()->setVisible(true);
    chart->legend()->setGeometry(QRectF(0, 0, 100, 100));
    // chart->legend()->setAlignment(Qt::AlignLeft);
    // chart->legend()->update();
    // chart->legend()->setPos(x, y - h / 2);

    // 将参数设置到画布
    ui.graphicsView_7->setChart(chart);
    ui.graphicsView_7->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_7->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.graphicsView_7->chart()->setTheme(QChart::ChartTheme(0));
}


void MainWindowPrivate::run_stove_calc()
{
    qDebug() << "\n#----------------------------当前阶段，面板运行数值如下:-------------------------#";
    qDebug("实际负荷: %f t/h \t 一次风机实际频率: %f kHz \t 二次风机实际频率: %f kHz", real_capacity, real_pa, real_sa);
    qDebug("各炉排控速段的推料电机运行频率: 第一段: %f\t, 第二段： %f\t, 第三段： %f\t, 第四段： %f\t\n", real_push_vals[0], real_push_vals[1], \
           real_push_vals[2], real_push_vals[3]);
    qDebug("各炉排控速段的推料电机间歇时间: 第一段: %f\t, 第二段： %f\t, 第三段： %f\t, 第四段： %f\t\n", real_pause_vals[0], real_pause_vals[1], \
           real_pause_vals[2], real_pause_vals[3]);
    qDebug("各一次风仓实际开度: 第一段: %f\t, 第二段： %f\t, 第三段： %f\t, 第四段： %f\t\n", real_openings[0], real_openings[1], \
           real_openings[2], real_openings[3]);
    qDebug("前墙二次风开度: %f\t, 后墙二次风开度： %f\t\n", real_ratio_sa_front, real_ratio_sa_rear);

    qDebug() << "\n#----------------------------当前阶段，运行调整建议如下:-------------------------#\n";

    if (RUN_Module1 == true) {
        calc_adjust_input_fuel();
        if (FUEL_IN_MODE == 1) {
            if (Delta_FU_PUSH > 0) {
                qDebug("给料提示：当前推料电机频率应调大 %f khz！\n", abs(Delta_FU_PUSH));
            } else if (Delta_FU_PUSH < 0) {
                qDebug("给料提示：当前推料电机频率应调小 %f khz！\n", abs(Delta_FU_PUSH));
            } else {
                qDebug("给料提示：当前推料电机频率暂勿需调整\n");
            }
        } else if (FUEL_IN_MODE == 2) {
            qDebug("给料提示：当前推料间歇时间应设置为 %f s！\n", Delta_FU_PAUSE);
        } else if (FUEL_IN_MODE == 3) {
            if (Delta_FU_PUSH > 0) {
                qDebug("给料提示：当前推料电机频率应调大 %f khz！\n", abs(Delta_FU_PUSH));
            } else if (Delta_FU_PUSH < 0) {
                qDebug("给料提示：当前推料电机频率应调小 %f khz！\n", abs(Delta_FU_PUSH));
            } else {
                qDebug("给料提示：当前推料电机频率暂勿需调整\n");
            }
        } else {
            qDebug("进料速度控制模块尚未激活!\n");
        }
    }

    if (RUN_Module2 == true) {
        calc_adjust_total_pa();
        if (real_pa > 1.05 * PA_MIN) {
            if (DELTA_PA > 0) {
                qDebug("一次风调整提示：当前一次风电机频率应调大 %f khz，当前目标值为 %f khz！\n", abs(DELTA_PA), NEXT_PA);
            } else if (DELTA_PA < 0) {
                qDebug("一次风调整提示：当前一次风电机频率应调小 %f khz，当前目标值为 %f khz！\n", abs(DELTA_PA), NEXT_PA);
            } else {
                qDebug("当前一次风电机频率无需调整！\n");
            }
        } else {
            qDebug("当前一次风点击频率已经接近下限！！！\n");
        }
    } else {
        qDebug("一次风量控制模块尚未激活!\n");
    }

    if (RUN_Module3 == true) {
        calc_adjust_total_sa();
        if (real_sa > 1.05 * SA_MIN) {
            if (DELTA_SA > 0) {
                qDebug("二次风调整提示：当前二次风电机频率应调大 %f khz，当前目标值为 %f khz！\n", abs(DELTA_SA), NEXT_SA);
            } else if (DELTA_SA < 0) {
                qDebug("二次风调整提示：当前二次风电机频率应调小 %f khz，当前目标值为 %f khz！\n", abs(DELTA_SA), NEXT_SA);
            } else {
                    qDebug("当前二次风电机频率无需调整！\n");
            }
        } else {
            qDebug("当前二次风点击频率已经接近下限！！！\n");
        }
    } else {
        qDebug("二次风量控制模块尚未激活!\n");
    }

    if (RUN_Module4 == true) {
        calc_pa_dis_adjust_vals();
        qDebug("各一次风仓建议开度：第一段: %f \t, 第二段: %f \t, 第三段: %f \t, 第四段: %f \t\n",\
               NEXT_values[0], NEXT_values[1], NEXT_values[2], NEXT_values[3]);
    } else {
        qDebug("一次风门控制模块尚未激活!\n");
    }

    if (RUN_Module5 == true) {
        calc_grate_vel_adjust_vals();
        qDebug("当前，各炉排控速段推料电机频率目标值为:\n");
        qDebug("第一段: %f \t, 第二段: %f \t, 第三段: %f \t, 第四段: %f \t\n",\
               NEXT_PUSH_values[0], NEXT_PUSH_values[1], NEXT_PUSH_values[2], NEXT_PUSH_values[3]);
        qDebug("当前，各炉排控速段推料电机运行间歇目标值为:\n");
        qDebug("第一段: %f \t, 第二段: %f \t, 第三段: %f \t, 第四段: %f \t\n",\
               NEXT_PAUSE_values[0], NEXT_PAUSE_values[1], NEXT_PAUSE_values[2], NEXT_PAUSE_values[3]);
    } else {
        qDebug("炉排运动控制控制模块尚未激活!\n");
    }

    if (RUN_Module6 == true) {
        calc_adjust_sa_distribution_ratio();
        if (DELTA_SA_RATIO_FRONT > 0) {
            qDebug("当前，前墙二次风门开度应调大 %f khz，当前目标值为 %fkhz！\n", abs(DELTA_SA_RATIO_FRONT), abs(NEXT_SA_RATIO_FRONT));
        } else if (DELTA_SA_RATIO_FRONT < 0) {
            qDebug("当前，前墙二次风门开度应调小 %f khz，当前目标值为 %fkhz！\n", abs(DELTA_SA_RATIO_FRONT), abs(NEXT_SA_RATIO_FRONT));
        } else {
            qDebug("当前前墙二次风门开度无需调整！\n");
        }

        if (DELTA_SA_RATIO_REAR > 0) {
            qDebug("当前，前墙二次风门开度应调大 %f khz，当前目标值为 %fkhz！\n", abs(DELTA_SA_RATIO_REAR), abs(NEXT_SA_RATIO_REAR));
        } else if (DELTA_SA_RATIO_REAR < 0) {
            qDebug("当前，前墙二次风门开度应调小 %f khz，当前目标值为 %fkhz！\n", abs(DELTA_SA_RATIO_REAR), abs(NEXT_SA_RATIO_REAR));
        } else {
            qDebug("当前后墙二次风门开度无需调整！\n");
        }
    } else {
        qDebug("二次风前后墙控制模块尚未激活!\n");
    }

}

Stove_Navigator::Stove_Navigator(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(this))
{
    d->ui.setupUi(this);

    d->createActions();

    // d->createContent();

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


    d->get_control_module_status();
    d->get_config_params();
    d->run_stove_calc();

    /* plot bar chart */
    d->plot_prim_fan_bar_chart();
    d->plot_second_fan_bar_chart();
    d->plot_pusher_air_silo_bar_chart();
    d->plot_push_motor_bar_chart();
    d->plot_pusher_motor_inter_bar_chart();
    d->plot_sa_ratio_front_bar_chart();
    d->plot_sa_ratio_rear_bar_chart();
}
void Stove_Navigator::ui_slot_initial_params()
{
    qDebug() << "Stove_Navigator::init default params\n";
}

