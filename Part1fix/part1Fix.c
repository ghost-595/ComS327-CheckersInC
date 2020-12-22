/*Jonathan Kelly 616716128 jkelly5*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void getFileText(char text[], char fileName[]);
void getStdinText(char text[]);
void textToWords(char text[], char word[], int *position);
void textToBoard(char text[], char **board, int *boardKeywordPosition, int *currentLine, int *error, int *redPawns, int *redKings, int *blackPawns, int *blackKings);

FILE* infile;

extern int noCapture = 0;
extern int multipuleJumps = 0;

int main(int argc, char *argv[])
{

  int ctr;
  char* inputSwitch;

  char* inputString = NULL;
  int usingStdin = 1;

  char text[5000];
  char* word = malloc(150 * sizeof(char*));
  int currentLine = 1;
  int position = 0;

  noCapture = 0;
  multipuleJumps = 0;

  int wordCount = 1;
  int movesFound = 0;
  int movesCount = 0;
  int error = 0;


  int redKings = 0;
  int redPawns = 0;
  int blackKings = 0;
  int blackPawns = 0;

  char *turn = malloc(6 * sizeof(char*));
  char **board = (char**)malloc(8*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 8; mallocCounter++)
  {
    board[mallocCounter] = malloc(8 * sizeof(char*));
  }
  char **moves = (char**)malloc(10*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 10; mallocCounter++)
  {
    moves[mallocCounter] = malloc(6 * sizeof(char*));
  }


   for( ctr=0; ctr < argc; ctr++ )
   {
     inputSwitch = argv[ctr];
     int len = strlen(inputSwitch);
     char *last_four = &inputSwitch[len-4];

     if(strcmp(last_four, ".txt") == 0)
     {
       inputString = argv[ctr];
       usingStdin = 0;
     }
   }

   if(usingStdin == 1)
   {
     getStdinText(text);
   }
   else
   {
     getFileText(text, inputString);
   }

  if(word == NULL || turn == NULL)
  {
    fprintf(stderr,"malloc failed\n");
  }

  /*put all words in to words array*/
  for(wordCount = 1;text[position] != '\0';)
  {
    textToWords(text, word, &position);
    if(strlen(word) > 0)
    {
      /*check that key words are present and in the expected order*/
      if(strcmp(word, "RULES:") != 0 && wordCount == 1)
      {
        fprintf(stderr,"RULES: key word expected, error on line %d\n", currentLine);
        return 1;
      }

      if((strcmp(word, "no") == 0  && wordCount == 2))
      {
        noCapture = 1;
        wordCount--;
      }
      if(strcmp(word, "capture") != 0  && wordCount == 2)
      {
        fprintf(stderr,"'no capture' or 'capture' key word expected, error on line %d\n", currentLine);
        return 1;
      }

      if((strcmp(word, "multiple") == 0 || strcmp(word, "single") == 0)  && wordCount == 3)
      {
        if((strcmp(word, "multiple") == 0))
        {
          multipuleJumps = 1;
        }
        wordCount--;
      }
      if(strcmp(word, "jumps") != 0 && wordCount == 3)
      {
        fprintf(stderr,"'multipule jumps' or 'single jumps' key word expected, error on line %d\n", currentLine);
        return 1;
      }

      if(strcmp(word, "TURN:") != 0 && wordCount == 4)
      {
        fprintf(stderr,"'TURN:' key word expected, error on line %d\n", currentLine);
        return 1;
      }
      if((strcmp(word, "red") != 0 && strcmp(word, "black") != 0) && wordCount == 5)
      {
        fprintf(stderr,"either 'red' or 'black' expected, error on line %d\n", currentLine);
        return 1;
      }
      else if(strcmp(word, "red") == 0 && wordCount == 5)
      {
        strcpy(turn,"red");
      }
      else if(strcmp(word, "black") == 0 && wordCount == 5)
      {
        strcpy(turn,"black");
      }

      if(strcmp(word, "BOARD:") != 0 && wordCount == 6)
      {
        fprintf(stderr,"'BOARD:' key word expected, error on line %d\n", currentLine);
        return 1;
      }
      else if(strcmp(word, "BOARD:") == 0 && wordCount == 6)
      {
        /*send in board array and position of board key word in the text array and return the amount of words to skip in here*/
        textToBoard(text, board, &position, &currentLine, &error, &redPawns, &redKings, &blackPawns, &blackKings);
        if(error == 1)
        {
          return 1;
        }
        position++;
      }

      if(movesFound == 1)
      {
        if(word[6] == '-')
        {
          fprintf(stderr, "Sorry multiple jumps are not implemnted\n");
          return 1;
        }

        if(strlen(word) != 6)
        {
          fprintf(stderr,"incorrect amount of charcter for move on line %d\n", currentLine);
          return 1;
        }
        if(!(word[0] >= 97 && word[0] <= 104) || !(word[4] >= 97 && word[4] <= 104))
        {
          fprintf(stderr,"Move out of bounds on line %d\n", currentLine);
          return 1;
        }
        else if(!(word[1] >= 49 && word[1] <= 56) || !(word[5] >= 49 && word[5] <= 56))
        {
          fprintf(stderr,"Move out of bounds on line %d\n", currentLine);
          return 1;
        }
        else
        {
          strcpy(moves[movesCount], word);
          movesCount++;
        }
      }

      if(strcmp(word, "MOVES:") != 0 && wordCount == 7)
      {
        fprintf(stderr,"'MOVES:' key word expected, error on line %d\n", currentLine);
        return 1;
      }
      else if(strcmp(word, "MOVES:") == 0 && wordCount == 7)
      {
        movesFound = 1;
      }
      wordCount++;
    }

    /*checks if last char on word is a new line chacter and not the last word in the file and if so increase current line int*/
    if(text[position-1] == '\n' && text[position+1] != '\0')
    {
      currentLine++;
    }

    word = realloc(word, 150 * sizeof(char*));
  }

  /*if word count is still one no words were found in the file*/
  if(wordCount == 1)
  {
    fprintf(stderr,"RULES: key word expected, error on line %d\n", currentLine);
    return 1;
  }


  printf("VALID INPUT\nInital configuration:\nTURN: %s\nRed: %d kings, %d pawns\nBlack %d kings, %d pawns\n", turn, redKings, redPawns, blackKings, blackPawns);

  /*free the memory used for word*/
  free(word);
  moves[movesCount] = "\0";

  return 0;
}

