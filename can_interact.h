#ifndef CAN_INTERACT_H
#define CAN_INTERACT_H
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
  * @brief Functionality declarations of library code to be used to usefully read and write to CAN bus
  * File in nearly all cases designed and works as independant from any reading specifications etc..
  */

enum EndianType {
    BIG_ENDIAN_VAL = 0,
    LITTLE_ENDIAN_VAL
} ;

/**
  * @brief can_socket_init - initialises CAN connection to specific network device via low level syscalls
  * @param const char* - c-string to CAN device name
  */
int can_socket_init(const char*) ;

/**
  * @brief apply_can_fitler - function applies kernel level filtering to socket to relevant CAN frames
  * @param const unsigned int* - const array of hex ids to request from kernel filter
  * @param const size_t - length of array of hex ids to filter for
  * @param const int - socket descriptor
  */
void apply_can_fitler(const unsigned int*, const size_t, const int) ;

/**
  * @brief hex_bytes_to_number - converts array of length x containing hex bytes into a float
  * @param const uint8_t* - const array of hex bytes
  * @param const size_t - length of hex bytes array
  * @param const enum EndianType - endianess. LITTLE_ENDIAN_VAL is little, BIG_ENDIAN_VAL is big
  * @return uint32_t - interpretted unsigned int from hex bytes, taking other params into account
  */
uint32_t hex_bytes_to_number(const uint8_t*, const size_t, const enum EndianType) ;

#endif // CAN_INTERACT_H
