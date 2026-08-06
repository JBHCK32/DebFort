#ifndef _SERVICES_UNINSTALL_H_
#define _SERVICES_UNINSTALL_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "../utils/utils.h"

#define SUCCESS 0x00
#define ERR_POPEN 0x01

#define ERR_SERVICE_NOT_EQ 0x02

#define LIST_SERVICES 40
#define MAX_RANGE_SERVICES (LIST_SERVICES + 1)

#define NAME_SERVICES 40
#define MAX_RANGE_NAME_SERVICES (NAME_SERVICES + 1)

bool SHOW_SERVICES();

uint8_t GET_SERVICES(char (*List_Services)[MAX_RANGE_NAME_SERVICES], uint16_t *Number_Services);

bool DISABLE_SERVICE(char *Service);

void Verification_ERR_SERVICES(const uint8_t CODE_ERR);

#endif
