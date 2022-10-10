#include "DigitalIn.h"
#include "InterruptIn.h"
#include "PinNames.h"
#include "PinNamesTypes.h"
#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "macro.hpp"
#include "stm32f4xx.h"
#include "utils.hpp"
#include "settings.hpp"

static BufferedSerial serial_port(USBTX, USBRX);

// pin setup
#if ENABLE_V1
PwmOut V1_PWM(PWM_PIN_V1);
#endif
#if ENABLE_V2
PwmOut V2_PWM(PWM_PIN_V2);
#endif
#if ENABLE_V3
PwmOut V3_PWM(PWM_PIN_V3);
#endif
#if ENABLE_V4
PwmOut V4_PWM(PWM_PIN_V4);
#endif
#if ENABLE_V5
PwmOut V5_PWM(PWM_PIN_V5);
#endif
#if ENABLE_V6
PwmOut V6_PWM(PWM_PIN_V6);
#endif
#if ENABLE_V7
PwmOut V7_PWM(PWM_PIN_V7);
#endif
#if ENABLE_V8
PwmOut V8_PWM(PWM_PIN_V8);
#endif

#if ENABLE_V1
DigitalOut V1_Digital(DIGITAL_PIN_V1);
#endif
#if ENABLE_V2
DigitalOut V2_Digital(DIGITAL_PIN_V2);
#endif
#if ENABLE_V3
DigitalOut V3_Digital(DIGITAL_PIN_V3);
#endif
#if ENABLE_V4
DigitalOut V4_Digital(DIGITAL_PIN_V4);
#endif
#if ENABLE_V5
DigitalOut V5_Digital(DIGITAL_PIN_V5);

#endif
#if ENABLE_V6
DigitalOut V6_Digital(DIGITAL_PIN_V6);
#endif
#if ENABLE_V7
DigitalOut V7_Digital(DIGITAL_PIN_V7);
#endif
#if ENABLE_V8
DigitalOut V8_Digital(DIGITAL_PIN_V8);
#endif

#if ENABLE_LIMIT_SWITCH_V5
InterruptIn Pin_limit_up_v5(LIMIT_UP_V5);
InterruptIn Pin_limit_down_v5(LIMIT_DOWN_V5);

bool status_limit_up_v5 = BUTTON_RELEASE;
bool status_limit_down_v5 = BUTTON_RELEASE;

void 
callback_limit_up_v5_fall(){
    status_limit_up_v5 = BUTTON_PRESS;
    V5_PWM.pulsewidth_us(0);
}

void 
callback_limit_down_v5_fall(){
    status_limit_down_v5 = BUTTON_PRESS;
    V5_PWM.pulsewidth_us(0);
}

void
callback_limit_up_v5_rise(){
    status_limit_up_v5 = BUTTON_RELEASE;
}

void 
callback_limit_down_v5_rise(){
    status_limit_down_v5 = BUTTON_RELEASE;
}
#endif

#if ENABLE_SUPPORT_A_TEAM
InterruptIn Pin_limit_back_a_team(LIMIT_BACK_A_TEAM);

bool status_limit_back_a_team = BUTTON_PRESS;

void
callback_limit_back_a_team_fall(){
    status_limit_back_a_team = BUTTON_PRESS;
    V8_PWM.pulsewidth_us(0);
    V7_PWM.pulsewidth_us(0);
}

void
callback_limit_back_a_team_rise(){
    status_limit_back_a_team = BUTTON_RELEASE;
}
#endif

Ticker safeTimer;
bool safeFlag = false;
int16_t safeCounter = 0;

void safeCheck(){
    if(safeFlag){
        safeFlag = false;
        safeCounter = 0;
        return;
    }else{
        safeCounter++;
    }

    if(safeCounter > 1000){
        safeCounter = 0;
        safeFlag = false;
        
#if ENABLE_V1
          V1_PWM.pulsewidth_us(0);
          V1_Digital = 0;
#endif
#if ENABLE_V2
          V2_PWM.pulsewidth_us(0);
          V2_Digital = 0;
#endif
#if ENABLE_V3
          V3_PWM.pulsewidth_us(0);
          V3_Digital = 0;
#endif
#if ENABLE_V4
          V4_PWM.pulsewidth_us(0);
          V4_Digital = 0;
#endif
#if ENABLE_V5
          V5_PWM.pulsewidth_us(0);
          V5_Digital = 0;
#endif
#if ENABLE_V6
          V6_PWM.pulsewidth_us(0);
          V6_Digital = 0;
#endif
#if ENABLE_V7
          V7_PWM.pulsewidth_us(0);
          V7_Digital = 0;
#endif
#if ENABLE_V8
          V8_PWM.pulsewidth_us(0);
          V8_Digital = 0;
#endif
    }
}


