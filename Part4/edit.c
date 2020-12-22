/*Jonathan Kelly jkelly5 616716128*/
#include <termbox.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "changes.c"
#include "info.c"

void outputToHFile(char *hString, char **board, char **moves, int illegalMoveFound, int currMove);
void outputToEFile(char *eString, int noCapture, int multipuleJumps, char *turn, char **board, char **moves, int currMove, int amountOfMovesInFile);

int main(int argc, char *argv[])
{
  char text[5000];
  char *turn = malloc(6 * sizeof(char*));
  char **board = (char**)malloc(8*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 8; mallocCounter++)
  {
    board[mallocCounter] = malloc(8 * sizeof(char*));
  }

  char **moves = (char**)malloc(150*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 150; mallocCounter++)
  {
    moves[mallocCounter] = malloc(6 * sizeof(char*));
  }


  char* eString = NULL;
  char* hString = NULL;
  int inputMoves = -1;
  int depth = 0;
  int verboseFound = 0;
  char* inputString = NULL;
  int usingStdin = 1;
  int noCapture = 0;
  int multipuleJumps = 0;


  /*get input switches from the terminal*/
  for(int ctr=0; ctr < argc; ctr++ )
  {
    char* inputSwitch = argv[ctr];
    int len = strlen(inputSwitch);
    char *last_four = &inputSwitch[len-4];

    if(strcmp(inputSwitch, "-e") == 0)
    {
      eString = argv[ctr+1];
      ctr++;
    }
    else if(strcmp(inputSwitch, "-h") == 0)
    {
      hString = argv[ctr+1];
      ctr++;
    }
    else if(strcmp(inputSwitch, "-m") == 0)
    {
      inputMoves = atoi(argv[ctr+1]);
      ctr++;
    }
    else if(strcmp(last_four, ".txt") == 0)
    {
      inputString = argv[ctr];
      usingStdin = 0;
    }
  }

  /*get text using stdinput or file input*/
  if(usingStdin == 1)
  {
    getStdinText(text);
    inputString = "stdin";
  }
  else
  {
    getFileText(text, inputString);
  }

  int errorInValidation = 0;
  /*validate the text and put it into the board, moves and turn arrays*/
  errorInValidation = validateText(text, board, moves, turn, &noCapture, &multipuleJumps);
  /*if an error is returned from validateText exit the program*/
  if(errorInValidation == 1)
  {
    return 1;
  }

  /*get amount of moves from input*/
  int amountOfMovesInFile = 0;
  int currMove = 0;
  int illegalMoveFound = 999;
  for(int z = 0; moves[z] != "\0"; z++)
  {
    amountOfMovesInFile++;
  }
  if(inputMoves == -1 || inputMoves > amountOfMovesInFile)
  {
    inputMoves = amountOfMovesInFile;
  }

  /* deep copy of board*/
  char **boardCopy = (char**)malloc(8*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 8; mallocCounter++)
  {
    boardCopy[mallocCounter] = malloc(8 * sizeof(char*));
  }
  for(int i = 0; i < 8; i++)
  {
    memcpy(boardCopy[i], board[i], sizeof(char)*8);
  }

  char *turnCopy = (char*)malloc(6*(sizeof(char)));
  strcpy(turnCopy, turn);

  for(int i = 0; illegalMoveFound == 999 && i < inputMoves; i++)
  {
    illegalMoveFound = validateMoves(turnCopy, boardCopy, moves[i], noCapture, i);
  }

  tb_init();

  int cx = 0;
  int cy = 0;

  char* exitStatment = malloc(12 * sizeof(char));
  char* arrowKeysStatment = malloc(35 * sizeof(char));
  char* player1Statment = malloc(6 * sizeof(char));
  char* player2Statment = malloc(6 * sizeof(char));

  strcpy(exitStatment, "Ctrl+Q: quit");
  strcpy(arrowKeysStatment, "left, right: move cursor");

  strcpy(player1Statment, turn);
  if(strcmp(turn, "red") == 0)
  {
      strcpy(player2Statment, "black");
  }
  else
  {
    strcpy(player2Statment, "red");
  }

  for(int i = 0; i < tb_width(); i++)
  {
    tb_change_cell(cx++, 0, ' ', TB_BLACK, TB_CYAN);
  }

  cx = 10;
  for(int i = 0; inputString[i] != '\0'; i++)
  {
    tb_change_cell(cx++, 0, inputString[i], TB_BLACK, TB_CYAN);
  }

  cx = 0;
  for(int i = 0; i < 25; i++)
  {
    tb_change_cell(cx++, 1, arrowKeysStatment[i], TB_WHITE, TB_DEFAULT);
  }

  cx = 0;
  for(int i = 0; i < 12; i++)
  {
    tb_change_cell(cx++, 2, exitStatment[i], TB_WHITE, TB_DEFAULT);
  }

  cx = 0;
  if(strcmp(player1Statment, "black") == 0)
  {
    for(int i = 0; i < 7; i++)
    {
      tb_change_cell(cx++, 5, player1Statment[i], TB_WHITE, TB_DEFAULT);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
    {
      tb_change_cell(cx++, 5, player1Statment[i], TB_WHITE, TB_DEFAULT);
    }
  }

  if(strcmp(player2Statment, "red") == 0)
  {
    cx = (tb_width()-44);
    for(int i = 0; i < 4; i++)
    {
      tb_change_cell(cx++, 5, player2Statment[i], TB_WHITE, TB_DEFAULT);
    }
  }
  else
  {
    cx = (tb_width()-46);
    for(int i = 0; i < 6; i++)
    {
      tb_change_cell(cx++, 5, player2Statment[i], TB_WHITE, TB_DEFAULT);
    }
  }





  uint16_t bgColor = TB_WHITE;
  int widthDiffrence = ((tb_width()-40)/2);

  int cursX;
  int cursY;
  int moveNumber;
  int illegalMovePrinted;
  uint16_t textColor;
  int moveSeleted = 0;
  int movePermormed = 0;

  char **tempBoard = (char**)malloc(8*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 8; mallocCounter++)
  {
    tempBoard[mallocCounter] = malloc(8 * sizeof(char*));
  }

for(;;)
{

  cursX = 0;
  cursY = 7;
  moveNumber = 0;
  illegalMovePrinted = 0;
  textColor = TB_GREEN;



  /*prints moves and selectes color by where the illegal move is if it exists*/
  for(; moveNumber <= amountOfMovesInFile;)
  {
    if(illegalMoveFound == moveNumber)
    {
      textColor = TB_RED;
      illegalMovePrinted = 1;
    }
    else if(illegalMovePrinted == 1)
    {
      textColor = TB_WHITE;
    }
    else
    {
      textColor = TB_GREEN;
    }


    cursX = 0;
    if(moveNumber == moveSeleted)
    {
      if(moveSeleted == amountOfMovesInFile)
      {
        if(illegalMoveFound == 999)
        {
          performMoves(tempBoard, moves, moveSeleted);
        }
        else
        {
          performMoves(tempBoard, moves, illegalMoveFound);
        }
        for(int b = 0; b < widthDiffrence; b++)
        {
          tb_change_cell(cursX++, cursY, ' ', TB_BLACK, TB_GREEN);
        }
      }
      else if(moveNumber >= illegalMoveFound)
      {
        performMoves(tempBoard, moves, illegalMoveFound);
      }
      else
      {
        performMoves(tempBoard, moves, moveNumber);
      }

      cursX = 0;
      for(int b = 0; b < widthDiffrence; b++)
      {
        tb_change_cell(cursX++, cursY, ' ', TB_BLACK, TB_GREEN);
      }
      if(moveNumber != amountOfMovesInFile)
      {
        cursX = 0;
        for(int b = 0; b < 6; b++)
        {
          tb_change_cell(cursX++, cursY, moves[moveNumber][b], TB_BLACK, TB_GREEN);
        }
      }
    }
    else
    {
      for(int b = 0 ; b < widthDiffrence; b++)
      {
        tb_change_cell(cursX++, cursY, ' ', TB_DEFAULT, TB_DEFAULT);
      }
      if(moveNumber != amountOfMovesInFile)
      {
        cursX = 0;
        for(int b = 0; b < 6; b++)
        {
          tb_change_cell(cursX++, cursY, moves[moveNumber][b], textColor, TB_DEFAULT);
        }
      }
    }

    moveNumber++;

    if(illegalMoveFound == moveNumber)
    {
      textColor = TB_RED;
      illegalMovePrinted = 1;
    }
    else if(illegalMovePrinted == 1)
    {
      textColor = TB_WHITE;
    }
    else
    {
      textColor = TB_GREEN;
    }

    cursX = widthDiffrence;
    if(moveNumber <= amountOfMovesInFile)
    {
      if(moveNumber == moveSeleted)
      {
        if(moveSeleted == amountOfMovesInFile)
        {
          if(illegalMoveFound == 999)
          {
            performMoves(tempBoard, moves, moveSeleted);
          }
          else
          {
            performMoves(tempBoard, moves, illegalMoveFound);
          }
          for(int b = widthDiffrence; b < tb_width()-41; b++)
          {
            tb_change_cell(cursX++, cursY, ' ', TB_BLACK, TB_GREEN);
          }
        }
        else if(moveNumber >= illegalMoveFound)
        {
          performMoves(tempBoard, moves, illegalMoveFound);
        }
        else
        {
          performMoves(tempBoard, moves, moveNumber);
        }

        if(moveSeleted != amountOfMovesInFile)
        {
          for(int b = widthDiffrence; b < tb_width()-41; b++)
          {
            tb_change_cell(cursX++, cursY, ' ', TB_BLACK, TB_GREEN);
          }
          if (moveNumber != amountOfMovesInFile)
          {
            cursX = tb_width()-47;
            for(int b = 0; b < 6; b++)
            {
              tb_change_cell(cursX++, cursY, moves[moveNumber][b], TB_BLACK, TB_GREEN);
            }
          }
        }
      }
      else
      {
        for(int b = widthDiffrence; b < tb_width()-41; b++)
        {
          tb_change_cell(cursX++, cursY, ' ', TB_DEFAULT, TB_DEFAULT);
        }
        if (moveNumber != amountOfMovesInFile)
        {
          cursX = tb_width()-47;
          for(int b = 0; b < 6; b++)
          {
            tb_change_cell(cursX++, cursY, moves[moveNumber][b], textColor, TB_DEFAULT);
          }
        }
      }
      cursY++;
    }
    moveNumber++;
  }

  cx = tb_width()-1;
  cy = 1;
  for(int r = 0; r < 8; r++)
  {
    if(bgColor == TB_WHITE)
    {
      bgColor = TB_RED;
    }
    else
    {
      bgColor = TB_WHITE;
    }
    for(int c = 0; c < 8; c++)
    {
      if(bgColor == TB_WHITE)
      {
        bgColor = TB_RED;
      }
      else
      {
        bgColor = TB_WHITE;
      }

      for(int i = 0; i < 3; i++)
      {
        for(int a = 0; a < 5; a++)
        {
          if(i == 1 && a == 2)
          {
            if(tempBoard[r][7-c] == 'r' || tempBoard[r][7-c] == 'R')
            {
              tb_change_cell(cx--, cy, tempBoard[r][7-c], TB_RED, bgColor);
            }
            else if(tempBoard[r][7-c] == 'b' || tempBoard[r][7-c] == 'B')
            {
              tb_change_cell(cx--, cy, tempBoard[r][7-c], TB_BLACK, bgColor);
            }
            else
            {
              tb_change_cell(cx--, cy, ' ', TB_DEFAULT, bgColor);
            }
          }
          else
          {
            tb_change_cell(cx--, cy, ' ', TB_DEFAULT, bgColor);
          }
        }
        tb_change_cell(cx+=5, cy++, ' ', TB_DEFAULT, bgColor);
        if(i == 2)
        {
            cx-=5;
            cy-=3;
        }
      }
    }
    cx = tb_width()-1;
    cy+=3;
  }


  tb_present();

  for(int i = 0; i < 8; i++)
  {
    memcpy(tempBoard[i], board[i], sizeof(char)*8);
  }

  struct tb_event event;
  tb_peek_event(&event, 50);
  if (TB_KEY_CTRL_Q == event.key)
  {
      tb_shutdown();
      break;
  }

    if (TB_KEY_ARROW_RIGHT == event.key)
    {
      if(moveSeleted != amountOfMovesInFile)
      {
        moveSeleted++;
      }
    }
    else if (TB_KEY_ARROW_LEFT == event.key)
    {
      if(moveSeleted != 0)
      {
        moveSeleted--;
      }
    }
  }

  /*out put to either e or h file*/
  if(eString != NULL)
  {
    outputToEFile(eString, noCapture, multipuleJumps, turn, board, moves, currMove, amountOfMovesInFile);
  }
  if(hString != NULL)
  {
    outputToHFile(hString, board, moves, illegalMoveFound, currMove);
  }

  /*free all variables*/
  free(turn);
  for(int freeCounter = 0; freeCounter < 8; freeCounter++)
  {
    free(board[freeCounter]);
  }
  free(board);
  for(int freeCounter = 0; freeCounter < 8; freeCounter++)
  {
    free(boardCopy[freeCounter]);
  }
  free(boardCopy);
  for(int freeCounter = 0; freeCounter < 8; freeCounter++)
  {
    free(tempBoard[freeCounter]);
  }
  free(tempBoard);
  for(int freeCounter = 0; freeCounter < amountOfMovesInFile; freeCounter++)
  {
    free(moves[freeCounter]);
  }
  free(moves);
  free(arrowKeysStatment);
  free(exitStatment);

  return 0;
}

void outputToHFile(char *hString,char **board, char **moves, int illegalMoveFound, int currMove)
{
  FILE *hfile;
  hfile = fopen(hString, "w");

  fprintf(hfile, "    a   b   c   d   e   f   g   h\n");
  fprintf(hfile, "  +---+---+---+---+---+---+---+---+\n");

  for(int r = 0; r < 8; r++)
  {
    fprintf(hfile, "%d |", (8-r));
    for(int c = 0; c < 8; c++)
    {
      fprintf(hfile, " %c |", board[r][c]);
    }
    fprintf(hfile, " %d\n", (8-r));
    fprintf(hfile, "  +---+---+---+---+---+---+---+---+\n");
  }

  fprintf(hfile, "    a   b   c   d   e   f   g   h\n");

  if(illegalMoveFound != -1)
  {
    fprintf(hfile, "\n%s\n", moves[illegalMoveFound]);
  }

  fclose(hfile);
}

void outputToEFile(char *eString, int noCapture, int multipuleJumps, char *turn, char **board, char **moves, int currMove, int amountOfMovesInFile)
{
  FILE *efile;
  efile = fopen(eString, "w");

  fprintf(efile, "RULES: ");
  if(noCapture == 0)
  {
    fprintf(efile, "capture ");
  }
  else
  {
    fprintf(efile, "no capture ");
  }

  if(multipuleJumps == 0)
  {
    fprintf(efile, "single jumps ");
  }
  else
  {
    fprintf(efile, "multiple jumps ");
  }

  fprintf(efile, "TURN: %s BOARD:\n", turn);

  for(int r = 0; r < 8; r++)
  {
    for(int c = 0; c < 8; c++)
    {
      fprintf(efile, "%c", board[r][c]);
    }
  }
  fprintf(efile, "\nMOVES: ");

  for(int i = currMove; i < amountOfMovesInFile; i++)
  {
    fprintf(efile, "%s ", moves[i]);
  }
  fprintf(efile, "\n");

  fclose(efile);
}
