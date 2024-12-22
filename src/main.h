#ifndef MAIN_H
#define MAIN_H

#include "stdint.h"

enum Ecommandtype {
  CMD_NENHUM = 0,
  CMD_DIR = 1,
  CMD_ESQ = 2,
  CMD_CIMA = 3,
  CMD_BAIXO = 4
};

// Structure da mensagem para enviar para ARDUINO
#define HEADER_MSG 0xca
#define FOOTER_MSG 0xfe

typedef struct struct_message {
  uint8_t header;
  uint8_t command;
} struct_message;


#endif