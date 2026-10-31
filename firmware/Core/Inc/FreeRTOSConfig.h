#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f446xx.h"

#define configCPU_CLOCK_HZ                   	 	 (16000000UL)
#define configTICK_RATE_HZ                   	 	 (1000UL)
#define configUSE_PREEMPTION                  	 	 (1UL)
#define configUSE_TIME_SLICING               	 	 (0UL) // Pas de taches de meme priorite dans mon projet
#define configTICK_TYPE_WIDTH_IN_BITS         		 (TICK_TYPE_WIDTH_32_BITS)
#define configMAX_PRIORITIES                  	 	 (6UL) // 4 taches prévues + la tache de repos + 1 de marge
#define configMINIMAL_STACK_SIZE              		 (128UL) // Valeur de depart
#define configUSE_IDLE_HOOK                   		 (0UL)
#define configUSE_TICK_HOOK                   		 (0UL)
#define configUSE_MALLOC_FAILED_HOOK          		 (0UL)
#define configUSE_DAEMON_TASK_STARTUP_HOOK    		 (0UL)
#define configSUPPORT_STATIC_ALLOCATION       		 (1UL)
#define configSUPPORT_DYNAMIC_ALLOCATION      		 (0UL)
#define configUSE_TIMERS                      		 (0UL)
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY (5UL)
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  		 (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8UL - __NVIC_PRIO_BITS))

#define INCLUDE_vTaskPrioritySet               (1UL)
#define INCLUDE_uxTaskPriorityGet              (1UL)
#define INCLUDE_vTaskDelete                    (1UL)
#define INCLUDE_vTaskSuspend                   (1UL)
#define INCLUDE_xTaskDelayUntil                (1UL)
#define INCLUDE_vTaskDelay                     (1UL)
#define INCLUDE_xTaskGetSchedulerState         (1UL)
#define INCLUDE_xTaskGetCurrentTaskHandle      (1UL)
#define INCLUDE_uxTaskGetStackHighWaterMark    (0UL)
#define INCLUDE_xTaskGetIdleTaskHandle         (0UL)
#define INCLUDE_eTaskGetState                  (0UL)
#define INCLUDE_xTimerPendFunctionCall         (0UL)
#define INCLUDE_xTaskAbortDelay                (0UL)
#define INCLUDE_xTaskGetHandle                 (0UL)
#define INCLUDE_xTaskResumeFromISR             (1UL)

#endif