void textToBoard(char text[], char **board, int *position, int *currentLine, int *error, int *redPawns, int *redKings, int *blackPawns, int *blackKings)
{
  char tmpChar = text[*position];
  char color = ' ';

  while(isspace(tmpChar) == 1)
  {
    *position = *position + 1;
     tmpChar = text[*position];
  }

  for(int c = 0; c < 8; c++)
  {
    for(int r = 0; r < 8;)
    {
      if(isspace(tmpChar) == 0 && tmpChar != '|' && tmpChar != '-' && tmpChar != '+')
      {
        /*if tmpChar is not an valid char for board set error variable to 1*/
        if(tmpChar != 'b' && tmpChar != 'B' && tmpChar != 'r' && tmpChar != 'R' && tmpChar != '\"' && tmpChar != '.')
        {
          *error = 1;
          fprintf(stderr,"invalid char in board or board not complete, error on line %d\n", *currentLine);
          return;
        }

        /*increment the counter for each piece based on tmpChar*/
        if(tmpChar == 'b')
        {
          *blackPawns = *blackPawns + 1;
        }
        else if(tmpChar == 'B')
        {
          *blackKings = *blackKings + 1;
        }
        else if(tmpChar == 'r')
        {
          *redPawns = *redPawns + 1;
        }
        else if(tmpChar == 'R')
        {
          *redKings = *redKings + 1;
        }

        /*make sure each square alternates colors*/
        if(tmpChar == 'b' || tmpChar == 'B' || tmpChar == '.' || tmpChar == 'r' || tmpChar == 'R' )
        {
          if(color == ' ')
          {
            color = 'b';
          }
          else if(color == 'b')
          {
            *error = 1;
            fprintf(stderr,"two black squares are in a row on line %d and char is %c\n", *currentLine, tmpChar);
            return;
          }
          else
          {
            color = 'b';
          }
        }
        else if(tmpChar == '\"')
        {
          if(color == ' ')
          {
            color = 'r';
          }
          else if(color == 'r')
          {
            *error = 1;
            fprintf(stderr,"\n\ntwo red squares are in a row on line %d at c: %d and c: %d is char %c\n", *currentLine, c, r, tmpChar);
            return;
          }
          else
          {
            color = 'r';
          }
        }
        board[c][r] = tmpChar;
        r++;
      }
      color = ' ';
      if(tmpChar == '\n')
      {
        *currentLine = *currentLine + 1;
      }
      *position = *position + 1;
       tmpChar = text[*position];
    }
  }

}

void textToWords(char text[], char word[], int *position)
{
  char tmpChar;
  int wordPosition = 0;

  do
  {
    tmpChar = text[*position];
    /*exludes all lines that start with '#'*/
    if(tmpChar == '#')
    {
      while(tmpChar != '\n')
      {
          tmpChar = text[*position];
          *position = *position + 1;
      }
      *position = *position - 1;
      break;
    }
    /*records a word until a whitespace chacter is found*/
    word[wordPosition] = tmpChar;
    wordPosition++;
    *position = *position + 1;
  }while(isspace(tmpChar) == 0 && tmpChar != ':');


  if(isspace(word[wordPosition-1]) != 0)
  {
    word[wordPosition-1] = '\0';
  }
  else
  {
    word[wordPosition] = '\0';
  }
}

void getStdinText(char text[])
{
  char tmpChar = ' ';
  int i = 0;

  while(tmpChar != EOF)
  {
    tmpChar = getchar();
    text[i] = tmpChar;
    i++;
  }
  text[i-1] = '\0';
}

void getFileText(char text[], char fileName[])
{
  FILE *file;
  file = fopen(fileName, "r");

  char tmpChar = ' ';
  int i = 0;

  while(tmpChar != EOF)
  {
    tmpChar = fgetc(file);
    text[i] = tmpChar;
    i++;
  }
  text[i-1] = '\0';
}
