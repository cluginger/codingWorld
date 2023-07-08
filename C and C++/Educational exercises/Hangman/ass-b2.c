//-----------------------------------------------------------------------------
// ass1-b2.c
//
// The programm reads wordlists from a file. These wordlists are the basic
// of some rounds hangman. The user has to guess single letters in every round
// and he has to find the correct word within 11 guesses.
//
// Group: 22, study-assistant: Kahr Mathias
//
// Author: Christian Luginger 01556194
//
// Latest Changes: 15.12.2019
//-----------------------------------------------------------------------------
//

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define A_ASCII 65
#define Z_ASCII 90
#define A_LOWERCASE_ASCII 97
#define Z_LOWERCASE_ASCII 122
#define BASIC_WORD_LENGTH 5
#define SIZE_LETTER 1
#define MAX_GUESSES 11
#define DIFF_UPPER_LOWERCASE 32


typedef enum _ErrorMessage_{
  SUCESS = 0,
  WRONG_ARGC = 1,
  FILE_CANT_OPEN = 2,
  INVALID_FILE = 3,
  OUT_OF_MEMORY = 4
} ErrorMessage;

typedef struct _WordList_
{
  char *word_;
  char *result_;
  struct WordList *next_;
} WordList;
//------------------------------------------------------------------------------
///
/// The function opens the argv[1} and it reads a textfile letter by letters.
/// Moreover it checks the correctnes of the file
///
/// @param name_txt_file is the name of the argv
///
/// @return ErrorMessage   the different messages are in the previous enum
//
ErrorMessage checkFileCorrectness(char *file_name);
//------------------------------------------------------------------------------
///
/// The function checks the file for correct letters. Only a-z in lower and
/// uppercase are allowed.
///
/// @param read_letter is the current letter of the word
///
/// @return Errormessage  the different messages are in the previous enum
//
ErrorMessage checkFileLetters(int read_letter);
//------------------------------------------------------------------------------
///
/// The function prints the correct errormessages
///
/// @param file_correct   is the correct status of the previous read file
/// @param file_name   is name of the argv[1]
///
//
void printErrorMessage(ErrorMessage file_correct, char *file_name);
//------------------------------------------------------------------------------
///
/// The function opens the argv[1] and it reads a textfile letter by letters.
/// Moreover it saves the word from a line dynamically in a linkedList. The
/// result contains the first letter of the word and the result is filled with
/// _ .
///
/// @param file_correct checks the corectness of the file...
///                     especially out of memory
/// @param file_name is the name of the argv
///
/// @return head of the wordlist
//
WordList *createWordlist(char *file_name, ErrorMessage *file_correct);
//------------------------------------------------------------------------------
///
/// The function creates the result-element within the linkedList. A result
/// contains the first element of the word and the rest is filled with _ .
///
/// @param count_letter is the size of the current word
/// @param *word is the current element of the WordList
/// @param *line is the dynamically array where the line of txt is saved
//
void createWordResult(int count_letter, WordList *word, char *line);
//------------------------------------------------------------------------------
///
/// The function frees all dynamically allocated memory of the linkedList.
///
/// @param *word_list is the head of the read wordlist
//
void freeWordList(WordList *word_list);
//------------------------------------------------------------------------------
///
/// The function is an loop where the game logic is performed.
///
/// @param *word_list is the head of the read wordlist
//
void gameLoop(WordList *word_list);
//------------------------------------------------------------------------------
///
/// The function counts the number of words in the linked list
///
/// @param *word is the head of the read wordlist
///
/// @return number of words
//
int totalNumWords(WordList *word_list);
//------------------------------------------------------------------------------
///
/// The function compares the given Input to the word. If the input is within
/// the word all positions of result where the letter is found in word become
/// given to the input.
///
/// @param *word is the head of the read wordlist
/// @param input is the input by getchar
/// @param *wrong_guesses is the current amount of wrong guesses
//
void compareResultToWord(WordList *word, int input, int *wrong_guesses);
//------------------------------------------------------------------------------
///
/// The main program.
/// Reads name of argv[1] and argv[0] and it gives as parameters to certain
/// funcion. Afterwards it creates a head of the linkedList where wordlists
/// are stored. Furthermore the gameLoop functoin is started and the
/// dynamically allocated memory of the linkedList is freed.
/// in LOOP_REPETITIONS.
///
/// @param argc used
/// @param argv used
///
/// @return 0 if all everything worked and a correct file is read. else
/// it returns an errormessage.
//
int main(int argc, char **argv)
{
  char *file_name = argv[1];
  char *executable_name = argv[0];

  if(argc != 2)
  {
    printf("usage: %s filename\n", executable_name);
    return WRONG_ARGC;
  }

  ErrorMessage file_correct = checkFileCorrectness(file_name);

  if(file_correct != SUCESS)
  {
    printErrorMessage(file_correct, file_name);
    return file_correct;
  }
  WordList *wordlist = createWordlist(file_name, &file_correct);

  if(file_correct != SUCESS)
  {
    printErrorMessage(file_correct, file_name);
    freeWordList(wordlist);
    return file_correct;
  }
  gameLoop(wordlist);
  freeWordList(wordlist);
  return SUCESS;
}

