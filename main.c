// Name: Jessica Runandy
// Date: April 18, 2020
// Description: This program is a Sudoku puzzle validator that uses multithreads 
// to determine if the solution to a Sudoku puzzle is valid.

#include <pthread.h>        // pthread*
#include <stdio.h>          // printf
#include <stdlib.h>         // calloc

// Struct to pass the row and column where a thread must begin validating
typedef struct {
  int row;
  int column;
} parameters;

// Keeps track of each thread and the validity of the Sudoku
int check_validity[27] = {0};

// 'grid' holds the Sudoku grid to be checked.  Notice that we define row-0
// and column-0 to be all-zeroes.  This just makes indexing easier to
// understand: for example, the first row of the Sudoku puzzle lies
// at grid[1], elements 1 thru 9.
int grid[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,6,2,4,5,3,9,1,8,7},
    {0,5,1,9,7,2,8,6,3,4},
    {0,8,3,7,6,1,4,2,9,5},
    {0,1,4,3,8,6,5,7,2,9},
    {0,9,5,8,2,4,7,3,6,1},
    {0,7,6,2,3,9,1,4,5,8},
    {0,3,7,1,9,5,6,8,4,2},
    {0,4,9,6,1,8,2,5,7,3},
    {0,2,8,5,4,7,3,9,1,6}
};

// Helper function of checkAllRows to check the validity of each
// row in the Sudoku grid, using a separate thread for each row.
// void* as the parameter and the return type.
void *checkAllRowsHelper(void *parameter) {
  parameters *parameter_value = (parameters*) parameter;
  int row = parameter_value->row;

  // row_valid to keep track of the numbers that have appeared in the row
  // The ith index of the row_valid array corresponds to the 9 values
  // in the row of the Sudoku (starting from index 1)
  int row_valid[10] = {0};

  // Loops through the 9 rows of the Sudoku
  for (int i = 1; i < 10; i++) {
    int box_value = grid[row][i];

    // If 0, haven't checked the value yet, so set to 1
    if (row_valid[box_value] == 0) {
      row_valid[box_value] = 1;
    } else { // Not 0, there is a duplicate number
      // Exits because invalid so don't create more threads
      //pthread_exit(NULL);
      pthread_exit(0);
    }
  }

  // Valid if reaches this part, updates the validity of sudoku row to 1
  // (updates index 0 to 8 in check_validity)
  check_validity[row - 1] = 1;
  //pthread_exit(NULL);
  pthread_exit(0);
}

// Check all 9 rows of the Sudoku grid,
// using a separate thread for each row.
void checkAllRows() {
  pthread_t rowTids[10]; // TIDs for row threads
  for (int i = 1; i < 10; i++) {
    parameters *data = (parameters *) malloc(sizeof(parameters));
    data->row = i; // Update the row to be checked
    data->column = 1;
    pthread_create(&rowTids[i], NULL, checkAllRowsHelper, (void *)data);
  }

  // Joins the threads after creating them
  for (int i = 1; i < 10; i++) {
    pthread_join(rowTids[i], NULL);
  }
}

// Helper function of checkAllCols to check the validity of each
// column in the Sudoku grid, using a separate thread for each column.
// void* as the parameter and the return type.
void *checkAllColsHelper(void *parameter) {
  parameters *parameter_value = (parameters*) parameter;
  int column = parameter_value->column;

  // column_valid to keep track of the numbers that have appeared in the column
  // The ith index of the column_valid array corresponds to the 9 values
  // in the column of the Sudoku (starting from index 1)
  int column_valid[10] = {0};

  // Loops through the 9 columns of the Sudoku
  for (int i = 1; i < 10; i++) {
    int box_value = grid[i][column];

    // If 0, haven't checked the value yet, so set to 1
    if (column_valid[box_value] == 0) {
      column_valid[box_value] = 1;
    } else { // Not 0, there is a duplicate number
      // Exits because invalid so don't create more threads
      //pthread_exit(NULL);
      pthread_exit(0);
    }
  }

  // Valid if reaches this part, updates the validity of sudoku column to 1
  // (updates index 9 to 17 in check_validity)
  check_validity[column + 8] = 1;
  //pthread_exit(NULL);
  pthread_exit(0);
}

// Check all 9 columns of the Sudoku grid,
// using a separate thread for each column.
void checkAllCols() {
  pthread_t colTids[10]; // TIDs for col threads
  for (int i = 1 ; i < 10; i++) {
    parameters *data = (parameters *) malloc(sizeof(parameters));
    data->row = 1;
    data->column = i; // Update the column to be checked
    pthread_create(&colTids[i], NULL, checkAllColsHelper, (void *)data);
  }

  // Joins the threads after creating them
  for (int i = 1; i < 10; i++) {
    pthread_join(colTids[i], NULL);
  }
}

// Helper function of checkAllBoxes to check the validity of each
// box in the Sudoku grid, using a separate thread for each box.
// void* as the parameter and the return type.
void* checkAllBoxesHelper(void* parameter) {
  parameters *parameter_value = (parameters*) parameter;
  int row = parameter_value->row;
  int column = parameter_value->column;

  // box_valid to keep track of the numbers that have appeared in the box
  // The ith index of the box_valid array corresponds to the 9 values
  // in each of the 3 x 3 subgrids of the Sudoku
  // (starting from index 1)
  int box_valid[10] = {0};

  // Loops through the 9 3 x 3 subgrids of the Sudoku
  for (int i = row; i < row + 3; i++) {
    for (int j = column; j < column + 3; j++) {
      int box_value = grid[i][j];

      // If 0, haven't checked the value yet, so set to 1
      if (box_valid[box_value] == 0) {
        box_valid[box_value] = 1;
      } else { // Not 0, there is a duplicate number
        // Exits because invalid so don't create more threads
        //pthread_exit(NULL);
        pthread_exit(0);
      }
    }
  }

  // Valid if reaches this part, updates the validity of sudoku box to 1
  // (updates index 18 to 26 in check_validity)
  check_validity[row + column / 3 + 17] = 1;
  //pthread_exit(NULL);
  pthread_exit(0);
}

// Check all 9 boxes of the Sudoku grid,
// using a separate thread for each box.
void checkAllBoxes() {
  pthread_t boxTids[10]; // TIDs for row threads
  int boxTidsCounter = 1;

  // Increments by 3 to account for the 9 3 x 3 boxes
  for (int i = 1 ; i < 10; i+=3) {
    for (int j = 1; j < 10; j+=3) {
      parameters *data = (parameters *) malloc(sizeof(parameters));
      data->row = i; // Update the row to be checked
      data->column = j; // Update the column to be checked
      pthread_create(&boxTids[boxTidsCounter++], NULL, checkAllBoxesHelper, (void *)data);
    }
  }

  // Joins the threads after creating them
  for (int i = 1; i < 10; i++) {
    pthread_join(boxTids[i], NULL);
  }
}

// Check rows, columns, and boxes
void checkAll() {
  checkAllRows();
  checkAllCols();
  checkAllBoxes();
}

int main() {
  checkAll();

  // Loop through check_validity to check if the Sudoku is valid.
  // If check_validity contains all "1"s, the Sudoku is valid.
  for (int i = 0; i < 27; i++) {
    if (check_validity[i] == 0) { // Invalid Sudoku
      printf("puzzle is NOT valid\n");
      return 0; // Exits
    }
  }
  printf("puzzle is valid\n");
  return 0;
}
