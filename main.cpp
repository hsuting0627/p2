#include "mbed.h"
#include "uLCD_4DGL.h"
#include <cmath>

uLCD_4DGL uLCD(D1, D0, D2);
InterruptIn StopButton(D11);
InterruptIn StartButton(D12);
AnalogOut  aout(PA_4);
AnalogIn Ain(A0);
DigitalOut led(LED1);
DigitalOut flash(LED2);
Thread t;
Thread t2;
Thread t3;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread que;
int flag = 1;
int flag2 = 1;
int GenWave = 2;
int sample = 1000;
int sf = 16;
float ADCdata[1000];
void DAC_thread()
{
    while(1){
    while(GenWave == 1){
        for (float i = 0.0f; i < 3.14f; i += 3.14 /100) {
	        if (i < 1.57)
                aout = tanh(i);
            else if (i< 3.14)
                aout = 1 - tanh (i - 1.57);
            else aout = 0;
            ThisThread::sleep_for(1ms);
        }
        queue.call(printf, "called immediately\r\n");

    }
}
}
void ADC_thread()
{
    while(1){
        if(GenWave == 1 && flag2 == 1){
            for (int i = 0; i < sample; i++){
                ADCdata[i] = Ain;
                ThisThread::sleep_for(1000ms/sample);
                if(GenWave != 1)
                    break;
            }
        }
        else 
        ThisThread::sleep_for(1000ms/sample);
    }
}
void print_thread()
{
    while(1){
        if (GenWave == 0){
            for (int i = 0; i < sample; i++){
                printf("%f\r\n", ADCdata[i]);
            }
        }
        else {
        ThisThread::sleep_for(1000ms/sample);
        }
}
}
void stop()
{
   GenWave = 0;
   flag = 1;
}
void start()
{
   GenWave = 1;
   flag = 1;
}
int main()
{
   t.start(&DAC_thread); // attach the address of the flip function to the rising edge
   t2.start(&ADC_thread);
   t3.start(&print_thread);
   que.start(callback(&queue, &EventQueue::dispatch_forever));
   StartButton.rise(&start);
   StopButton.rise(&stop);
   uLCD.color(WHITE);
   ThisThread::sleep_for(1s);
  uLCD.text_width(2); // 4X size text
  uLCD.text_height(2);
   while (1)
   { // wait around, interrupts will interrupt this!
    if (GenWave == 1 && flag == 1) {
        uLCD.locate(1, 2);
        uLCD.printf("Waveform generation");
        flag = 0;
   }
   else if (GenWave == 0 && flag == 1) {
        uLCD.locate(1, 2);
        uLCD.printf("Data transfer      ");
        flag = 0;
   }
    else 
        ThisThread::sleep_for(1s);

   }
}
