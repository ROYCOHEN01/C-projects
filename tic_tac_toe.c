#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define SIZE 3

void Board(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("---|---|---\n");
    }
}

int checkWin(char board[SIZE][SIZE], int* winningLine, int* isRow) {
    // Check rows
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            *winningLine = i;
            *isRow = 1;
            return 1;
        }
    }
    // Check columns
    for (int i = 0; i < SIZE; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            *winningLine = i;
            *isRow = 0;
            return 1;
        }
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        *winningLine = 0;
        *isRow = 2;
        return 1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        *winningLine = 1;
        *isRow = 2;
        return 1;
    }
    return 0;
}

void fillWinningLine(char board[SIZE][SIZE], int winningLine, int isRow) {
    if (isRow == 1) {
        // Fill row
        for (int i = 0; i < SIZE; i++) {
            board[winningLine][i] = 'V';
        }
    }
    else if (isRow == 0) {
        // Fill column
        for (int i = 0; i < SIZE; i++) {
            board[i][winningLine] = 'V';
        }
    }
    else if (isRow == 2) {
        // Fill diagonal
        if (winningLine == 0) {
            for (int i = 0; i < SIZE; i++) {
                board[i][i] = 'V';
            }
        }
        else {
            for (int i = 0; i < SIZE; i++) {
                board[i][SIZE - 1 - i] = ' ';
            }
        }
    }
}

int isBoardFull(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

int main() {
    char board[SIZE][SIZE] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
    char player = 'X';
    int row, col;
    int winningLine, isRow;

    while (1) {
        Board(board);
        printf("Player %c, enter your move in the pattern: row,column \n", player);
        scanf("%d,%d", &row, &col);

        if (row < 1 || row > SIZE || col < 1 || col > SIZE || board[row - 1][col - 1] != ' ') {
            printf("Invalid move. Try again.\n");
            continue;
        }

        board[row - 1][col - 1] = player;

        if (checkWin(board, &winningLine, &isRow)) {
            fillWinningLine(board, winningLine, isRow);
            Board(board);
            printf("Player %c wins!\n", player);
            break;
        }

        if (isBoardFull(board)) {
            Board(board);
            printf("The game is a draw.\n");
            break;
        }

        player = (player == 'X') ? 'O' : 'X';
    }

    return 0;
}
