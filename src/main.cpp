/***
 * Demo program to flash LED attached to GPIO PADs.
 * Uses FreeRTOS Task
 */

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

#include "BlinkAgent.h"
#include "BlinkWorker.h"

// Standard Task priority
#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

// LED PADs
#define LED_PAD             0
#define LED1_PAD            1
#define LED2_PAD            2
#define LED3_PAD            3
#define LED4_PAD            4
#define LED5_PAD            5
#define LED6_PAD            6
#define LED7_PAD            7
#define LED8_PAD            8


void runTimeStats(){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    // Get number of tasks
    uxArraySize = uxTaskGetNumberOfTasks();
    printf("Number of tasks %d\n", uxArraySize);

    // Allocate a TaskStatus_t structure for each task
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if(pxTaskStatusArray != NULL){
        // Generate raw status information about each task
        uxArraySize = uxTaskGetSystemState(
            pxTaskStatusArray,
            uxArraySize,
            &ulTotalRunTime
        );

        // Print stats
        for(x = 0; x < uxArraySize; x++){
            printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                pxTaskStatusArray[x].xTaskNumber,
                pxTaskStatusArray[x].uxCurrentPriority,
                pxTaskStatusArray[x].uxBasePriority,
                pxTaskStatusArray[x].usStackHighWaterMark,
                pxTaskStatusArray[x].pcTaskName
            );
        }

        // Free array
        vPortFree(pxTaskStatusArray);
    } else {
        printf("Failed to allocate space for stats\n");
    }

    // Get heap allocation information
    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
        heapStats.xAvailableHeapSpaceInBytes,
        heapStats.xNumberOfFreeBlocks,
        heapStats.xNumberOfSuccessfulAllocations,
        heapStats.xNumberOfSuccessfulFrees
    );
}


/***
 * Main task to blink external LEDs
 */
void mainTask(void *params){
    BlinkAgent blink(LED_PAD);

    BlinkWorker worker1(LED1_PAD);
    BlinkWorker worker2(LED2_PAD);
    BlinkWorker worker3(LED3_PAD);
    BlinkWorker worker4(LED4_PAD);
    BlinkWorker worker5(LED5_PAD);
    BlinkWorker worker6(LED6_PAD);
    BlinkWorker worker7(LED7_PAD);
    BlinkWorker worker8(LED8_PAD);

    // Counting semaphore
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(8, 4);

    worker1.setSemaphore(sem);
    worker2.setSemaphore(sem);
    worker3.setSemaphore(sem);
    worker4.setSemaphore(sem);
    worker5.setSemaphore(sem);
    worker6.setSemaphore(sem);
    worker7.setSemaphore(sem);
    worker8.setSemaphore(sem);

    printf("Main task started\n");

    blink.start("Blink", TASK_PRIORITY);

    worker1.start("Worker 1", TASK_PRIORITY);
    worker2.start("Worker 2", TASK_PRIORITY);
    worker3.start("Worker 3", TASK_PRIORITY);
    worker4.start("Worker 4", TASK_PRIORITY);
    worker5.start("Worker 5", TASK_PRIORITY);
    worker6.start("Worker 6", TASK_PRIORITY);
    worker7.start("Worker 7", TASK_PRIORITY);
    worker8.start("Worker 8", TASK_PRIORITY);

    while(true){
        runTimeStats();
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}


/***
 * Launch the tasks and scheduler
 */
void vLaunch(void){
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}


/***
 * Main
 */
int main(void){
    // Setup serial over USB and give a few seconds to settle before we start
    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    // Start tasks and scheduler
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();

    return 0;
}