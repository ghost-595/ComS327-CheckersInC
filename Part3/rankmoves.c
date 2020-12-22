/*Jonathan Kelly jkelly5 616716128*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "changes.c"
#include "info.c"

void outputToHFile(char *hString, char **board, char **moves, int illegalMoveFound, int currMove);
void outputToEFile(char *eString, int noCapture, int multipuleJumps, char *turn, char **board, char **moves, int currMove, int amountOfMovesInFile);
void findAvalibleMoves(int r, int c, char *turn, char **board, char **avalibleMoves, int *movesCount);
int findAllAvalibleMoves(int depth, char *move, char **board, char *turn, int verboseFound, int maxDepth);
void performMove(char *move, char **board, char *turn);
int scoreBoard(char **board, char *turn);
char* convertCordinate(char *avalibleMove);

int main(int argc, char *argv[])
{
  char text[5000];
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


  char* eString = NULL;
  char* hString = NULL;
  int inputMoves = -1;
  int depth = 0;
  int verboseFound = 0;
  char* inputString = NULL;
  int usingStdin = 1;


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
    else if(strcmp(inputSwitch, "-d") == 0)
    {
      depth = atoi(argv[ctr+1]);
      ctr++;
    }
    else if(strcmp(inputSwitch, "-v") == 0)
    {
      verboseFound = 1;
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
  }
  else
  {
    getFileText(text, inputString);
  }

  int errorInValidation = 0;
  /*validate the text and put it into the board, moves and turn arrays*/
  errorInValidation = validateText(text, board, moves, turn);
  /*if an error is returned from validateText exit the program*/
  if(errorInValidation == 1)
  {
    return 1;
  }

  /*get amount of moves from input*/
  int amountOfMovesInFile = 0;
  int currMove = 0;
  int illegalMoveFound = 0;
  for(int z = 0; moves[z] != "\0"; z++)
  {
    amountOfMovesInFile++;
  }
  if(inputMoves == -1 || inputMoves > amountOfMovesInFile)
  {
    inputMoves = amountOfMovesInFile;
  }
  /*Part 3 does not process moves from input*/
  if(amountOfMovesInFile != 0)
  {
    fprintf(stderr, "moves found in the input file, they will not be processed\n\n");
  }
  else
  {
    illegalMoveFound = validateMoves(inputMoves, turn, board, moves, noCapture, multipuleJumps, currMove);
  }

  char **avalibleMoves = (char**)malloc(48*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 48; mallocCounter++)
  {
    avalibleMoves[mallocCounter] = malloc(6 * sizeof(char*));
  };

  int movesCount = 0;

  /*finds all players peices in alphabetic order*/
  for(int c = 0; c < 8; c++)
  {
    for(int r = 7; r >= 0; r--)
    {
      findAvalibleMoves(r, c, turn, board, avalibleMoves, &movesCount);
      while(avalibleMoves[movesCount][0] != NULL)
      {
        movesCount++;
      }
    }
  }

  /* deep copy of board*/
  char **newBoard = (char**)malloc(8*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 8; mallocCounter++)
  {
    newBoard[mallocCounter] = malloc(8 * sizeof(char*));
  }

  for(int i = 0; i < 8; i++)
  {
    memcpy(newBoard[i], board[i], sizeof(char)*8);
  }

  char *initalTurn = malloc(6 * sizeof(char*));
  int maxDepth = depth;
  strcpy(initalTurn, turn);

  int *scores = malloc(movesCount * sizeof(int*));
  char *move = malloc(7 * sizeof(char*));

  for (int i = 0; i < movesCount; i++)
  {
    if(verboseFound == 1 && depth > 0)
    {
      move = convertCordinate(avalibleMoves[i]);
      printf("? %s for %s:\n", move, turn);
    }
    scores[i] = findAllAvalibleMoves(depth, avalibleMoves[i], newBoard, turn, verboseFound, maxDepth);
    strcpy(turn, initalTurn);

    move = convertCordinate(avalibleMoves[i]);
    if(verboseFound == 1)
    {
        printf(". %s for %s: score %d\n", move, turn, scores[i]);
    }
    else
    {
        printf("%s for %s: score %d\n", move, turn, scores[i]);
    }
  }

  free(move);



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
  for(int freeCounter = 0; freeCounter < 48; freeCounter++)
  {
    free(avalibleMoves[freeCounter]);
  }
  free(avalibleMoves);
  free(moves);
  free(initalTurn);

  return 0;
}

