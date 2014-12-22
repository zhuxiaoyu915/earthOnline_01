#include "adc_sensor.h"
//added by liulingyi 

uint8_t  adc_init(void)
{
     { 
           os_atomic_t _nesc_atomic = os_atomic_start();
		  {
           //ADC���ƼĴ�����ADIEλ��ADSCλ���� 
                  ADCSRA &=0x86;
           //���òο���ѹ������ΪĬ������
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
     //����ADMUX�Ĵ���
         ADMUX |=0x01;  
     //ADʹ�ܿ�ʼADת��
         ADCSRA |=0xe7; 
     }

    os_atomic_end(_nesc_atomic); }       

     //��ѯADCSRA�жϣ�
  
   while(ADC_INTERRUPT_FLAG==0)
   	{    
   	      ADC_INTERRUPT_FLAG=(ADCSRA&0x10);
   	}
  
    //��ȡADCH��ADCL
    
    SENSOR_DATA_VALUE[loopnum]=ADCL;
    SENSOR_DATA_VALUE[loopnum]+=(ADCH<<8);
  //printf("sensor is %04x\n",SENSOR_DATA_VALUE[loopnum]);
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
     //���ADC�жϱ�־
          ADCSRA |=0x10;
     //�ر�ADCת��ʹ��
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