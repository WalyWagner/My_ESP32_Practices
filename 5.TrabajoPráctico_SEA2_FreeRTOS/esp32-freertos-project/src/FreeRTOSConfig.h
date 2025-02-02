#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/// Include the ESP-IDF FreeRTOS configuration
#include "sdkconfig.h"

/// Application specific definitions

// Set to 1 for preemptive scheduling, 0 for cooperative scheduling
#define configUSE_PREEMPTION                    1 
// Define the tick rate in Hertz (ticks per second)
#define configTICK_RATE_HZ                      (1000) // 2000 ticks per second, 0.5 milliseconds per tick
// Define the maximum number of task priorities
#define configMAX_PRIORITIES                    (5)
// Define the minimum stack size for tasks
#define configMINIMAL_STACK_SIZE                ((unsigned short) 1024)
// Define the total heap size available for dynamic memory allocation
#define configTOTAL_HEAP_SIZE                   ((size_t) (32 * 1024))
// Define the maximum length of task names
#define configMAX_TASK_NAME_LEN                 (16)
// Set to 1 to use 16-bit ticks, 0 to use 32-bit ticks
#define configUSE_16_BIT_TICKS                  0
// Set to 1 if the idle task should yield the CPU when other tasks are ready to run
#define configIDLE_SHOULD_YIELD                 1
// Set to 1 to enable the use of mutexes
#define configUSE_MUTEXES                       1
// Define the size of the queue registry
#define configQUEUE_REGISTRY_SIZE               8
// Set to 1 to enable stack overflow checking
#define configCHECK_FOR_STACK_OVERFLOW          2
// Set to 1 to enable the use of recursive mutexes
#define configUSE_RECURSIVE_MUTEXES             1
// Set to 1 to enable the malloc failed hook
#define configUSE_MALLOC_FAILED_HOOK            1
// Set to 1 to enable the use of application task tags
#define configUSE_APPLICATION_TASK_TAG          0
// Set to 1 to enable the use of counting semaphores
#define configUSE_COUNTING_SEMAPHORES           1
// Set to 1 to support dynamic memory allocation
#define configSUPPORT_DYNAMIC_ALLOCATION        1
// Set to 1 to support static memory allocation
#define configSUPPORT_STATIC_ALLOCATION         0
// Hook function related definitions
// Set to 1 to enable the idle hook
#define configUSE_IDLE_HOOK                     0
// Set to 1 to enable the tick hook
#define configUSE_TICK_HOOK                     0
// Set to 1 to enable the malloc failed hook
#define configUSE_MALLOC_FAILED_HOOK            1

/// Run time and task stats gathering related definitions

// Set to 1 to enable run time stats gathering
#define configGENERATE_RUN_TIME_STATS           0
// Set to 1 to enable the trace facility
#define configUSE_TRACE_FACILITY                1
// Set to 1 to enable stats formatting functions
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/// Co-routine related definitions

// Set to 1 to enable co-routines
#define configUSE_CO_ROUTINES                   0
// Define the maximum number of co-routine priorities
#define configMAX_CO_ROUTINE_PRIORITIES         (2)
// Software timer related definitions
// Set to 1 to enable software timers
#define configUSE_TIMERS                        1
// Define the priority of the timer task
#define configTIMER_TASK_PRIORITY               (2)
// Define the length of the timer queue
#define configTIMER_QUEUE_LENGTH                10
// Define the stack depth of the timer task
#define configTIMER_TASK_STACK_DEPTH            (configMINIMAL_STACK_SIZE * 2)

/// Interrupt nesting behaviour configuration

// Define the kernel interrupt priority
#define configKERNEL_INTERRUPT_PRIORITY         (configLIBRARY_LOWEST_INTERRUPT_PRIORITY)
// Define the maximum syscall interrupt priority
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
// Define the maximum API call interrupt priority
#define configMAX_API_CALL_INTERRUPT_PRIORITY   (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
// Define to trap errors during development
#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/// FreeRTOS MPU specific definitions

// Set to 1 to include application defined privileged functions
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/// Optional functions - most linkers will remove unused functions anyway

// Set to 1 to include vTaskPrioritySet
#define INCLUDE_vTaskPrioritySet                1
// Set to 1 to include uxTaskPriorityGet
#define INCLUDE_uxTaskPriorityGet               1
// Set to 1 to include vTaskDelete
#define INCLUDE_vTaskDelete                     1
// Set to 1 to include vTaskCleanUpResources
#define INCLUDE_vTaskCleanUpResources           0
// Set to 1 to include vTaskSuspend
#define INCLUDE_vTaskSuspend                    1
// Set to 1 to include vTaskDelayUntil
#define INCLUDE_vTaskDelayUntil                 1
// Set to 1 to include vTaskDelay
#define INCLUDE_vTaskDelay                      1
// Set to 1 to include xTaskGetSchedulerState
#define INCLUDE_xTaskGetSchedulerState          1

#endif // FREERTOS_CONFIG_H