int findAllAvalibleMoves(int depth, char *move, char **board, char *turn, int verboseFound, int maxDepth)
{

  /*deep copy of board and malloc avalibleMoves*/
  char **newBoard = (char**)malloc(8*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 8; mallocCounter++)
  {
    newBoard[mallocCounter] = malloc(8 * sizeof(char*));
  }
  for(int i = 0; i < 8; i++)
  {
    memcpy(newBoard[i], board[i], sizeof(char)*8);
  }
  char **avalibleMoves = (char**)malloc(48*(sizeof(char*)));
  for(int mallocCounter = 0; mallocCounter < 48; mallocCounter++)
  {
    avalibleMoves[mallocCounter] = malloc(6 * sizeof(char*));
  };

  performMove(move, newBoard, turn);

  if(depth == 0)
  {
    int score = scoreBoard(newBoard, turn);
    return score;
  }

  if(strcmp(turn, "red") == 0)
  {
    strcpy(turn, "black");
  }
  else if(strcmp(turn, "black") == 0)
  {
    strcpy(turn, "red");
  }

  int movesCount = 0;
  /* find all moves a single piece*/
  for(int c = 0; c < 8; c++)
  {
    for(int r = 7; r >= 0; r--)
    {
      findAvalibleMoves(r, c, turn, newBoard, avalibleMoves, &movesCount);
    }
  }

  int score = -99;
  int newScore = -100;

  for(int i = 0; i < movesCount; i++)
  {
    if(verboseFound == 1 && depth > 1)
    {
      for( int z = depth; z < maxDepth+1; z++)
      {
        printf("\t");
      }
      printf("? %s for %s:\n", convertCordinate(avalibleMoves[i]), turn);
    }
    newScore = findAllAvalibleMoves(depth-1, avalibleMoves[i], newBoard, turn, verboseFound, maxDepth);

    if(verboseFound == 1)
    {
      for( int z = depth; z < maxDepth+1; z++)
      {
        printf("\t");
      }
      printf(". %s for %s: score %d\n", convertCordinate(avalibleMoves[i]), turn, newScore);
    }

    if(newScore > score)
    {
      score = newScore;
    }
  }

  score *= -1;

  /*free variables once done*/
  for(int freeCounter = 0; freeCounter < 48; freeCounter++)
  {
    free(avalibleMoves[freeCounter]);
  }
  free(avalibleMoves);
  for(int freeCounter = 0; freeCounter < 8; freeCounter++)
  {
    free(newBoard[freeCounter]);
  }
  free(newBoard);

  return score;
}

void performMove(char *move, char **board, char *turn)
{

  int initalC = (move[1] - '0');
  int initalR = (move[0] - '0');
  int finalC = (move[4] - '0');
  int finalR = (move[3] - '0');

  board[finalR][finalC] = board[initalR][initalC];
  board[initalR][initalC] = '.';

  if(abs(finalR - initalR) == 2)
  {
    if((finalR == initalR - 2) && (finalC == initalC + 2 ))
    {
      board[initalR-1][initalC+1] = '.';
    }
    else if((finalR == initalR - 2) && (finalC == initalC - 2 ))
    {
      board[initalR-1][initalC-1] = '.';
    }
    else if((finalR == initalR + 2) && (finalC == initalC - 2 ))
    {
      board[initalR+1][initalC-1] = '.';
    }
    else if((finalR == initalR + 2) && (finalC == initalC + 2 ))
    {
      board[initalR+1][initalC+1] = '.';
    }
  }

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
}

