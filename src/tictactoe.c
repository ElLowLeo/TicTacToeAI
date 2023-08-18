#include <stdio.h>

#include <stdbool.h>

#include <math.h>

bool isBoardFull(char board[3][3]);

void printBoard(char board[3][3]) {
  printf("\x1b[H\x1b[2J");
  printf("\x1B[33mBem vindo ao jogo da velha! Em C!\x1B[0m\n");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%c", board[i][j]);
      if (j < 2) {
        printf(" | ");
      }
    }
    printf("\n");
    if (i < 2) {
      printf("---------\n");
    }
  }
}

bool checkWinner(char board[3][3], char player) {
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
      return true;
    }
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
      return true;
    }
  }
  if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
    (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
    return true;
  }
  return false;
}

void getAvailableMoves(char board[3][3], int moves[9][2], int * numMoves) {
  * numMoves = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        moves[ * numMoves][0] = i;
        moves[ * numMoves][1] = j;
        ( * numMoves) ++;
      }
    }
  }
}

bool makeMove(char board[3][3], char player, int row, int col) {
  if (board[row][col] == ' ') {
    board[row][col] = player;
    return true;
  }
  return false;
}

int minimax(char board[3][3], int depth, bool maximizingPlayer, int alpha, int beta) {
  if (checkWinner(board, 'O')) {
    return 1;
  }
  if (checkWinner(board, 'X')) {
    return -1;
  }
  if (isBoardFull(board)) {
    return 0;
  }

  if (maximizingPlayer) {
    int maxEval = -INFINITY;
    int moves[9][2];
    int numMoves;
    getAvailableMoves(board, moves, & numMoves);

    for (int i = 0; i < numMoves; i++) {
      int row = moves[i][0];
      int col = moves[i][1];

      board[row][col] = 'O';
      int eval = minimax(board, depth + 1, false, alpha, beta);
      board[row][col] = ' ';

      if (eval > maxEval) {
        maxEval = eval;
      }
      alpha = (alpha > eval) ? alpha : eval;

      if (beta <= alpha) {
        break;
      }
    }
    return maxEval;
  } else {
    int minEval = INFINITY;
    int moves[9][2];
    int numMoves;
    getAvailableMoves(board, moves, & numMoves);

    for (int i = 0; i < numMoves; i++) {
      int row = moves[i][0];
      int col = moves[i][1];

      board[row][col] = 'X';
      int eval = minimax(board, depth + 1, true, alpha, beta);
      board[row][col] = ' ';

      if (eval < minEval) {
        minEval = eval;
      }
      beta = (beta < eval) ? beta : eval;

      if (beta <= alpha) {
        break;
      }
    }
    return minEval;
  }
}

void aiMakeMove(char board[3][3]) {
  int bestEval = -INFINITY;
  int bestMove[2];
  int moves[9][2];
  int numMoves;
  int alpha = -INFINITY;
  int beta = INFINITY;

  getAvailableMoves(board, moves, & numMoves);

  for (int i = 0; i < numMoves; i++) {
    int row = moves[i][0];
    int col = moves[i][1];

    board[row][col] = 'O';
    int eval = minimax(board, 0, false, alpha, beta);
    board[row][col] = ' ';

    if (eval > bestEval) {
      bestEval = eval;
      bestMove[0] = row;
      bestMove[1] = col;
    }
  }

  if (bestEval != -INFINITY) {
    makeMove(board, 'O', bestMove[0], bestMove[1]);
  }
}

bool isBoardFull(char board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        return false;
      }
    }
  }
  return true;
}

int main() {
  char board[3][3] = {
    {
      ' ',
      ' ',
      ' '
    },
    {
      ' ',
      ' ',
      ' '
    },
    {
      ' ',
      ' ',
      ' '
    }
  };

  while (1) {
    printBoard(board);

    while (1) {
      int move;
      printf("Jogador X - Qual o seu movimento? (1-9): \n");
      if (scanf("%d", & move) != 1) {
        while (getchar() != '\n');
        printf("\x1B[31mEntrada invalida! Por favor escreva um numero entre 1 e 9.\x1B[0m\n");
        continue;
      }

      if (move >= 1 && move <= 9) {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;

        if (makeMove(board, 'X', row, col)) {
          break;
        } else {
          printf("\x1B[31mMovimento invalido! Tente novamente.\x1B[0m\n");
        }
      } else {
        printf("\x1B[31mEntrada invalida! Por favor escreva um numero entre 1 e 9.\x1B[0m\n");
      }
    }

    if (checkWinner(board, 'X')) {
      printBoard(board);
      printf("\x1B[32mJogador X ganhou!\x1B[0m\n");
      break;
    }

    if (isBoardFull(board)) {
      printBoard(board);
      printf("\x1B[33mVelha!\x1B[0m\n");
      break;
    }

    aiMakeMove(board);

    if (checkWinner(board, 'O')) {
      printBoard(board);
      printf("\x1B[32mA máquina ganhou!\x1B[0m\n");
      break;
    }

    if (isBoardFull(board)) {
      printBoard(board);
      printf("\x1B[33mVelha!\x1B[0m\n");
      break;
    }
  }

  return 0;
}