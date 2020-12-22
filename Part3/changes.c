/*Jonathan Kelly jkelly5 616716128*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int validateMoves(int inputMoves, char *turn, char **board, char **moves, int noCapture, int multipuleJumps, int currMove)
{

  int movesToProcess = inputMoves;

  for(; currMove < inputMoves; currMove++)
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
      movesToProcess = currMove;
      return 1;
    }
    /*check if piece exists at inital square*/
    else if(board[boardInitalR][initalC] == '.' || board[boardInitalR][initalC] == '\"')
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
    }
    else if(board[boardFinalR][finalC] == 'r' || board[boardFinalR][finalC] == 'R'  || board[boardFinalR][finalC] == 'b'  || board[boardFinalR][finalC] == 'B')
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
    }

    /*check peice exists when capturing make this the generic capture rule for red, black and kings*/

    /*check if either cordinate moves more than 2*/
    else if(abs(boardFinalR - boardInitalR) > 2 || abs(finalC - initalC) > 2)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
    }
    /*if only moving in one direction*/
    else if ((abs(boardFinalR - boardInitalR) == 2 && abs(finalC - initalC) == 0)  || (abs(boardFinalR - boardInitalR) == 0 && abs(finalC - initalC) == 2))
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
    }
    /*check if a red pawn moves backwards*/
    else if(board[boardInitalR][initalC] == 'r' && boardFinalR > boardInitalR)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
    }
    /*check if a black pawn moves backwards*/
    else if(board[boardInitalR][initalC] == 'b' && boardInitalR > boardFinalR)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
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
          movesToProcess = currMove;
          return 1;
        }
        else if(board[boardInitalR][initalC] == 'R' && board[boardInitalR+1][initalC+1] != 'b' && board[boardInitalR+1][initalC+1] != 'B')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToProcess = currMove;
          return 1;
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
          movesToProcess = currMove;
          return 1;
        }
        else if(board[boardInitalR][initalC] == 'R' && board[boardInitalR+1][initalC-1] != 'b' && board[boardInitalR+1][initalC-1] != 'B')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToProcess = currMove;
          return 1;
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
          movesToProcess = currMove;
          return 1;
        }
        else if(board[boardInitalR][initalC] == 'B' && board[boardInitalR-1][initalC+1] != 'r' && board[boardInitalR-1][initalC+1] != 'R')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToProcess = currMove;
          return 1;
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
          movesToProcess = currMove;
          return 1;
        }
        else if(board[boardInitalR][initalC] == 'B' && board[boardInitalR-1][initalC-1] != 'r' && board[boardInitalR-1][initalC-1] != 'R')
        {
          printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
          movesToProcess = currMove;
          return 1;
        }
        else
        {
          board[boardInitalR-1][initalC-1] = '.';
          captureTaken = 1;
        }
      }
    }

    /*if capture is on, a capture is possible but the capture is not taken and the move is not illegal for anyother reason*/
    if(capturePossible == 1 && captureTaken == 0)
    {
      printf("move %d is illegal: %s\n", currMove+1, moves[currMove]);
      movesToProcess = currMove;
      return 1;
    }

    /*after checking if move is illegal implemnt to move on board*/
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

   return 0;
}
