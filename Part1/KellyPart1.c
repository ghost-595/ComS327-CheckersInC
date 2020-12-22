/*Jonathan Kelly 616716128 jkelly5*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void getFileText(FILE* infile,char text[]);

FILE* infile;

int main()
{
  infile = fopen("input.txt", "r");
  char text[5000];
  char tmpWord[1000];
  char words[100][1000];
  char board[8][8];
  char tmpChar = ' ';

  int i = 0;
  int wordPosition = 0;
  int wordCount = 0;
  int boardCount = 0;

  int boardKeywordPosition = 0;
  int movesKeywordPosition = 0;

  int rulesKeywordWordPosition = 0;
  int turnsKeywordWordPosition = 0;
  int movesKeywordWordPosition = 0;

  int rulesPressent = 0, turnsPressent = 0, boardPressent = 0, movesPressent = 0;

  int capture = 0;
  int multipuleJumps = 0;

  int redKings = 0;
  int redPawns = 0;
  int blackKings = 0;
  int blackPawns = 0;

  char turn[6];


  getFileText(infile, text);

  /*put all words in to words array*/
  while(text[i] != '\0')
  {
    tmpChar = text[i];

    /*exludes all lines that start with '#'*/
    if(tmpChar == '#')
    {
      while(tmpChar != '\n')
      {
          tmpChar = text[i];
          i++;
      }
      i--;
    }
    /*records a word until a whitespace chacter is found*/
    else if(isspace(tmpChar) == 0 && tmpChar != '\n')
    {
      tmpWord[wordPosition] = tmpChar;
      wordPosition++;
    }
    /*if tempWord has length and is not just whitespace*/
    else if(strlen(tmpWord) > 0)
    {
      strcpy(words[wordCount], tmpWord);

      /*check if all keywords are pressent and in correct order*/
      if(strncmp(tmpWord, "RULES:", 6) == 0 || rulesPressent == 1)
      {
        if(rulesPressent == 0)
        {
          rulesKeywordWordPosition = wordCount;
        }
        rulesPressent = 1;
        if(strncmp(tmpWord, "TURN:", 5) == 0 || turnsPressent == 1)
        {
          if(turnsPressent == 0)
          {
            turnsKeywordWordPosition = wordCount;
          }
          turnsPressent = 1;
          if(strncmp(tmpWord, "BOARD:", 6) == 0 || boardPressent == 1)
          {
            if(boardPressent == 0)
            {
              boardKeywordPosition = i;
            }
            boardPressent = 1;
            if(strncmp(tmpWord, "MOVES:", 6) == 0)
            {
              movesPressent = 1;
              movesKeywordWordPosition = wordCount;
              movesKeywordPosition = i;
            }
          }
        }
      }

      wordCount++;
      wordPosition = 0;
      memset(&tmpWord[0], 0, sizeof(tmpWord));
    }
    i++;
  }

  /*if keywords are wrong print error and stop program*/
  if(rulesPressent != 1 || turnsPressent != 1 || boardPressent != 1 || movesPressent != 1)
  {
    printf("a keyword missing or keywords are in the wrong order\nexiting program\n");
    return 0;
  }

  /*setting capture or no capture for RULES: and seeing if input is invalid for RULES:*/
  if(strncmp(words[rulesKeywordWordPosition+1], "capture", 7) == 0)
  {
    capture = 1;
  }
  else if(strncmp(words[rulesKeywordWordPosition+1], "no", 2) != 0 || strncmp(words[rulesKeywordWordPosition+2], "capture", 7) != 0)
  {
    printf("Invlaid input for RULES: keyword\nexiting program\n");
    return 0;
  }
  if(strncmp(words[rulesKeywordWordPosition+3], "multiple", 9) == 0 || strncmp(words[rulesKeywordWordPosition+4], "multiple", 9) == 0)
  {
    multipuleJumps = 1;
  }
  else if(strncmp(words[rulesKeywordWordPosition+3], "single", 6) != 0 && strncmp(words[rulesKeywordWordPosition+4], "single", 6) != 0)
  {
    printf("jump rule missing\nexiting program\n");
  }


  /*setting turn array based on TURN: input*/
  if(strncmp(words[turnsKeywordWordPosition+1], "red", 3) == 0)
  {
    strcpy(turn, "red");
  }
  else if(strncmp(words[turnsKeywordWordPosition+1], "black", 5) == 0)
  {
    strcpy(turn, "black");
  }
  else
  {
    printf("Invlaid player name for TURNS:\nexiting program\n");
    return 0;
  }

  /*transfer board characters from text array to board 2D array*/
  for(int r = boardKeywordPosition+1; r < movesKeywordPosition-7; r++)
  {
    if(isspace(text[r]) == 0 && text[r] != '\n' && text[r] != '|' && text[r] != '-' && text[r] != '+')
    {
      if(text[r] != '\"' && text[r] != '.' && text[r] != 'b' && text[r] != 'B' && text[r] != 'r' && text[r] != 'R')
      {
        printf("Invlaid character in the board exiting program\n");
        return 0;
      }
      if(text[r] == 'b')
      {
        blackPawns++;
      }
      else if(text[r] == 'B')
      {
        blackKings++;
      }
      else if(text[r] == 'r')
      {
        redPawns++;
      }
      else if(text[r] == 'R')
      {
        redKings++;
      }
      board[boardCount/8][boardCount%8] = text[r];
      boardCount++;
    }
  }

  /*print valid inital board configuration*/
  printf("VALID INPUT\nInital configuration:\nTURN: %s\nRed: %d kings, %d pawns\nBlack %d kings, %d pawns\n", turn, redKings, redPawns, blackKings, blackPawns);

  return 0;
}

void getFileText(FILE* infile,char text[])
{
  char tmpChar = ' ';
  int i = 0;

  while(!feof(infile))
  {
    tmpChar = fgetc(infile);
    text[i] = tmpChar;
    i++;
  }
  text[i-1] = '\0';
}
