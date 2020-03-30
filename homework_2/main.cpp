#include "mbed.h"
#include "math.h"
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
Serial pc( USBTX, USBRX );
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
int sample = 1500;
float i;
float ADCdata[1500];
float hz;
int hz_2;
int j;
int num[3];
float t;
int flag;

int main(){
    for (j = 0; j < sample; j++){
        ADCdata[j] = Ain;
        Aout = ADCdata[j];
        wait(1./1000);
    }
    for (j = 0; j < sample; j+=1){
        if (ADCdata[j] <= 0.01 && flag == 0){
            t = j;
            flag = 1;
        }else if (ADCdata[j] <= 0.01){
            t = j-t;
            break;
        }
    }
    for (j = 0; j < sample; j++){
        pc.printf("%1.3f\r\n", ADCdata[j]);
        wait(0.1);
    }
    hz = 1000/t;
    hz_2 = round(hz);
    while(1){
        for( i=0; i<2; i+=0.05){
            Aout = 0.5 + 0.5*sin(i*3.14159);
            wait(1./(40*hz));
        }

        num[0] = hz_2 / 100;
        num[1] = hz_2 % 100 / 10;
        num[2] = hz_2 % 10;
        if (Switch == 1){
            redLED = 1;
            greenLED = 0;
            display = 0;
        }else {
            redLED = 0;
            greenLED = 1;
            display = table[num[0]];
            wait(1);
            display = table[num[1]];
            wait(1);
            display = table[num[2]] + 0x80;
            wait(1);  
        }
    }
}
