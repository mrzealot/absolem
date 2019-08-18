#ifndef ABSOLEM_BINARY_H
#define ABSOLEM_BINARY_H

#define BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BINARY(byte)  \
  (byte & 0x80 ? 'X' : '_'), \
  (byte & 0x40 ? 'X' : '_'), \
  (byte & 0x20 ? 'X' : '_'), \
  (byte & 0x10 ? 'X' : '_'), \
  (byte & 0x08 ? 'X' : '_'), \
  (byte & 0x04 ? 'X' : '_'), \
  (byte & 0x02 ? 'X' : '_'), \
  (byte & 0x01 ? 'X' : '_') 

#endif