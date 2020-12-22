/*Jonathan Kelly jkelly5 616716128*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "info.c"

int main(int argc, char *argv[])
{
  int ctr;
  char* inputSwitch;

  char* eString = NULL;
  char* hString = NULL;
  int inputMoves = -1;
  char* inputString = NULL;
  int usingStdin = 1;

  char text[5000];
  int errorInValidation = 0;

  int amountOfMovesInFile = 0;

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

   if(usingStdin == 1)
   {
     getStdinText(text);
   }
   else
   {
     getFileText(text, inputString);
   }

   errorInValidation = validateText(text, board, moves, turn);
   if(errorInValidation == 1)
   {
     return 1;
   }

   for(int z = 0; moves[z] != "\0"; z++)
   {
     amountOfMovesInFile++;
   }

   if(inputMoves == -1 || inputMoves > amountOfMovesInFile)
   {
     inputMoves = amountOfMovesInFile;
   }

   printf("Input file has %d moves\n", amountOfMovesInFile);
   printf("processing %d moves\n", inputMoves);


/*all before this is setting up*/

    int movesToprocess = inputMoves;
    int illegalMoveFound = 0;
    int currMove;

  for(currMove = 0; currMove < inputMoves; currMove++)
  {
    int initalC = (moves[currMove][0] % 97);
    int initalR = (moves[currMove][1] - '0') - 1;
    int finalC = (moves[currMove][4] % 97);
    int finalR = (moves[currMove][5] - '0') - 1;

    /*board counts rows backwards to human readable so 7 - row gets board readable*/
    int boardInitalR = 7 - initalR;
    int boardFinalR = 7 - finalR;

    int capturePossible = 0;
    int captureTaken = 0;

    /*check if a capture is possible*/
    if(noCapture == 0)
    {
      if(board[boardInitalR][initalC] == 'b' && (board[boardInitalR+1][initalC-1] == 'r' || board[boardInitalR+1][initalC+1] == 'r' || board[boardInitalR+1][initalC-1] == 'R' || board[boardInitalR+1][initalC+1] == 'R'))
      {
          capturePossible = 1;
      }
      else if(board[boardInitalR][initalC] == 'r' && (board[boardInitalR-1][initalC-1] == 'b' || board[boardInitalR-1][initalC+1] == 'b' || board[boardInitalR-1][initalC-1] == 'B' || board[boardInitalR-1][initalC+1] == 'B'))
      {
          capturePossible = 1;
      }
      else if(board[boardInitalR][initalC] == 'R' && (board[boardInitalR-1][initalC-1] == 'b' || board[boardInitalR-1][initalC+1] == 'b' || board[boardInitalR+1][initalC-1] == 'b' || board[boardInitalR+1][initalC-1] == 'b'
                                                   || board[boardInitalR-1][initalC-1] == 'B' || board[boardInitalR-1][initalC+1] == 'B' || board[boardInitalR+1][initalC+1] == 'B' || board[boardInitalR+1][initalC+1] == 'B'))
      {
          capturePossible = 1;
      }
      else if(board[boardInitalR][initalC] == 'B' && (board[boardInitalR-1][initalC-1] == 'r' || board[boardInitalR-1][initalC+1] == 'r' || board[boardInitalR+1][initalC-1] == 'r' || board[boardInitalR+1][initalC-1] == 'r'
                                                   || board[boardInitalR-1][initalC-1] == 'R' || board[boardInitalR-1][initalC+1] == 'R' || board[boardInitalR+1][initalC+1] == 'R' || board[boardInitalR+1][initalC+1] == 'R'))
      {
          capturePossible = 1;
      }
    }

    /* black squares on even columns occur on even rows
     and red squares on odd columns occur on odd rows*/
    if(!(finalR % 2 == 0 && finalC % 2 == 0) && !(finalR % 2 == 1 && finalC % 2 == 1))
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }
    /*check if piece exists at inital square*/
    else if(board[boardInitalR][initalC] == '.' || board[boardInitalR][initalC] == '\"')
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }
    else if(board[boardFinalR][finalC] == 'r' || board[boardFinalR][finalC] == 'R'  || board[boardFinalR][finalC] == 'b'  || board[boardFinalR][finalC] == 'B')
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }

    /*check peice exists when capturing make this the generic capture rule for red, black and kings*/

    /*check if either cordinate moves more than 2*/
    else if(abs(boardFinalR - boardInitalR) > 2 || abs(finalC - initalC) > 2)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }
    /*if only moving in one direction*/
    else if ((abs(boardFinalR - boardInitalR) == 2 && abs(finalC - initalC) == 0)  || (abs(boardFinalR - boardInitalR) == 0 && abs(finalC - initalC) == 2))
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }
    /*check if a red pawn moves backwards*/
    else if(board[boardInitalR][initalC] == 'r' && boardFinalR > boardInitalR)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }
    /*check if a black pawn moves backwards*/
    else if(board[boardInitalR][initalC] == 'b' && boardInitalR > boardFinalR)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }
    /*black or red king captiring downwards*/
    else if((board[boardInitalR][initalC] == 'b' || board[boardInitalR][initalC] == 'B' || board[boardInitalR][initalC] == 'R') && boardFinalR == boardInitalR + 2)
    {
      /*make sure the correct peice is being jumped*/
      if(finalC == (initalC + 2))
      {
        if((board[boardInitalR][initalC] == 'b' || board[boardInitalR][initalC] == 'B') && board[boardInitalR+1][initalC+1] != 'r' && board[boardInitalR+1][initalC+1] != 'R')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else if(board[boardInitalR][initalC] == 'R' && board[boardInitalR+1][initalC+1] != 'b' && board[boardInitalR+1][initalC+1] != 'B')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else
        {
          board[boardInitalR+1][initalC+1] = '.';
          captureTaken = 1;
        }
      }
      else if(finalC == (initalC - 2))
      {
        if((board[boardInitalR][initalC] == 'b' || board[boardInitalR][initalC] == 'B') && board[boardInitalR+1][initalC-1] != 'r' && board[boardInitalR+1][initalC-1] != 'R')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else if(board[boardInitalR][initalC] == 'R' && board[boardInitalR+1][initalC-1] != 'b' && board[boardInitalR+1][initalC-1] != 'B')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else
        {
          board[boardInitalR+1][initalC-1] = '.';
          captureTaken = 1;
        }
      }
    }
    /*red or black king capturing upwards*/
    else if((board[boardInitalR][initalC] == 'r' || board[boardInitalR][initalC] == 'R' || board[boardInitalR][initalC] == 'B') && boardFinalR ==  boardInitalR - 2)
    {
      /*make sure the correct peice is being jumped*/
      if(finalC == (initalC + 2))
      {
        if((board[boardInitalR][initalC] == 'r' || board[boardInitalR][initalC] == 'R') && board[boardInitalR-1][initalC+1] != 'b' && board[boardInitalR-1][initalC+1] != 'B')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else if(board[boardInitalR][initalC] == 'B' && board[boardInitalR-1][initalC+1] != 'r' && board[boardInitalR-1][initalC+1] != 'R')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else
        {
          board[boardInitalR-1][initalC+1] = '.';
          captureTaken = 1;
        }
      }
      else if(finalC == (initalC - 2))
      {
        if((board[boardInitalR][initalC] == 'r' || board[boardInitalR][initalC] == 'R') && board[boardInitalR-1][initalC-1] != 'b' && board[boardInitalR-1][initalC-1] != 'B')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else if(board[boardInitalR][initalC] == 'B' && board[boardInitalR-1][initalC-1] != 'r' && board[boardInitalR-1][initalC-1] != 'R')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToprocess = currMove;
          illegalMoveFound = 1;
        }
        else
        {
          board[boardInitalR-1][initalC-1] = '.';
          captureTaken = 1;
        }
      }
    }

    /*if capture is on, a capture is possible but the capture is not taken and the move is not illegal for anyother reason*/
    if(capturePossible == 1 && captureTaken == 0 && illegalMoveFound == 0)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToprocess = currMove;
      illegalMoveFound = 1;
    }

    /*after checking if move is illegal implemnt to move on board*/
    if(illegalMoveFound == 1)
    {
      break;
    }
    else
    {
      char tmpPiece = board[boardInitalR][initalC];
      board[boardFinalR][finalC] = tmpPiece;
      board[boardInitalR][initalC] = '.';
      /*check if red pawn at row 0*/
      for(int i = 0; i < 8; i++)
      {
        if(board[0][i] == 'r')
        {
          board[0][i] = 'R';
        }
      }
      /*check if red pawn at row 8*/
      for(int i = 0; i < 8; i++)
      {
        if(board[7][i] == 'b')
        {
          board[7][i] = 'B';
        }
      }
      if(strcmp(turn, "red") == 0)
      {
        strcpy(turn, "black");
      }
      else if(strcmp(turn, "black") == 0)
      {
        strcpy(turn, "red");
      }
    }
  }

  if(illegalMoveFound != 1)
  {
    printf("All moves are legal\n");
  }

  if(eString != NULL)
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

  if(hString != NULL)
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

    if(illegalMoveFound)
    {
      fprintf(hfile, "\n%s\n", moves[currMove]);
    }

    fclose(hfile);
  }



/*all after this is cleaning up*/

   /*free all variables*/
   free(turn);
   for(int freeCounter = 0; freeCounter < 8; freeCounter++)
   {
     free(board[freeCounter]);
   }
   free(board);
   free(moves);

   return 0;
}
