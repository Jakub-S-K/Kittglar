/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include <sys/time.h>

#include "protocol/protocol.h"
#include "one_wire/one_wire.h"

void app_main(void)
{
  gpio_num_t serial_port = GPIO_NUM_25;
  gpio_num_t parallel_port = GPIO_NUM_26;

  printf("Hello world!\n");

  initialize_ports (parallel_port, serial_port);
  ets_delay_us(100);

  printf ("Assign adress 0x%x\n", assign_address (serial_port, parallel_port));
struct timeval tv_now;
gettimeofday(&tv_now, NULL);
int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
  printf ("timeoftheday %lld\n", time_us);
  //
  // first write, read and reset functions have delay due to hardware initializaion of variables and stuff
  // this must be accounted for in slave devices
  //
  one_wire_write_byte (parallel_port, 3);
  ets_delay_us(20);

  one_wire_write_byte (parallel_port, 2);
  ets_delay_us(20);

  one_wire_write_byte (parallel_port, 5);
  ets_delay_us(20);

  one_wire_read_byte (parallel_port);
  ets_delay_us(20);

  for (int i = 10; i >= 0; i--)
  {
    printf("Restarting in %d seconds...\n", i);
    gettimeofday(&tv_now, NULL);
    time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
    printf ("timeoftheday %lld\n", time_us);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}
