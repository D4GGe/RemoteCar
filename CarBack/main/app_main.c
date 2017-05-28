#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "debug.h"
#include "communication.h"

void app_main() {
  INFO("[APP] Startup..\n");
  INFO("[APP] Free memory: %d bytes\n", system_get_free_heap_size());
  INFO("[APP] SDK version: %s, Build time: %s\n", system_get_sdk_version(),
       BUID_TIME);

#ifdef CPU_FREQ_160MHZ
  INFO("[APP] Setup CPU run as 160MHz\n");
  SET_PERI_REG_BITS(RTC_CLK_CONF, RTC_CNTL_SOC_CLK_SEL, 0x1,
                    RTC_CNTL_SOC_CLK_SEL_S);
  WRITE_PERI_REG(CPU_PER_CONF_REG, 0x01);
  INFO("[APP] Setup CPU run as 160MHz - Done\n");
#endif

  nvs_flash_init();
  wifi_conn_init();
}