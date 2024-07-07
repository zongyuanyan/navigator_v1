#ifndef PARAMS_H
#define PARAMS_H

bool DISABLE = true;
bool RUN_Module1 = true;
bool RUN_Module2 = true;
bool RUN_Module3 = true;
bool RUN_Module4 = true;
bool RUN_Module5 = true;
bool RUN_Module6 = true;

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
double NUM_GATE = 4;
double NUM_GATE_VEL = 4;
double AVE_PUSH = 15.0;
double AVE_PAUSE = 20.0;

double default_RATIO_SA_Front = 0.75;
double default_RATIO_SA_Rear = 0.75;
double default_RATIO_SA_STEP = 0.05;

#endif // PARAMS_H
