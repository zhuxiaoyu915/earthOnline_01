#include "adc_sensor.h"
//added by liulingyi 

uint8_t  adc_init(void)
{
     { 
           os_atomic_t _nesc_atomic = os_atomic_start();
		  {
           //ADC控制寄存器中ADIE位和ADSC位清零 
                  ADCSRA &=0x86;
           //设置参考电压，配置为默认情形
                  ADMUX=0;
           	}
          os_atomic_end(_nesc_atomic);
     }
    ADC_INTERRUPT_FLAG=0;
	 
}

uint8_t  sensor_init(void)
{
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
     
      
      OS_MAKE_INT1_OUTPUT();
      OS_SET_INT1_PIN();
      //whether to configure  the INT2 pin???
    }

    os_atomic_end(_nesc_atomic); }           
          
}

void read_data_task(void)
{
  uint8_t loopnum=0;
  while (loopnum<10)
  {
  
  SENSOR_DATA_VALUE[loopnum]=0;
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
     //设置ADMUX寄存器
         ADMUX |=0x01;  
     //AD使能开始AD转换
         ADCSRA |=0xe7; 
     }

    os_atomic_end(_nesc_atomic); }       

     //查询ADCSRA中断，
  
   while(ADC_INTERRUPT_FLAG==0)
   	{    
   	      ADC_INTERRUPT_FLAG=(ADCSRA&0x10);
   	}
  
    //读取ADCH和ADCL
    
    SENSOR_DATA_VALUE[loopnum]=ADCL;
    SENSOR_DATA_VALUE[loopnum]+=(ADCH<<8);
  //printf("sensor is %04x\n",SENSOR_DATA_VALUE[loopnum]);
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
     //清除ADC中断标志
          ADCSRA |=0x10;
     //关闭ADC转换使能
          //ADCSRA &=0x7f;
    	}
      os_atomic_end(_nesc_atomic); }  
	  
   //printf("sensor is %04x\n",SENSOR_DATA_VALUE);
  // SENSOR_DATA_VALUE=0;
   adc_init();
   //
   //OS_CLR_INT1_PIN();
   ADC_INTERRUPT_FLAG=0;
   //sensor_init();
loopnum++;
  } 
	 
}