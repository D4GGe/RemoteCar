#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"

#include "esp_event_loop.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#include "communication.h"
#include "debug.h"
#include "motor.h"

void app_main() {
  INFO("[APP] Startup..\n");

#ifdef CPU_FREQ_160MHZ
  INFO("[APP] Setup CPU run as 160MHz\n");
  SET_PERI_REG_BITS(RTC_CLK_CONF, RTC_CNTL_SOC_CLK_SEL, 0x1,
                    RTC_CNTL_SOC_CLK_SEL_S);
  WRITE_PERI_REG(CPU_PER_CONF_REG, 0x01);
  INFO("[APP] Setup CPU run as 160MHz - Done\n");
#endif

  nvs_flash_init();
  wifi_conn_init();
  init_controls();
  mcpwm_example_brushed_motor_control();


  //brushed_motor_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, 0.0);
  //xTaskCreate(mcpwm_example_brushed_motor_control, "mcpwm_examlpe_brushed_motor_control", 4096, NULL, 5, NULL);
}
