#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#define MAXIMUM_BUFFER_SIZE  128

//test data
//M:1:700:0:100:1:700:0:100:E
//M:0:100:1:700:0:100:1:700:E


class msgCreater{
public:
    std::vector<float> VecString2VecFloat(const std::string& vecString){
        std::vector<float> motorPower;
        auto splitedData = split(vecString, ':');
        splitedData.pop_back();
        splitedData.erase(std::cbegin(splitedData));
        
        for(std::vector<std::string>::size_type counter = 0; counter < splitedData.size() ; counter += 2){
            if(splitedData[counter] == "1"){
                motorPower.push_back(float(std::stoi(splitedData[counter+1])) / 1000.0);
            }else if(splitedData[counter] == "0"){
                motorPower.push_back((float(std::stoi(splitedData[counter+1])) / 1000.0) * -1);
            }
        }
        return motorPower;
    }
    
private:
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        std::string item;
        for (char ch: s) {
            if (ch == delim) {
                if (!item.empty())
                    elems.push_back(item);
                item.clear();
            }
            else {
                item += ch;
            }
        }
        if (!item.empty()){
            elems.push_back(item);
        }
            
        return elems;
    }
};

static BufferedSerial serial_port(USBTX, USBRX);

//pin setup
PwmOut FR_pwm(PB_8);
PwmOut FL_pwm(PB_9);
PwmOut RR_pwm(PA_5);
PwmOut RL_pwm(PA_6);

DigitalOut FR_dgital(PC_9);
DigitalOut FL_dgital(PC_8);
DigitalOut RR_dgital(PC_6);
DigitalOut RL_dgital(PC_5);



int main()
{
    msgCreater builder = msgCreater();

    // Serial
    serial_port.set_baud(115200);
    serial_port.set_format(8,BufferedSerial::None,1);
    char buf[MAXIMUM_BUFFER_SIZE] = {0};
    std::string bufString = "";

    //PWM setup
    FR_pwm.period_us(100); 
    FL_pwm.period_us(100); 
    RR_pwm.period_us(100); 
    RL_pwm.period_us(100); 

    while (1) {
        if(uint32_t num = serial_port.read(buf, sizeof(buf))){
            std::string convert2string = std::string(buf);
            bufString += convert2string;
            if(convert2string.find("E") != std::string::npos){
                serial_port.write(bufString.c_str(), bufString.size());
                auto motorPower = builder.VecString2VecFloat(bufString);

                if(motorPower[0] >= 0.0){
                    FR_pwm.write(motorPower[0]);
                    FR_dgital.write(1);
                }else{
                    FR_pwm.write(motorPower[0] * -1);
                    FR_dgital.write(0);
                }

                if(motorPower[1] >= 0.0){
                    FL_pwm.write(motorPower[1]);
                    FL_dgital.write(1);
                }else{
                    FL_pwm.write(motorPower[1] * -1);
                    FL_dgital.write(0);
                }

                if(motorPower[2] >= 0.0){
                    RR_pwm.write(motorPower[2]);
                    RR_dgital.write(1);
                }else{
                    RR_pwm.write(motorPower[2] * -1);
                    RR_dgital.write(0);
                }

                if(motorPower[3] >= 0.0){
                    RL_pwm.write(motorPower[3]);
                    RL_dgital.write(0);
                }else{
                    RL_pwm.write(motorPower[3] * -1);
                    RL_dgital.write(0);
                }

                bufString = "";
            }
            for(int i = 0; i < 128 ; i++){
                buf[i] = 0;
            }
        }
    }
}