ErrorMessage checkFileCorrectness(char *file_name)
{
  FILE *file;
  file = fopen(file_name, "r");
  if(file == NULL)
  {
    return FILE_CANT_OPEN;
  }
  int read_letter = 0;
  //first letter which is not space
  bool check_first_letter = false;
  //after check first letter is true.... this value checks
  // if there are spaces after first word
  bool check_space = false;
  ErrorMessage check_return = SUCESS;
  do{
    read_letter = fgetc(file);
    // \n and EOF are end of line
    if(read_letter != EOF && read_letter != '\n')
    {

      if(read_letter != ' ')
      {
        check_return = checkFileLetters(read_letter);
        check_first_letter = true;
      }

      if(read_letter == ' ' && check_first_letter == true)
      {
        check_space = true;
      }
      //checks if there are spaces between letters-> incorrect file
      if(read_letter != ' ' && check_space == true)
      {
        check_return = INVALID_FILE;
      }
    }

    if(check_return == INVALID_FILE)
    {
      fclose(file);
      return INVALID_FILE;
    }

    if(read_letter == '\n')
    {
      check_first_letter = false;
      check_space = false;
    }

  } while (read_letter != EOF);
    fclose(file);
    return SUCESS;
}

ErrorMessage checkFileLetters(int read_letter)
{
  if((read_letter < A_ASCII) || (read_letter > Z_ASCII &&
      read_letter < A_LOWERCASE_ASCII) || (read_letter > Z_LOWERCASE_ASCII))
  {
    return INVALID_FILE;
  }
  else
  {
    return SUCESS;
  }
}
void printErrorMessage(ErrorMessage file_correct, char *file_name)
{
  switch(file_correct)
  {
    case FILE_CANT_OPEN:
      printf("ERROR: cannot open file %s\n", file_name);
      break;
    case INVALID_FILE:
      printf("ERROR: file %s invalid\n", file_name);
      break;
    case OUT_OF_MEMORY:
      printf("ERROR: Out of Memory\n");
      break;
    case SUCESS:
      break;
    case WRONG_ARGC:
      break;
    default:
      break;
  }
}
WordList *createWordlist(char *file_name, ErrorMessage *file_correct){
  FILE *file;
  file = fopen(file_name, "r");
  WordList *head;
  //next node after head
  WordList *following;
  if(file == NULL)
  {
    *file_correct = FILE_CANT_OPEN;
  }
  head = malloc(sizeof(WordList));
  if(head == NULL)
  {
    *file_correct = OUT_OF_MEMORY;
    return head;
  }

  int read_letter = 0;
  bool check_head = true;
  char *word;
  int word_length = BASIC_WORD_LENGTH;

  word = malloc(BASIC_WORD_LENGTH * sizeof(char));
  if(word == NULL)
  {
    *file_correct = OUT_OF_MEMORY;
    return head;
  }
  int count_positon_letter = 0;

  do{
    read_letter = fgetc(file);
    //reads letters of line in dynamic word array
    if(read_letter != ' ' && read_letter != EOF && read_letter != '\n')
    {
      word[count_positon_letter] = read_letter;
      count_positon_letter++;

      if(count_positon_letter == word_length)
      {
        //if length of array is reached realloc enhances the
        //length again by 5 byte
        word_length += BASIC_WORD_LENGTH;
        word = realloc(word, word_length * sizeof(char));
        if(word == NULL)
        {
          *file_correct = OUT_OF_MEMORY;
          return head;
        }
      }

    }
    if(read_letter == '\n' || read_letter == EOF)
    {
      //save word and result of word in head
      if(check_head == true)
      {
        word[count_positon_letter] = '\0';
        head->word_ = malloc((count_positon_letter + SIZE_LETTER) *
                              sizeof(char));
        if(head->word_ == NULL)
        {
          *file_correct = OUT_OF_MEMORY;
          return head;
        }

        for(int letter = 0; letter <= count_positon_letter; letter++)
        {
          head->word_[letter] = word[letter];
        }
        head->result_ = malloc((count_positon_letter + SIZE_LETTER) *
                                sizeof(char));
        if(head->result_ == NULL)
        {
          *file_correct = OUT_OF_MEMORY;
          return head;
        }
        createWordResult(count_positon_letter, head, word);
        head->next_ = NULL;
        check_head = false;
      }
      else
      {
        word[count_positon_letter] = '\0';
        following = head;
        while(following->next_)
        {
          following = (WordList *)following->next_;
        }
        //create a temoorary Wordlist to store it in next element of list
        WordList *temp = NULL;
        temp = malloc(sizeof(WordList));
        if(temp == NULL)
        {
          *file_correct = OUT_OF_MEMORY;
          return head;
        }
        temp->word_ = malloc((count_positon_letter + SIZE_LETTER) *
                              sizeof(char));
        if(temp->word_ == NULL)
        {
          *file_correct = OUT_OF_MEMORY;
          return head;
        }
        for(int letter = 0; letter <= count_positon_letter; letter++)
        {
          temp->word_[letter] = word[letter];
        }
        temp->result_ = malloc((count_positon_letter + SIZE_LETTER) * sizeof(char));
        if(temp->result_ == NULL)
        {
          *file_correct = OUT_OF_MEMORY;
          return head;
        }
        createWordResult(count_positon_letter, temp, word);
        temp->next_ = NULL;

        following->next_ = (struct WordList *)temp;
      }
      count_positon_letter = 0;
    }

  } while(read_letter != EOF);
  free(word);
  return head;
}
void createWordResult(int count_letter, WordList *word, char *line)
{
  //first element is as in line then only underscores
  for(int counter = 0; counter <= count_letter; counter++)
  {
    if(counter == 0)
    {
      word->result_[counter] = line[counter];
    }
    else if(counter > 0 && counter < count_letter)
    {
      word->result_[counter] = '_';
    }
    else if(counter == count_letter)
    {
      word->result_[counter] = line[count_letter];
    }
  }
}
void freeWordList(WordList *word_list)
{
  WordList *temp = NULL;
  while(word_list)
  {
    free(word_list->word_);
    free(word_list->result_);
    temp = (WordList *)word_list->next_;
    free(word_list);
    word_list = temp;
  }
}
void gameLoop(WordList *word_list)
{
  int total_words = totalNumWords(word_list);
  int correct_words = 0;
  int wrong_guesses = 0;
  int finished_words = 0;
  int input = 0;

  printf("%s (%d)\n", word_list->result_, wrong_guesses);
  //loop gets interrupted if the amount of total words is equal to the
  //finished words
  for( ; ; )
  {
    printf("Your guess: ");
    input = getchar();
    //without this there would not be a fluent programm
    while(getchar() != '\n');
    compareResultToWord(word_list, input, &wrong_guesses);
    //if wrong_guesse is reached print wrong word and go
    //to next element of list
    if(wrong_guesses >= MAX_GUESSES)
    {
      printf("%s (x_x)\n", word_list->word_);
      wrong_guesses = 0;
      finished_words++;
      if(word_list->next_ != NULL)
      {
        word_list = (WordList *)word_list->next_;
        printf("%s (%d)\n", word_list->result_, wrong_guesses);
      }
    }
    //else print current state of word and if word is equal to result go
    // to next element of the list
    else
    {
      printf("%s (%d)\n", word_list->result_, wrong_guesses);
      if(strcmp(word_list->word_, word_list->result_) == 0)
      {
        correct_words++;
        finished_words++;
        wrong_guesses = 0;
        if(word_list->next_ != NULL)
        {
          word_list = (WordList *)word_list->next_;
          printf("%s (%d)\n", word_list->result_, wrong_guesses);
        }
      }
    }

    if(finished_words == total_words)
    {
      break;
    }
  }
  //end message tells how often you tipped the correct word
  printf("won (%d/%d)", correct_words, total_words);
}
int totalNumWords(WordList *word_list)
{
  int total_num = 0;
  while(word_list)
  {
    total_num += 1;
    word_list = (WordList *)word_list->next_;
  }
  return total_num;
}
void compareResultToWord(WordList *word, int input, int *wrong_guesses)
{
  bool letter_in_word = false;

  for(int counter_word = 0; counter_word < strlen(word->word_); counter_word++)
  {
    if(word->word_[counter_word] == (input + DIFF_UPPER_LOWERCASE))
    {
      input += DIFF_UPPER_LOWERCASE;
    }
    if(word->word_[counter_word] == (input - DIFF_UPPER_LOWERCASE))
    {
      input -= DIFF_UPPER_LOWERCASE;
    }
    if(word->word_[counter_word] == input)
    {
      word->result_[counter_word] = input;
      letter_in_word = true;
    }
  }
  if(letter_in_word == false)
  {
    *wrong_guesses += 1;
  }
}