int scoreBoard(char **board, char *turn)
{

  int score = 0;

  int redPawns = 0;
  int redKings = 0;
  int blackPawns = 0;
  int blackKings = 0;

  char piece;

  for(int r = 0; r < 8; r++)
  {
    for(int c = 0; c < 8; c++)
    {
      piece = board[r][c];
      if(piece == 'r')
      {
        redPawns++;
      }
      else if(piece == 'R')
      {
        redKings++;
      }
      else if(piece == 'b')
      {
        blackPawns++;
      }
      else if(piece == 'B')
      {
        blackKings++;
      }
    }
  }

  if(redPawns == 0 && redKings == 0 && blackPawns == 0 && blackKings == 0)
  {
    return 0;
  }

  if(redPawns == 0 && redKings == 0)
  {
    score = -99;
  }
  else if(blackPawns == 0 && blackKings == 0)
  {
    score = 99;
  }
  else
  {
    score = ((redPawns + (2*redKings)) - (blackPawns + (2*blackKings)));
  }

  if(strcmp("black", turn) == 0)
  {
    score *= -1;
  }

  return score;
}

void findAvalibleMoves(int r, int c, char *turn, char **board, char **avalibleMoves, int *movesCount)
{
  int ctrlVarR = 0;
  int ctrlVarC = 0;
  if(strcmp(turn, "red") == 0)
  {
    if(board[r][c] == 'r')
    {
      for(int i = 0; i < 2; i++)
      {
        if(i == 0)
        {
          ctrlVarC = -1;
        }
        else if(i == 1)
        {
          ctrlVarC = 1;
        }

        char charC = c + '0';
        char charR = r + '0';
        char finalC = c +'0' + ctrlVarC;
        char finalR = r +'0' - 1;

        char tempMove[5] = "";

        tempMove[0] = charR;
        tempMove[1] = charC;
        tempMove[2] = ',';
        tempMove[3] = finalR;
        tempMove[4] = finalC;
        tempMove[5] = '\0';

        if((c == 0 && ctrlVarC == -1) || (c == 7 && ctrlVarC == 1) || r == 0)
        {
          continue;
        }

        if(board[r - 1][c + ctrlVarC] == 'r' || board[r - 1][c + ctrlVarC] == 'R')
        {
          continue;
        }

        if(board[r - 1][c + ctrlVarC] == 'b' || board[r - 1][c + ctrlVarC] == 'B')
        {
          if(!((r-2) > -1) || !((c+(ctrlVarC*2)) < 8 && (c+(ctrlVarC*2) > -1)))
          {
            continue;
          }
          else if (board[(r-2)][c+(ctrlVarC*2)] != '.')
          {
            continue;
          }
          else
          {
            tempMove[3] -= 1;
            tempMove[4] += ctrlVarC;
          }
        }

        strcpy(avalibleMoves[*movesCount], tempMove);
        *movesCount = *movesCount + 1;
      }
    }
    /*end of piece r*/
    if(board[r][c] == 'R')
    {
      for(int i = 0; i < 4; i++)
      {
        if(i == 0)
        {
          ctrlVarC = -1;
          ctrlVarR = 1;
        }
        else if(i == 1)
        {
          ctrlVarC = -1;
          ctrlVarR = -1;
        }
        else if(i == 2)
        {
          ctrlVarC = 1;
          ctrlVarR = 1;
        }
        else if(i == 3)
        {
          ctrlVarC = 1;
          ctrlVarR = -1;
        }

        char charC = c + '0';
        char charR = r + '0';
        char finalC = c +'0' + ctrlVarC;
        char finalR = r +'0' + ctrlVarR;

        char tempMove[5] = "";

        tempMove[0] = charR;
        tempMove[1] = charC;
        tempMove[2] = ',';
        tempMove[3] = finalR;
        tempMove[4] = finalC;
        tempMove[5] = '\0';

        if((c == 0 && ctrlVarC == -1) || (c == 7 && ctrlVarC == 1) || (r == 0 && ctrlVarR == -1) || (r == 7 && ctrlVarR == 1))
        {
          continue;
        }

        if(board[r + ctrlVarR][c + ctrlVarC] == 'r' || board[r + ctrlVarR][c + ctrlVarC] == 'R')
        {
          continue;
        }

        /*if square trying to go to can be jumped*/
        if(board[r + ctrlVarR][c + ctrlVarC] == 'b' || board[r + ctrlVarR][c + ctrlVarC] == 'B')
        {
          /*if jump is in bound*/
          if(!((r+(ctrlVarR*2)) < 8 && (r+(ctrlVarR*2)) > -1) || !((c+(ctrlVarC*2)) < 8 && (c+(ctrlVarC*2)) > -1))
          {
            continue;
          }
          /*if sqaure jumping to is an empty black square*/
          else if(board[(r+2)][c+(ctrlVarC*2)] != '.')
          {
            continue;
          }
          else
          {
            tempMove[3] += ctrlVarR;
            tempMove[4] += ctrlVarC;
            if(i == 1)
            {
              /*if same peice as previous move and pararell corner can not jump either*/
              if(avalibleMoves[*movesCount-1][0] == tempMove[0] && avalibleMoves[*movesCount-1][1] == tempMove[1] && (board[r+1][c-1] != 'b' && board[r+1][c-1] != 'B'))
              {
                  char* moveSwitch = malloc(5 * sizeof(char*));
                  strcpy(moveSwitch, avalibleMoves[*movesCount-1]);
                  strcpy(avalibleMoves[*movesCount-1], tempMove);
                  strcpy(avalibleMoves[*movesCount], moveSwitch);
                  *movesCount = *movesCount + 1;
                  continue;
              }
            }
            else if(i == 2)
            {
              /*if same peice as previous move and pararell corner can not jump either*/
              if(avalibleMoves[*movesCount-1][0] == tempMove[0] && avalibleMoves[*movesCount-1][1] == tempMove[1] && (board[r-1][c+1] != 'b' && board[r-1][c+1] != 'B'))
              {
                  strcpy(avalibleMoves[*movesCount+1], tempMove);
                  continue;
              }
            }
          }

        }

        strcpy(avalibleMoves[*movesCount], tempMove);
        *movesCount = *movesCount + 1;
      }
    }

  }
  else if(strcmp(turn, "black") == 0)
  {
    if(board[r][c] == 'b')
    {
      for(int i = 0; i < 2; i++)
      {
        if(i == 0)
        {
          ctrlVarC = -1;
        }
        else if(i == 1)
        {
          ctrlVarC = 1;
        }

        char charC = c + '0';
        char charR = r + '0';
        char finalC = c +'0' + ctrlVarC;
        char finalR = r +'0' + 1;

        char tempMove[5] = "";

        tempMove[0] = charR;
        tempMove[1] = charC;
        tempMove[2] = ',';
        tempMove[3] = finalR;
        tempMove[4] = finalC;
        tempMove[5] = '\0';

        if((c == 7 && ctrlVarC == 1) || (c == 0 && ctrlVarC == -1) || r == 7)
        {
          continue;
        }

        if(board[r + 1][c + ctrlVarC] == 'b' || board[r + 1][c + ctrlVarC] == 'B')
        {
          continue;
        }

        if(board[r + 1][c + ctrlVarC] == 'r' || board[r + 1][c + ctrlVarC] == 'R')
        {
          if(!((r+2) < 8) || !((c+(ctrlVarC*2)) < 8 && (c+(ctrlVarC*2) > -1)))
          {
            continue;
          }
          else if(board[(r+2)][c+(ctrlVarC*2)] != '.')
          {
            continue;
          }
          tempMove[3] += 1;
          tempMove[4] += ctrlVarC;
        }

        strcpy(avalibleMoves[*movesCount], tempMove);
        *movesCount = *movesCount + 1;
      }
    }
    /*end of piece b*/
    if(board[r][c] == 'B')
    {
      for(int i = 0; i < 4; i++)
      {
        if(i == 0)
        {
          ctrlVarC = -1;
          ctrlVarR = 1;
        }
        else if(i == 1)
        {
          ctrlVarC = -1;
          ctrlVarR = -1;
        }
        else if(i == 2)
        {
          ctrlVarC = 1;
          ctrlVarR = 1;
        }
        else if(i == 3)
        {
          ctrlVarC = 1;
          ctrlVarR = -1;
        }

        char charC = c + '0';
        char charR = r + '0';
        char finalC = c +'0' + ctrlVarC;
        char finalR = r +'0' + ctrlVarR;

        char tempMove[5] = "";

        tempMove[0] = charR;
        tempMove[1] = charC;
        tempMove[2] = ',';
        tempMove[3] = finalR;
        tempMove[4] = finalC;
        tempMove[5] = '\0';

        if((c == 0 && ctrlVarC == -1) || (c == 7 && ctrlVarC == 1) || (r == 0 && ctrlVarR == -1) || (r == 7 && ctrlVarR == 1))
        {
          continue;
        }

        if(board[r + ctrlVarR][c + ctrlVarC] == 'b' || board[r + ctrlVarR][c + ctrlVarC] == 'B')
        {
          continue;
        }

        if(board[r + ctrlVarR][c + ctrlVarC] == 'r' || board[r + ctrlVarR][c + ctrlVarC] == 'R')
        {
          if(!((r+(ctrlVarR*2)) < 8 && (r+(ctrlVarR*2)) > -1) || !((c+(ctrlVarC*2)) < 8 && (c+(ctrlVarC*2)) > -1))
          {
            continue;
          }
          else if(board[r+(ctrlVarR*2)][c+(ctrlVarC*2)] != '.')
          {
            continue;
          }
          else
          {
            tempMove[3] += ctrlVarR;
            tempMove[4] += ctrlVarC;
            if(i == 1)
            {
              /*if same peice as previous move and pararell corner can not jump either*/
              if(avalibleMoves[*movesCount-1][0] == tempMove[0] && avalibleMoves[*movesCount-1][1] == tempMove[1] && (board[r+1][c-1] != 'r' && board[r+1][c-1] != 'R'))
              {
                  char* moveSwitch = malloc(5 * sizeof(char*));
                  strcpy(moveSwitch, avalibleMoves[*movesCount-1]);
                  strcpy(avalibleMoves[*movesCount-1], tempMove);
                  strcpy(avalibleMoves[*movesCount], moveSwitch);
                  *movesCount = *movesCount + 1;
                  continue;
              }
            }
            else if(i == 2)
            {
              if(avalibleMoves[*movesCount-1][0] == tempMove[0] && avalibleMoves[*movesCount-1][1] == tempMove[1] && (board[r-1][c+1] != 'r' && board[r-1][c+1] != 'R'))
              {
                  strcpy(avalibleMoves[*movesCount+1], tempMove);
                  continue;
              }
            }
          }
        }

        strcpy(avalibleMoves[*movesCount], tempMove);
        *movesCount = *movesCount + 1;
      }
    }
  }
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

  if(illegalMoveFound)
  {
    fprintf(hfile, "\n%s\n", moves[currMove]);
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

char* convertCordinate(char *avalibleMove)
{
  char *move = malloc(7 * sizeof(char*));

    move[0] = avalibleMove[1] + 49;
    move[1] = ((8 - (avalibleMove[0] - '0')) + '0');
    move[2] = '-';
    move[3] = '>';
    move[4] = avalibleMove[4] + 49;
    move[5] = ((8 - (avalibleMove[3] - '0')) + '0');

  return move;
  free(move);
}
