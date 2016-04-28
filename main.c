#include "dsk6713.h"
#include "dsk6713_aic23.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include <stdio.h>

#define tam 7680000
#define GAIN 0.5

Uint32 fs = DSK6713_AIC23_FREQ_16KHZ;
DSK6713_AIC23_CodecHandle manejador;
DSK6713_AIC23_Config configuracion = DSK6713_AIC23_DEFAULTCONFIG;

Uint32 loop = 0;
Uint32 i = 0;

short ganancia = 10;
short delayed,output;
short buffer[1000];
Uint32 dato;

short re[tam];
#pragma DATA_SECTION(re,".EXTRAM")

void codec_init(void);

void leds_output(short led_state)
#define LED_STATE_PLAYING 1
#define LED_STATE_RECORDING 2
#define LED_STATE_WATING 3

int main(){
    codec_init();

    //processing loop
    while(1){
        //Recording data
        if(DSK6713_DIP_get(3)==0){
            leds_output(LED_STATE_RECORDING);
            while(!(DSK6713_AIC23_read(manejador, &dato)));
            delayed= re[i];
            re[i]=dato + delayed;
            re[i] = dato + delayed*GAIN;
            if(++i>tam) i=0;
        }
        //Playing data
        if(DSK6713_DIP_get(0)==0){//line30
            leds_output(LED_STATE_PLAYING);
            while(!(DSK6713_AIC23_write(manejador,re[loop]*ganancia)));
            loop++;
            if(loop>tam) loop=0;
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

void leds_output(short state){
    if(led_state == LED_STATE_PLAYING){
        DSK6713_LED_on(0);
        DSK6713_LED_off(1);
        DSK6713_LED_off(2);
        DSK6713_LED_off(3);
    }else if(led_state == LED_STATE_RECORDING){
        DSK6713_LED_on(3);
        DSK6713_LED_off(1);
        DSK6713_LED_off(2);
        DSK6713_LED_off(0);
    }else{
        DSK6713_LED_off(0);
        DSK6713_LED_off(1);
        DSK6713_LED_off(2);
        DSK6713_LED_off(3);
    }
}
