#include "PinNames.h"
#include "mbed.h"
#include "mbed_thread.h"
#include <cstdint>

#define MAXIMUM_BUFFER_SIZE 32

uint32_t* pwrCreater(char *data) {
    char cut_data[16][8] = {{0}};
    uint8_t cnt_index = 0;
    uint8_t cnt_char = 0;
    for (uint8_t i = 0; data[i] != 'E' && cnt_index < 15; i++) {
        if (data[i] == ':') {
            cnt_index++;
            cnt_char = 0;
        } else {
            cut_data[cnt_index][cnt_char] = data[i];
            cnt_char++;
        }
    }
    
    static uint32_t data_int[16] = {0};
    for (uint8_t i = 0; i < 14; i++) {
        data_int[i] = std::atoi(cut_data[i + 1]);
    }
    
    return data_int;
}

static BufferedSerial serial_port(USBTX, USBRX);

// pin setup
PwmOut V1_PWM(PB_10);
PwmOut V2_PWM(PB_1); 
PwmOut V3_PWM(PC_6);
PwmOut V4_PWM(PC_7);

DigitalOut V1_Digital(PA_9);
DigitalOut V2_Digital(PA_5);
DigitalOut V3_Digital(PC_4);
DigitalOut V4_Digital(PB_2);


int main() {

  // Serial
    serial_port.set_baud(115200);
    serial_port.set_format(8, BufferedSerial::None, 1);
    char buf[MAXIMUM_BUFFER_SIZE] = {0};

  // PWM setup
    V1_PWM.period_us(1000);
    V2_PWM.period_us(1000);
    V3_PWM.period_us(1000);
    V4_PWM.period_us(1000);

  char pwrData[32] = {0};
  uint32_t pdConter = 0;

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
                   buf[i] == '9' || buf[i] == ':') {
          pwrData[pdConter] = buf[i];
          pdConter++;
        } else if (buf[i] == 'E') {
          pwrData[pdConter] = buf[i];

          uint32_t* motorPower = pwrCreater(pwrData);

          V1_PWM.pulsewidth_us(*(motorPower + 1));
          V1_Digital = *(motorPower + 0);

          V2_PWM.pulsewidth_us(*(motorPower + 3));
          V2_Digital = *(motorPower + 2);

          V3_PWM.pulsewidth_us(*(motorPower + 5));
          V3_Digital = *(motorPower + 4);

          V4_PWM.pulsewidth_us(*(motorPower + 7));
          V4_Digital = *(motorPower + 6);
          pdConter = 0;
          for (uint8_t j = 0; j < 32; j++) {
            pwrData[j] = 0;
          }
        }
      }
    }
  }
}
