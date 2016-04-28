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
int main(){
manejador = DSK6713_AIC23_openCodec(0,&configuracion);
	DSK6713_AIC23_setFreq(manejador,fs);
	DSK6713_DIP_init();
	DSK6713_LED_init();

	DSK6713_LED_off(0);   //line20
	DSK6713_LED_off(1);
	DSK6713_LED_off(2);
	DSK6713_LED_off(3);	
	#pragma DATA_SECTION(re,".EXTRAM")
	while(1){
	
		if(DSK6713_DIP_get(0)==0){//line30
			DSK6713_LED_on(0);
			DSK6713_LED_off(1);
			DSK6713_LED_off(2);
			DSK6713_LED_off(3);
		
			while(!(DSK6713_AIC23_write(manejador,re[loop]*ganancia)));
			loop++;
			if(loop>tam) loop=0;
		}

		if(DSK6713_DIP_get(3)==0){
			DSK6713_LED_on(3);
			DSK6713_LED_off(1);  
			DSK6713_LED_off(2); //40
			DSK6713_LED_off(0); 
			
			while(!(DSK6713_AIC23_read(manejador, &dato)));
			/*re[i]=dato + delayed*GAIN;
			i++;
			delayed =dato;
			buffer
			if(i>tam) i=0;*/
			delayed= re[i];
			re[i]=dato + delayed;
			re[i] = dato + delayed*GAIN;
			if(++i>tam) i=0;


		else{
			
			DSK6713_LED_off(3);
			DSK6713_LED_off(1);  
			DSK6713_LED_off(2); 
			DSK6713_LED_off(0); 
			}

		
		}
	}
}
