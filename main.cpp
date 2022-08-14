#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#define MAXIMUM_BUFFER_SIZE 32

// test data
// M:1:700:0:100:1:700:0:100:E
// M:0:100:1:700:0:100:1:700:E

int ctoi_ori(char c) {
  switch (c) {
  case '0':
    return 0;
    break;
  case '1':
    return 1;
    break;
  case '2':
    return 2;
    break;
  case '3':
    return 3;
    break;
  case '4':
    return 4;
    break;
  case '5':
    return 5;
    break;
  case '6':
    return 6;
    break;
  case '7':
    return 7;
    break;
  case '8':
    return 8;
    break;
  case '9':
    return 9;
    break;

  default:
    return 0;
    break;
  }
}

double pwrCreater(char *data, int n, int index) {
  char cut_data[16][16] = {{0}};
  int cnt_index = 0;
  int cnt_char = 0;
  for (int i = 0; i < n; i++) {
    if (data[i] == ':') {
      cnt_index++;
      cnt_char = 0;
    } else {
      cut_data[cnt_index][cnt_char] = data[i];
      cnt_char++;
    }
  }

  size_t size = std::strlen(cut_data[index * 2 + 2]);
  if (cut_data[index * 2 + 1][0] == '1') {
    double rt = 0.0;

    for (int x = 0; x < size; x++) {
      rt += (double)(ctoi_ori(cut_data[index * 2 + 2][x]) *
                     std::pow(10, (size - x - 1)));
    }
    return rt / 1000;

  } else if (cut_data[index * 2 + 1][0] == '0') {
    double rt = 0.0;
    for (int x = 0; x < size; x++) {
      rt += (double)(ctoi_ori(cut_data[index * 2 + 2][x]) *
                     std::pow(10, (size - x - 1)) * -1);
    }
    return rt / 1000;
  }else{
      return 0;
  }
}

static BufferedSerial serial_port(USBTX, USBRX);

// pin setup
PwmOut FR_pwm(PB_8);
PwmOut FL_pwm(PB_9);
PwmOut RR_pwm(PA_5);
PwmOut RL_pwm(PA_6);

DigitalOut FR_dgital(PC_9);
DigitalOut FL_dgital(PC_8);
DigitalOut RR_dgital(PC_6);
DigitalOut RL_dgital(PC_5);

int main() {

  // Serial
  serial_port.set_baud(230400);
  serial_port.set_format(8, BufferedSerial::None, 1);
  char buf[MAXIMUM_BUFFER_SIZE] = {0};

  // PWM setup
  FR_pwm.period_us(100);
  FL_pwm.period_us(100);
  RR_pwm.period_us(100);
  RL_pwm.period_us(100);

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
          //serial_port.write(pwrData, sizeof(pwrData));
          //char c = '\t';
          //serial_port.write(&c, sizeof(c));

          double motorPower[4] = {0.0};
          motorPower[0] = pwrCreater(pwrData, sizeof(pwrData), 0);
          motorPower[1] = pwrCreater(pwrData, sizeof(pwrData), 1);
          motorPower[2] = pwrCreater(pwrData, sizeof(pwrData), 2);
          motorPower[3] = pwrCreater(pwrData, sizeof(pwrData), 3);

          //char debug[128] = {0};
          //sprintf(debug, "Debug:0:%d:1:%d:2:%d:3:%d\n" , (int)(motorPower[0] * 1000.0), (int)(motorPower[1] * 1000.0), (int)(motorPower[2] * 1000.0), (int)(motorPower[3] * 1000.0));
          //serial_port.write(&debug, sizeof(debug));

          if (motorPower[0] >= 0.0) {
            FR_pwm.write(motorPower[0]);
            FR_dgital.write(1);
          } else {
            FR_pwm.write(motorPower[0] * -1);
            FR_dgital.write(0);
          }

          if (motorPower[1] >= 0.0) {
            FL_pwm.write(motorPower[1]);
            FL_dgital.write(1);
          } else {
            FL_pwm.write(motorPower[1] * -1);
            FL_dgital.write(0);
          }

          if (motorPower[2] >= 0.0) {
            RR_pwm.write(motorPower[2]);
            RR_dgital.write(1);
          } else {
            RR_pwm.write(motorPower[2] * -1);
            RR_dgital.write(0);
          }

          if (motorPower[3] >= 0.0) {
            RL_pwm.write(motorPower[3]);
            RL_dgital.write(0);
          } else {
            RL_pwm.write(motorPower[3] * -1);
            RL_dgital.write(1);
          }

          pdConter = 0;
          for (uint8_t j = 0; j < 32; j++) {
            pwrData[j] = 0;
          }
        }
      }
    }
  }
}
