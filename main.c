#include "dsk6713.h"
#include "dsk6713_aic23.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include <stdio.h>


Uint32 fs = DSK6713_AIC23_FREQ_16KHZ;
DSK6713_AIC23_CodecHandle manejador;
DSK6713_AIC23_Config configuracion = DSK6713_AIC23_DEFAULTCONFIG;

void codec_init(void);
Uint32 codec_read(void);
void codec_write(Uint32 output_data);
#define OUTPUT_GAIN 7

#define BUFFER_SIZE 7680000
short input_buffer[BUFFER_SIZE];
void input_buffer_clean(void);
#pragma DATA_SECTION(input_buffer,".EXTRAM")


void leds_output(short led_state);
#define LED_STATE_ACTIVE 1
#define LED_STATE_WATING 0

#define DELAY_TIME 15 //Delay time define in samples
#define DELAY_GAIN 0.5 // 0 - 1.0

int main(){
    //Configure hardware
    codec_init();

    while(1){
        //Processing data
        if(DSK6713_DIP_get(3)==0){
            //Initialize processing
            leds_output(LED_STATE_ACTIVE);
            input_buffer_clean();
            int k = 0;
            int delay_index = 0;
            int output_data = 0;

            while(DSK6713_DIP_get(3)==0){
                input_buffer[k] = codec_read();

                delay_index  = k - DELAY_TIME;
                delay_index  = (delay_index >= 0) ? delay_index : delay_index + BUFFER_SIZE;

                output_data = input_buffer[k] + DELAY_GAIN * input_buffer[delay_index];
                codec_write(output_data * OUTPUT_GAIN);

                k++;
                if(k >= BUFFER_SIZE){
                    k = 0;
                }
            }
        }
        //Waiting
        else{
            leds_output(LED_STATE_WATING);
        }
    }
}

void codec_init(void){
    manejador = DSK6713_AIC23_openCodec(0,&configuracion);
    DSK6713_AIC23_setFreq(manejador,fs);
    DSK6713_DIP_init();
    DSK6713_LED_init();

}

Uint32 codec_input_data;

Uint32 codec_read(void){
    while(!(DSK6713_AIC23_read(manejador, &coded_input_data)));
    return codec_input_data;
}

void codec_write(Uint32 output_data){
    while(!(DSK6713_AIC23_write(manejador,output_data)));
}

void input_buffer_clean(void){
    for(int i = 0; i < BUFFER_SIZE; i++){
        input_buffer[k] = 0;
    }
}

void leds_output(short state){
    if(led_state == LED_STATE_ACTIVE){
        DSK6713_LED_on(0);
        DSK6713_LED_off(1);
        DSK6713_LED_off(2);
        DSK6713_LED_off(3);
    }else{
        DSK6713_LED_off(0);
        DSK6713_LED_off(1);
        DSK6713_LED_off(2);
        DSK6713_LED_off(3);
    }
}
