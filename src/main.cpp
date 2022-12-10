#include <mbed.h>

#include "stm32f4xx_hal.h"

void Configure_GPIO(void);
void configure_ADC2_Channel_0(void);
void Delay_ms(volatile int time_ms);

uint32_t Adc_value;
ADC_HandleTypeDef myADC2Handle;
DigitalOut led(LED1);

int main(void)
{
	Configure_GPIO(); // initialize PA0 pin 
	configure_ADC2_Channel_0(); // configure ADC2
  
  Timer timer1; // initialize Timer 
  uint32_t prev_Adc_value=0;
	while(1)
	{
    timer1.start();
	  HAL_ADC_Start(&myADC2Handle); // start A/D conversion
		if(HAL_ADC_PollForConversion(&myADC2Handle, 5) == HAL_OK) //check if conversion is completed
		{
		Adc_value  = HAL_ADC_GetValue(&myADC2Handle); // read digital value and save it inside uint32_t variable
		}
		HAL_ADC_Stop(&myADC2Handle); // stop conversion 
    printf("My sensor: %d\n",Adc_value);
    
    //If Breath detected timer will be reset
    if(prev_Adc_value>=Adc_value&&prev_Adc_value-Adc_value>300){  
      timer1.reset();
    }else if(Adc_value>=prev_Adc_value && Adc_value-prev_Adc_value>300){
      timer1.reset();
    }
    
    prev_Adc_value =Adc_value;
    
    //If Breath not detected for 10s, send a Warning message!
    if(timer1.read()>=10){
      printf("Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!Help!\n");
      led=!led; // toggle led on board
    }

		Delay_ms(2500); //Breath detection interval
    
    printf("Timer interval: %f\n",timer1.read());
	}
}

/* Configure and initialize PA0 pin as analog input pin for A/D conversion */
void Configure_GPIO(void)
{
	 __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock to GPIOA
	GPIO_InitTypeDef ADCpin; //create an instance of GPIO_InitTypeDef C struct
	ADCpin.Pin = GPIO_PIN_0; // Select pin PA0
	ADCpin.Mode = GPIO_MODE_ANALOG; // Select Analog Mode
	ADCpin.Pull = GPIO_NOPULL; // Disable internal pull-up or pull-down resistor
	HAL_GPIO_Init(GPIOA, &ADCpin); // initialize PA0 as analog input pin
}
void configure_ADC2_Channel_0(void)
{
__HAL_RCC_ADC2_CLK_ENABLE(); // enable clock to ADC2 module
	myADC2Handle.Instance = ADC2; // create an instance of ADC2
	myADC2Handle.Init.Resolution = ADC_RESOLUTION_12B; // select 12-bit resolution 
	myADC2Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV; //select  single conversion as a end of conversion event
	myADC2Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT; // set digital output data right justified
	myADC2Handle.Init.ClockPrescaler =ADC_CLOCK_SYNC_PCLK_DIV8; 
	HAL_ADC_Init(&myADC2Handle); // initialize AD2 with myADC2Handle configuration settings
	
  /*select ADC2 channel */
	ADC_ChannelConfTypeDef Channel_AN0; // create an instance of ADC_ChannelConfTypeDef
	Channel_AN0.Channel = ADC_CHANNEL_0; // select analog channel 0
	Channel_AN0.Rank = 1; // set rank to 1
	Channel_AN0.SamplingTime = ADC_SAMPLETIME_15CYCLES; // set sampling time to 15 clock cycles
	HAL_ADC_ConfigChannel(&myADC2Handle, &Channel_AN0); // select channel_0 for ADC2 module. 
}
void Delay_ms(volatile int time_ms)
{
	      int j;
        for(j = 0; j < time_ms*4000; j++)
            {}  /* excute NOP for 1ms */
}
