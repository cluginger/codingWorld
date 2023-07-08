//-----------------------------------------------------------------------------
// ass1-b3.c
//
// The program changes a decimal number to its bcd number and then it transform
// the number back. After the trasformation the program prints out the number
// plus the binary code of the number.
//
// Group: 22, study-assistant: Kahr Mathias
//
// Author: Christian Luginger 01556194
//
// Latest Changes: 15.01.2019
//-----------------------------------------------------------------------------
//
#include<stdio.h>
#include<stdlib.h>

#define MAX_NUMBER 4294967295 //for some unknown reason limits.h isnt working
#define INT_SIZE sizeof(int) * 8 //size in bits(32)
#define MAX_BCD 99999999


typedef enum _ErrorMessage_{
  SUCESS = 0,
  WRONG_ARGC = -1,
  NUMBER_OUT_OF_RANGE = -2
} ErrorMessage;
//------------------------------------------------------------------------------
///
/// The function checks the main arguments for correctness.
///
/// @param argc is the number of program_arguments
/// @param input is argv[1]
/// @program_name is argv[0]
///
/// @return ErrorMessage   the different messages are in the previous enum
//
ErrorMessage checkParameters(int argc, unsigned long input,
                             char *program_name);
//------------------------------------------------------------------------------
///
/// Here the program prints the program logic and it checks length of bcd
///
///
/// @param input is argv[1]
///
//
void transformationsOfNumber(unsigned long input);
//------------------------------------------------------------------------------
///
/// The function prints the binary of a number seperated by |
///
/// @param input is argv[1]
//
void binaryDec(unsigned long input);
//------------------------------------------------------------------------------
///
/// The function converts a number to binary coded decimal. Therefor every
/// single letter of the param number is represented by 4 bit in a nibble.
/// Therefore it takes the number modulo 10 and it has a counter variable move
/// to shift the number in correct positin for bcd.
///
/// @param number represents a binary decimal number
///
/// @return unsigned long bcd number
//
unsigned long convertBinaryToBCD(unsigned long number);
//------------------------------------------------------------------------------
///
/// The function converts the bcd to its binary complement. Therefore it shifts
/// the number by the amount of bit(which get reduced in 4 bit steps from 28 -
/// 0) and it multiplies the number with 1000000(this value gets divided by
/// 10 the lower the current bit is). the result is  add to an extra
/// variable called bcd.
///
/// @param number represents a binary decimal number
///
/// @return unsigned long bcd number
//
unsigned long convertBCDToBinary(unsigned long number);

//------------------------------------------------------------------------------
///
/// The main function transforms the argv[1] in an int for future calculations.
/// Furthermore it returns an ErrorMessage if the parameter isnt correct and
/// it calls the function transformationsOfNumber where all calculations are
/// done.
///
/// @param argc is the number of program_arguments
/// @param input is argv[1]
/// @program_name is argv[0]
///
/// @return ErrorMessage   the different messages are in the previous enum
//
int main(int argc, char* argv[])
{
  unsigned long input = 0;

  if(argc == 2)
  {
    input = strtoul(argv[1], NULL, 10);
  }

  char *program_name = argv[0];

  ErrorMessage program_arguments = checkParameters(argc, input, program_name);

  if(program_arguments != SUCESS)
  {
    return program_arguments;
  }
  transformationsOfNumber(input);

  return SUCESS;
}

ErrorMessage checkParameters(int argc, unsigned long input, char *program_name)
{
  //check correct amount of parameters
  if(argc != 2)
  {
    printf("Usage: %s number\n", program_name);
    return WRONG_ARGC;
  }
  //check correct range of parameter
  else if(input >= MAX_NUMBER || input < 1)
  {
    printf("number out of range: allowed range is 1 to %ld\n", MAX_NUMBER - 1);
    return NUMBER_OUT_OF_RANGE;
  }
  else
  {
    return SUCESS;
  }
}
void transformationsOfNumber(unsigned long input)
{
  unsigned long bcd = 0;
  unsigned long binary = 0;

  printf(" Input: %10lu - ", input);
  binaryDec(input);
  bcd = convertBinaryToBCD(input);
  binary = convertBCDToBinary(bcd);
  //check length of bcd
  if(bcd > MAX_BCD)
  {
    printf("   BCD: Binary Code too large.\n");
    printf("Binary: Binary Code too large.\n");
  }
  else
  {
    printf("   BCD: %10lu - ", bcd);
    binaryDec(bcd);
    printf("Binary: %10lu - ", binary);
    binaryDec(binary);
  }


}
void binaryDec(unsigned long input)
{
  int compare = 0;

  for(int position_binary = INT_SIZE - 1; position_binary >= 0;
      position_binary--)
  {
    //shift input to higest bit and compares the value with bit &
    compare = input >> position_binary;

    if(compare & 1)
    {
      printf("1");
    }
    else
    {
      printf("0");
    }
    //after every 4th letter there is a space and after every 8th leter there
    // is a |
    if(position_binary % 4 == 0)
    {
      printf(" ");
    }
    if(position_binary % 8 == 0 && position_binary != 0)
    {
      printf("| ");
    }

  }
  printf("\n");
}

unsigned long convertBinaryToBCD(unsigned long number)
{
  int move = 0;
  unsigned long bcd = 0;
  while(number > 0)
  {
    //take the bitwise or oprerand to store result in variable
    bcd = bcd | (number % 10) << (move++ << 2);
    number = number / 10;
  }
  return bcd;
}

unsigned long convertBCDToBinary(unsigned long number)
{
  unsigned long binary;
  binary = (number >> 28) * 100000000;
  binary = binary + (0x0f & (number >> 24)) * 1000000;
  binary = binary + (0x0f & (number >> 20)) * 100000;
  binary = binary + (0x0f & (number >> 16)) * 10000;
  binary = binary + (0x0f & (number >> 12)) * 1000;
  binary = binary + (0x0f & (number >> 8)) * 100;
  binary = binary + (0x0f & (number >> 4)) * 10;
  binary = binary + (0x0f & (number));

  return binary;
}
