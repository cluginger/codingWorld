//-----------------------------------------------------------------------------
// ass1-b1.c
//
// implementation of a reversePrint function:
// the function reads the input up to a space and it saves the reverse input
// in a seperated array. After the hole input is put reverse in the seperated
// array, the function changes the first letter of the after beginnig and after
// a space to uppercase and all other letters to lowercase. Afterwards the
// function prints the array.
//
// Group: 22, study-assistant: Kahr Mathias
//
// Author: Christian Luginger 01556194
//
// Latest Changes: 07.12.2019
//-----------------------------------------------------------------------------
//

#include<stdio.h>
#include <stdbool.h>

#define TEST_LENGTH 100
//-----------------------------------------------------------------------------
///
/// @param name The name of a person is given as input seperated by spaces
///
/// @return 0 if success
//  @return -1 if empty string
//  @return -2 if parameters are not correct
//
int reversePrint(char *name);

int main()
{
  char *name = "Short Name";
  int ret = reversePrint(name);
  if(ret != 0)
  {
    printf("WRONG");
  }
  return 0;
}

int reversePrint(char *name)
{
  char test[TEST_LENGTH];
  int reverse_element = 0;
  int counter_test = 0;
  //create variables because of coding standard
  int one_letter = 1;
  int lowercase_ascii = 97;
  int diff_low_up = 32;
  bool wrong_input = true;
  bool space_after_space = false;
  bool first_letter_test = false;
  //check for empty input
  if(name[0] == '\0')
  {
    return -1;
  }
  //check if inpurt start with whitespace
  if(name[0] == ' ')
  {
    return -2;
  }
  //count the number of spaces in input. if no space then only one word
  for(int iter_name = 0; name[iter_name] != '\0'; iter_name++)
  {
    if(name[iter_name] == ' ')
    {
      wrong_input = false;
    }
  }
  if(wrong_input == true)
  {
    return -2;
  }
  //put shor name reverse in test
  else
  {
    for(int iter_name = 0; name[iter_name] != '\0'; iter_name++)
    {
      //check if first letter after space is space too
      if(space_after_space == true && name[iter_name] == ' ')
      {
        return -2;
      }
      else
      {
        space_after_space = false;
      }
      //if whitespace put reverse string in test
      if(name[iter_name] == ' ')
      {
        space_after_space = true;
        for(int back = iter_name - one_letter; back >= reverse_element; back--)
        {
          test[counter_test] = name[back];
          counter_test++;
        }
        test[counter_test] = name[iter_name];
        counter_test++;
        reverse_element = counter_test;
      }
      if(name[iter_name + one_letter] == '\0')
      {
        for(int back = iter_name; back >= reverse_element; back--)
        {
          test[counter_test] = name[back];
          counter_test++;
        }
        test[counter_test] = name[iter_name + one_letter];
      }
    }
  }
  //change first element to Uppercase and the other one to lowercase
  for(int iter_test = 0; iter_test < counter_test; iter_test++)
  {
    if(test[iter_test] < lowercase_ascii && first_letter_test == true
       && test[iter_test] != ' ')
    {
      test[iter_test] += diff_low_up;
    }

    if(first_letter_test == false)
    {
      test[iter_test] -= diff_low_up;
      first_letter_test = true;
    }

    if(test[iter_test] == ' ')
    {
      first_letter_test = false;
    }
  }
  printf("%s",test);
  return 0;
}

