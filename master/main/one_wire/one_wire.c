#include "one_wire/one_wire.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "freertos/FreeRTOS.h"
#include <sys/types.h>

#ifdef INTERRUPTS
#define DISABLE_INTERRUPTS() portDISABLE_INTERRUPTS()
#define ENABLE_INTERRUPTS() portENABLE_INTERRUPTS()
#else
#define DISABLE_INTERRUPTS()
#define ENABLE_INTERRUPTS()
#endif

#define usleep(us)    \
do {                  \
  ets_delay_us (us);  \
} while(0)

void one_wire_write_dword(gpio_num_t gpio_pin, uint32_t data) {
  uint8_t i;
  for (i = 0; i < sizeof (uint32_t) * 8; ++i) {
    one_wire_write_bit (gpio_pin, EXTRACT_BIT_TO_LESSER (data, i));
  }
}

void one_wire_write_word(gpio_num_t gpio_pin, uint16_t data) {
  uint8_t i;
  for (i = 0; i < sizeof (uint16_t) * 8; ++i) {
    one_wire_write_bit (gpio_pin, EXTRACT_BIT_TO_LESSER (data, i));
  }
}

void one_wire_write_byte(gpio_num_t gpio_pin, uint8_t data) {
  uint8_t i;
  for (i = 0; i < sizeof (uint8_t) * 8; ++i) {
    one_wire_write_bit (gpio_pin, EXTRACT_BIT_TO_LESSER (data, i));
  }
}

uint32_t one_wire_read_dword(gpio_num_t gpio_pin) {
  uint8_t i;
  uint32_t result = 0;
  for (i = 0; i < sizeof (uint32_t) * 8; ++i) {
    result |= (one_wire_read_bit (gpio_pin) << i);
  }
  return result;
}

uint16_t one_wire_read_word(gpio_num_t gpio_pin) {
  uint8_t i;
  uint16_t result = 0;
  for (i = 0; i < sizeof (uint16_t) * 8; ++i) {
    result |= (one_wire_read_bit (gpio_pin) << i);
  }
  return result;
}

uint8_t one_wire_read_byte(gpio_num_t gpio_pin) {
  uint8_t i;
  uint8_t result = 0;
  for (i = 0; i < sizeof (uint8_t) * 8; ++i) {
    result |= (one_wire_read_bit (gpio_pin) << i);
  }
  return result;
}

void one_wire_initialize(gpio_num_t serial_pin, gpio_num_t parallel_pin) {
  ESP_ERROR_CHECK (gpio_set_pull_mode (parallel_pin, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK (gpio_set_pull_mode (serial_pin, GPIO_PULLUP_ONLY));
}

uint8_t one_wire_reset(gpio_num_t gpio_pin) {
  DISABLE_INTERRUPTS ();
  uint8_t result;
  gpio_set_direction (gpio_pin, GPIO_MODE_INPUT_OUTPUT_OD);
  gpio_set_level (gpio_pin, LOW_LEVEL);
  usleep (ONE_WIRE_RESET_SIGNAL_TIME);
  gpio_set_level (gpio_pin, HIGH_LEVEL);
  usleep (ONE_WIRE_RESET_SAMPLE_TIME);
  result = gpio_get_level (gpio_pin);
  usleep (ONE_WIRE_RESET_STABILIZATION_TIME);
  gpio_set_direction (gpio_pin, GPIO_MODE_OUTPUT);
  gpio_set_level (gpio_pin, HIGH_LEVEL);
  ENABLE_INTERRUPTS ();
  return result;
}

void one_wire_write_bit(gpio_num_t gpio_pin, one_wire_bits data) {
  DISABLE_INTERRUPTS ();
  gpio_set_direction (gpio_pin, GPIO_MODE_OUTPUT);
  gpio_set_level (gpio_pin, LOW_LEVEL);
  usleep (ONE_WIRE_WRITE_SIGNAL_TIME);
  gpio_set_level (gpio_pin, data);
  usleep (ONE_WIRE_WRITE_TIME);
  gpio_set_level (gpio_pin, HIGH_LEVEL);
  usleep (ONE_WIRE_RECOVER_TIME);
  ENABLE_INTERRUPTS ();
}

uint8_t one_wire_read_bit(gpio_num_t gpio_pin) {
  DISABLE_INTERRUPTS ();
  uint8_t result = 0x0;
  gpio_set_direction (gpio_pin, GPIO_MODE_INPUT_OUTPUT_OD);
  gpio_set_level (gpio_pin, LOW_LEVEL);
  usleep (ONE_WIRE_READ_SIGNAL_TIME);
  gpio_set_level (gpio_pin, HIGH_LEVEL);
  usleep (ONE_WIRE_READ_SAMPLE_TIME);
  result = gpio_get_level (gpio_pin);
  usleep (ONE_WIRE_READ_TIME);
  gpio_set_direction (gpio_pin, GPIO_MODE_OUTPUT);
  gpio_set_level (gpio_pin, HIGH_LEVEL);
  ENABLE_INTERRUPTS ();
  return result;
}
