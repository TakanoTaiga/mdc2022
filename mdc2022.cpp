#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

#define MAXIMUM_BUFFER_SIZE 128

#define ENABLE 1
#define DISABLE 0

// User settings
#define REPORT_STATUS ENABLE

#define PULSEWIDTH_US 1000

#define ENABLE_V1 ENABLE
#define ENABLE_V2 ENABLE
#define ENABLE_V3 ENABLE
#define ENABLE_V4 ENABLE
#define ENABLE_V5 DISABLE
#define ENABLE_V6 DISABLE
#define ENABLE_V7 DISABLE
#define ENABLE_V8 DISABLE

#define PWM_PIN_V1 PB_8
#define PWM_PIN_V2 PB_9 
#define PWM_PIN_V3 PA_7 //PA_5 -> PA_7 //Connect PA_5 & PA_7
#define PWM_PIN_V4 PA_6
#define PWM_PIN_V5 
#define PWM_PIN_V6 
#define PWM_PIN_V7 
#define PWM_PIN_V8 

#define DIGITAL_PIN_V1 PC_9
#define DIGITAL_PIN_V2 PC_8
#define DIGITAL_PIN_V3 PC_6
#define DIGITAL_PIN_V4 PC_5
#define DIGITAL_PIN_V5 PA_6
#define DIGITAL_PIN_V6 
#define DIGITAL_PIN_V7 
#define DIGITAL_PIN_V8 

// User settings end

// test data
// M:1:700:0:500:1:300:0:100:E
// M:0:500:1:700:0:100:1:300:E

// M:1:650:0:0:1:0:0:0:E
// M:1:0:0:650:1:0:0:0:E
// M:0:0:1:0:0:650:1:0:E
// M:0:0:1:0:0:0:1:650:E

//M:0:0:1:0:0:0:1:650:0:0:E

#define CW 1
#define CCW 0

#define PWR_CREATER_BUFFER_SIZE 64
#define MOTOR_COUNTS (ENABLE_V1 + ENABLE_V2 + ENABLE_V3 + ENABLE_V4 + ENABLE_V5 + ENABLE_V6 + ENABLE_V7 + ENABLE_V8)

int32_t* pwrCreater(char *data) {
    char cut_data[PWR_CREATER_BUFFER_SIZE][16] = {{0}};
    int16_t cnt_index = 0;
    int16_t cnt_char = 0;
    for (int16_t i = 0; data[i] != 'E' && cnt_index < PWR_CREATER_BUFFER_SIZE; i++) {
        if (data[i] == ':') {
            cnt_index++;
            cnt_char = 0;
        } else {
            cut_data[cnt_index][cnt_char] = data[i];
            cnt_char++;
        }
    }
    
    static int32_t data_int[PWR_CREATER_BUFFER_SIZE] = {0};
    for (int16_t i = 0; i < PWR_CREATER_BUFFER_SIZE - 1; i++) {
        data_int[i] = std::atoi(cut_data[i + 1]);
    }
    
    return data_int;
}

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



int main() {

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
          pwrData[pdConter] = buf[i]; // add data.

          //char bufOut[5];
          //sprintf(bufOut, "%d\n" , indexCount);
          serial_port.write(pwrData, strlen(pwrData));

          int32_t* motorPower = pwrCreater(pwrData);
#ifdef REPORT_STATUS
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
#if ENABLE_V5
          V5_PWM.pulsewidth_us(*(motorPower + 9));
          V5_Digital = *(motorPower + 8);
#endif
#if ENABLE_V6
          V6_PWM.pulsewidth_us(*(motorPower + 11));
          V6_Digital = *(motorPower + 10);
#endif
#if ENABLE_V7
          V7_PWM.pulsewidth_us(*(motorPower + 13));
          V7_Digital = *(motorPower + 12);
#endif
#if ENABLE_V8
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