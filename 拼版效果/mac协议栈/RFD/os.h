#ifndef os_h
#define os_h
#include "project.h"

	//原子操作
	 typedef uint8_t  os_atomic_t;
	 inline os_atomic_t  os_atomic_start(void );
	 inline void  os_atomic_end(os_atomic_t oldSreg);
	 inline void os_atomic_enable_interrupt(void);
	//系统函数
	 inline void OS_wait(void );
	 inline void OS_sleep(void );
	 inline void OS_sched_init(void );
	 inline bool OS_run_next_task(void);
	 inline void OS_run_task(void);
	 bool  OS_post(void (*tp)(void));
	 inline  void  error(char * info);
	//参数设置
	typedef struct _OS_sched_entry_T {
		void (*tp)(void);
	  
	}OS_sched_entry_T;

	enum OS_CON{
	  OS_MAX_TASKS = 8, 
	  OS_TASK_BITMASK = OS_MAX_TASKS - 1
	};
	volatile OS_sched_entry_T OS_queue[OS_MAX_TASKS];
	uint8_t OS_sched_full;
	volatile uint8_t OS_sched_free;

	
	

#endif

