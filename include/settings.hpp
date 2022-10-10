#include "macro.hpp"

// User settings
#define REPORT_STATUS DISABLE

#define PULSEWIDTH_US 1000

#define ENABLE_V1 ENABLE
#define ENABLE_V2 ENABLE
#define ENABLE_V3 ENABLE
#define ENABLE_V4 ENABLE
#define ENABLE_V5 ENABLE
#define ENABLE_V6 ENABLE
#define ENABLE_V7 ENABLE
#define ENABLE_V8 ENABLE

#define ENABLE_LIMIT_SWITCH_V5 DISABLE
#define ENABLE_SUPPORT_A_TEAM DISABLE

#define PWM_PIN_V1 PC_9
#define PWM_PIN_V2 PB_8 
#define PWM_PIN_V3 PB_9
#define PWM_PIN_V4 PA_6
#define PWM_PIN_V5 PB_6
#define PWM_PIN_V6 PC_7
#define PWM_PIN_V7 PA_9
#define PWM_PIN_V8 PA_8

#define DIGITAL_PIN_V1 PA_5
#define DIGITAL_PIN_V2 PA_7
#define DIGITAL_PIN_V3 PC_8
#define DIGITAL_PIN_V4 PC_6
#define DIGITAL_PIN_V5 PC_5
#define DIGITAL_PIN_V6 PA_12
#define DIGITAL_PIN_V7 PA_11
#define DIGITAL_PIN_V8 PB_12


// Warning! : don't attach pwm pin.

#define LIMIT_UP_V5 PC_0
#define LIMIT_DOWN_V5 PC_1

#define LIMIT_BACK_A_TEAM PA_4
/*
--- TEST DATA ---
M:1:0200:1:0200:1:0300:1:0400:E
M:0:500:0:700:0:100:0:300:E
M:0:500:1:700:0:100:1:300:0:160:1:300:0:940:1:777:E

M:1:650:0:0:1:0:0:0:E
M:1:0:0:650:1:0:0:0:E
M:0:0:1:0:0:650:1:0:E
M:0:0:1:0:0:0:1:650:E

M:0:0:1:0:0:0:1:650:0:0:E

*/