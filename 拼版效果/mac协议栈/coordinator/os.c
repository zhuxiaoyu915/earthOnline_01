#include "os.h"

//-----------------原子操作-函数---------------------------------------

  inline void os_atomic_enable_interrupt(void)

{
   __asm volatile ("sei");
   
 }

  inline void  os_atomic_end(os_atomic_t oldSreg)
{
  * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x3F + 0x20) = oldSreg;
}


 inline os_atomic_t  os_atomic_start(void )
{
  os_atomic_t  result = * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x3F + 0x20);
   __asm volatile ("cli");
  return result;
}


//-----------------系统休眠函数--------------------------------------
 inline void OS_wait(void)
{
   __asm volatile ("nop");
   __asm volatile ("nop");}

 inline void OS_sleep(void)
{

  * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x35 + 0x20) |= 1 << 5;
  
   __asm volatile ("sleep");}
   
//-----------------任务调度初始化----------------------------------------

 inline void OS_sched_init(void )
{
  OS_sched_free = 0;
  OS_sched_full = 0;
}

//-----------------任务提交函数----------------------------------------

bool  OS_post(void (*tp)(void))
{
  os_atomic_t fInterruptFlags;
  uint8_t tmp;
  fInterruptFlags = os_atomic_start();
  tmp = OS_sched_free;
  if (OS_queue[tmp].tp == (void *)0) {
      OS_sched_free = (tmp + 1) & OS_TASK_BITMASK;
      OS_queue[tmp].tp = tp;
      
      os_atomic_end(fInterruptFlags);

      return TRUE;
    }
  else {
      os_atomic_end(fInterruptFlags);

      return FALSE;
    }
}
//--------------------错误信息输出函数-------------------------------
inline void  error(char * info)
{
      printf("%s",info);
}
//---------------任务运行函数1------------------------------------------

 inline bool OS_run_next_task(void)
{
  os_atomic_t fInterruptFlags;
  uint8_t old_full;
  void (*func)(void );

  fInterruptFlags = os_atomic_start();
  old_full = OS_sched_full;
  func = OS_queue[old_full].tp;
  if (func == (void *)0) 
    {
      os_atomic_end(fInterruptFlags);
      return 0;
    }

  OS_queue[old_full].tp = (void *)0;
  OS_sched_full = (old_full + 1) & OS_TASK_BITMASK;
  os_atomic_end(fInterruptFlags);
  
 
            func();
  
  
  return 1;
}

//--------------任务运行函数2-------------------------------------------
 inline void OS_run_task(void)
{
  OS_run_next_task();
  //OS_sleep();
  //OS_wait();
  
    //while (OS_run_next_task()) 
    //;
  //OS_sleep();
     OS_wait();
  
}
