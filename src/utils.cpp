#include "mbed.h"
#include "stm32f4xx.h"
#include "utils.hpp"
#include "macro.hpp"


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