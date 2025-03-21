﻿#ifndef PARAMS_H
#define PARAMS_H

#define NUM_GATE        4
#define NUM_GATE_VEL    4
double AVE_PUSH   =      15.0;
double AVE_PAUSE  =      20.0;

double Total_Capacity = 15;
double Target_Capacity = 15;

double Target_T_Bed_Mean = 700;
double Target_T_Fur_MID = 1000;
double Target_T_Fur_TOP = 900;
double Target_Oxy_Out = 0.030;
double Total_PA = 25.0;
double PA_MIN = 10.0;
double Delta_PA_default = 0.50;
double Total_SA = 25.0;
double SA_MIN = 5.0;
double Delta_SA_default = 0.50;

double FUEL_IN_MODE = 1;
double Delta_FU_PUSH_default = 0.50;
double FU_PAUSE_default = 30.0;

double default_RATIO_SA_Front = 0.75;
double default_RATIO_SA_Rear = 0.75;
double default_RATIO_SA_STEP = 0.05;

double Real_Oxy_Out = 0.055;

double openings_1[3] = {0.20, 0.55, 0.65};
double openings_2[3] = {0.50, 0.50, 0.50};
double openings_3[4] = {0.15, 0.45, 0.65, 0.75};
double openings_4[4] = {0.15, 0.45, 0.65, 0.75};
double openings_5[5] = {0.10, 0.40, 0.40, 0.50, 0.55};
double openings_6[5] = {0.50, 0.50, 0.50, 0.50, 0.50};

double ini_ratios_1[3] = {0.65, 0.65, 0.65};
double ini_ratios_2[4] = {0.65, 0.65, 0.65, 0.65};
double ini_ratios_3[5] = {0.65, 0.65, 0.65, 0.65, 0.65};

double ini_PUSH_AdRatio_1[3] = {1.0, 1.0, 1.0};
double ini_PUSH_AdRatio_2[4] = {1.0, 1.0, 1.0, 1.0};
double ini_PUSH_AdRatio_3[5] = {1.0, 1.0, 1.0, 1.0, 1.0};

double ini_PAUSE_AdRatio_1[3] = {1.0, 1.0, 1.0};
double ini_PAUSE_AdRatio_2[4] = {1.0, 1.0, 1.0, 1.0};
double ini_PAUSE_AdRatio_3[5] = {1.0, 1.0, 1.0, 1.0, 1.0};

double ini_PUSH_VALUEs_1[3] = {1.50*AVE_PUSH, AVE_PUSH, 0.50*AVE_PUSH};
double ini_PUSH_VALUEs_2[4] = {1.50*AVE_PUSH, 1.25*AVE_PUSH, 0.75*AVE_PUSH, 0.50*AVE_PUSH};
double ini_PUSH_VALUEs_3[5] = {1.50*AVE_PUSH, 1.25*AVE_PUSH, AVE_PUSH, 1.25*AVE_PUSH, 1.50*AVE_PUSH};

double ini_PAUSE_VALUEs_1[3] = {0.75*AVE_PAUSE, AVE_PAUSE, 1.25*AVE_PAUSE};
double ini_PAUSE_VALUEs_2[4] = {0.75*AVE_PAUSE, 0.85*AVE_PAUSE, 1.25*AVE_PAUSE, 1.50*AVE_PAUSE};
double ini_PAUSE_VALUEs_3[5] = {0.75*AVE_PAUSE, 0.85*AVE_PAUSE, AVE_PAUSE, 1.25*AVE_PAUSE, 1.50*AVE_PAUSE};

#endif // PARAMS_H
