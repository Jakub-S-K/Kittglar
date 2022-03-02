#ifndef PROTOCOL
#define PROTOCOL

#include "hal/gpio_types.h"

uint16_t assign_address(gpio_num_t parallel_port, gpio_num_t serial_port);
void serial_port_listener(void* data);
void parallel_port_task(void* data);
void initialize_ports(gpio_num_t parallel_port, gpio_num_t serial_port);

#endif // PROTOCOL