int main() {
    safeTimer.attach(&safeCheck, 1ms);

  // Serial
  serial_port.set_baud(115200);
  serial_port.set_format(8, BufferedSerial::None, 1);
  char buf[MAXIMUM_BUFFER_SIZE] = {0};

  // PWM setup
#if ENABLE_V1
  V1_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V2
  V2_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V3
  V3_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V4
  V4_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V5
  V5_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V6
  V6_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V7
  V7_PWM.period_us(PULSEWIDTH_US);
#endif
#if ENABLE_V8
  V8_PWM.period_us(PULSEWIDTH_US);
#endif

#if ENABLE_LIMIT_SWITCH_V5
    Pin_limit_up_v5.mode(PullUp);
    Pin_limit_down_v5.mode(PullUp);

    Pin_limit_up_v5.fall(&callback_limit_up_v5_fall);
    Pin_limit_down_v5.fall(&callback_limit_down_v5_fall);

    Pin_limit_up_v5.rise(&callback_limit_up_v5_rise);
    Pin_limit_down_v5.rise(&callback_limit_down_v5_rise);
#endif

#if ENABLE_SUPPORT_A_TEAM
    Pin_limit_back_a_team.mode(PullUp);
    
    Pin_limit_back_a_team.fall(&callback_limit_back_a_team_fall);

    Pin_limit_back_a_team.rise(&callback_limit_back_a_team_rise);
#endif


  char pwrData[MAXIMUM_BUFFER_SIZE] = {0};
  uint32_t pdConter = 0;
  uint8_t indexCount = 0;

  while (1) {
    if (uint32_t num = serial_port.read(buf, sizeof(buf))) {
      for (uint32_t i = 0; i < num; i++) {
        if (buf[i] == 'M') {
          pdConter = 0;
          for (uint8_t j = 0; j < 32; j++) {
            pwrData[j] = 0;
          }

          pwrData[pdConter] = buf[i];
          pdConter++;
        } else if (buf[i] == '0' || buf[i] == '1' || buf[i] == '2' ||
                   buf[i] == '3' || buf[i] == '4' || buf[i] == '5' ||
                   buf[i] == '6' || buf[i] == '7' || buf[i] == '8' ||
                   buf[i] == '9') {
          pwrData[pdConter] = buf[i]; // add data.
          pdConter++;
        }else if(buf[i] == ':'){
            pwrData[pdConter] = buf[i]; // add data.
            pdConter++;
            indexCount++;
        }else if (buf[i] == 'E') {
            safeFlag = true;

            pwrData[pdConter] = buf[i]; // add data.

            int32_t* motorPower = pwrCreater(pwrData);
#if REPORT_STATUS
            serial_port.write(pwrData, strlen(pwrData));
            serial_port.write("\nBEGIN\n", 7);
            for(int16_t reportCounter = 0 ; reportCounter < MOTOR_COUNTS ; reportCounter++){
                char reportsStatusBuffer[32] = {0};

                if(*(motorPower + reportCounter * 2) == CW){
                    sprintf(reportsStatusBuffer, "V%d:%d:CW\n" , reportCounter , *(motorPower + reportCounter * 2 + 1));
                }else if(*(motorPower + reportCounter * 2) == CCW){
                    sprintf(reportsStatusBuffer, "V%d:%d:CCW\n" , reportCounter , *(motorPower + reportCounter * 2 + 1));
                }

                serial_port.write(reportsStatusBuffer, strlen(reportsStatusBuffer));
            }
            serial_port.write("END\n", 5);
#endif
        
#if ENABLE_V1
          V1_PWM.pulsewidth_us(*(motorPower + 1));
          V1_Digital = *(motorPower + 0);
#endif

#if ENABLE_V2
          V2_PWM.pulsewidth_us(*(motorPower + 3));
          V2_Digital = *(motorPower + 2);
#endif

#if ENABLE_V3
          V3_PWM.pulsewidth_us(*(motorPower + 5));
          V3_Digital = *(motorPower + 4);
#endif

#if ENABLE_V4
          V4_PWM.pulsewidth_us(*(motorPower + 7));
          V4_Digital = *(motorPower + 6);
#endif

#if ENABLE_LIMIT_SWITCH_V5
if(*(motorPower + 8)){
    // CW
    if(status_limit_up_v5 == BUTTON_RELEASE){
        V5_PWM.pulsewidth_us(*(motorPower + 9));
        V5_Digital = *(motorPower + 8);
    }
}else{
    // CCW
    if(status_limit_down_v5 == BUTTON_RELEASE){
        V5_PWM.pulsewidth_us(*(motorPower + 9));
        V5_Digital = *(motorPower + 8);
    }
}
#elif ENABLE_V5
    V5_PWM.pulsewidth_us(*(motorPower + 9));
    V5_Digital = *(motorPower + 8);
#endif

#if ENABLE_SUPPORT_A_TEAM
if (*(motorPower + 10) == 1 &&
    status_limit_back_a_team == BUTTON_RELEASE){

    V8_PWM.pulsewidth_us(*(motorPower + 11));
    V8_Digital = *(motorPower + 10);
    V7_PWM.pulsewidth_us(*(motorPower + 11));
    V7_Digital = *(motorPower + 10);

}else{
    V8_PWM.pulsewidth_us(*(motorPower + 11));
    V8_Digital = *(motorPower + 10);
    V7_PWM.pulsewidth_us(*(motorPower + 11));
    V7_Digital = *(motorPower + 10);
}
#endif

#if ENABLE_V6
          V6_PWM.pulsewidth_us(*(motorPower + 11));
          V6_Digital = *(motorPower + 10);
#endif

#if ENABLE_SUPPORT_A_TEAM

#elif ENABLE_V7
          V7_PWM.pulsewidth_us(*(motorPower + 13));
          V7_Digital = *(motorPower + 12);
#endif

#if ENABLE_SUPPORT_A_TEAM

#elif ENABLE_V8
          V8_PWM.pulsewidth_us(*(motorPower + 15));
          V8_Digital = *(motorPower + 14);
#endif

          pdConter = 0;
          indexCount = 0;
          for (int16_t j = 0; j < MAXIMUM_BUFFER_SIZE; j++) {
            pwrData[j] = 0;
          }
        }
      }
    }
  }
}