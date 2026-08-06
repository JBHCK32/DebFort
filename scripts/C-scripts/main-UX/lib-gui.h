#ifndef _LIB_GUI_H_
#define _LIB_GUI_H_

#include "services-uninstall.h"
#include "close-ports.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

#define GENERIC_SLEEP 1

#define SIZE_OPTION 1
#define MAX_RANGE_OP (SIZE_OPTION + 1)

#define MAX_NUMBER_PORT 65535

#define LEN_PORT 5
#define MAX_LEN_PORT (LEN_PORT + 1)

#define SUCCESS 0x00

#define ERR_SIGINIT 0x01
#define ERR_MEMORY_BUFFER 0x02
#define ERR_OPEN_TTY 0x03

#define ERR_INVALID_PORT 0x04
#define ERR_INVALID_NUMBER 0x05


uint8_t Processing_InputChar(char *CONTAINER, size_t MAX_RANGE);


void Especific_Menu();
void Message_ERR();
void L_MESSAGE_ERR(int sig);
void clear();

void Menu_Services();
void Menu_Ports();
void Menu_Firewall();
void Menu_Privileges();
void Menu_SSH();

void Verification_ERR_MENU(const uint8_t CODE_ERR);

#endif
