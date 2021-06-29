#include <FreeRTOS.h>
#include <task.h>

static TaskHandle_t xDemotask;

#include "driver_init.h"

static void demo_task(void *p)
{
    volatile int x = 0;
    for(;;)
    {
        vTaskDelay(1000);
        x += 1;
    }
}

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    system_init();
    
    xTaskCreate(demo_task, "Demo", configMINIMAL_STACK_SIZE, NULL, 1, xDemotask);

    vTaskStartScheduler();
}
