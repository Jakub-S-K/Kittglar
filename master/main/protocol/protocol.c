#include "protocol/protocol.h"

#include <stdio.h>

#include "protocol/commands.h"
#include "one_wire/one_wire.h"

#include "hal/gpio_types.h"
#include "driver/gpio.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

uint16_t assign_address(gpio_num_t parallel_port, gpio_num_t serial_port) {
  // emit request for broadcast, if doesn't have an address then
  // broadcast reset on parrarel lane
  // address 0x0 is broadcast address
  esp_log_write (ESP_LOG_DEBUG ,"Entered: %s\n", __FUNCTION__);
  printf ("Entered: %s\n", __FUNCTION__);
  uint16_t max_address = 0x1;

  // check for any device
  if (one_wire_reset (serial_port)) {
    // printf ("%s: No devices connected\n", __FUNCTION__);
    return max_address;
  }

  // send command to serial port, it will be trapped in each device that do not have an address
  one_wire_write_byte (serial_port, COMMAND_ASSIGN_ADDRESS);

  do {
    one_wire_write_byte (parallel_port, COMMAND_ASSIGN_ADDRESS);
    one_wire_write_word (parallel_port, max_address);
    ++max_address;
    // might need to wait for next device to get signal for addres registration
    one_wire_write_byte (parallel_port, COMMAND_CHECK_MISSING_ADDRESS);
    // while even one is missing address
  } while (one_wire_read_bit (parallel_port));
  return max_address;
}

uint16_t to_read = 0x0;

void serial_port_listener(void* data) {
  uint64_t port = *(uint8_t*) (data);
  gpio_set_direction (port, GPIO_MODE_INPUT);
  while (1) {
    // TODO change this to interrupt
    if (!gpio_get_level (port)) {
      to_read = one_wire_read_word (port);
      //xTaskNotify (this_different_task, one_wire_read_word (port));
    }
  }
}

void initialize_ports(gpio_num_t parallel_port, gpio_num_t serial_port) {
  one_wire_initialize (serial_port, parallel_port);
}

void parallel_port_task(void* data) {
  uint64_t port = *(uint8_t*) (data);
  uint16_t address;
  uint32_t incoming_data;

  uint8_t package_index = 64;
  while (1) {
    //address = xTaskWaitForNotify ();
    address = to_read;
    one_wire_write_byte (port, COMMAND_READ_VALUE);
    one_wire_write_word (port, address);
    // we will read packages by 64 bytes
    package_index = 64;
    do {
      package_index = package_index - 4;
      incoming_data = one_wire_read_dword (port);
      printf ("Data recieved 0x%x\n", incoming_data);
    } while (package_index > 0);
  }
}
