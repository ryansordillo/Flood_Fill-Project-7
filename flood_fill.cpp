/**
 * @file flood_fill.cpp
 * @author Ryan Sordillo
 * @brief Accomplishes 3 tasks - 
 * 1st - Given an image, a row and column
of a pixel, and a color, change all pixels in that pixel’s region to that
color
2nd - Find the adjacent colors to a certain pixels region
3rd - Find minimum number of steps to fill the board completely with one color
 * @date 4/17/2024
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std; 
const int TOP_LEFT = 0;

/**
* Generates a vector of vector of chars based on the input number of cols and rows
* @param rows input number of rows in image
* @param cols input number of cols in image
* @param board reference to the board
*/
void start_board(int rows, int cols, vector<vector<char>>& board) {
    for (int r = 0; r < rows; r++) {
        vector<char> row; // empty by default
        char color;
        for (int c = 0; c < cols; c++) {
            cin >> color;
            row.push_back(color);
        }
        board.push_back(row);
    }
}

/**
* Recursively changes all pixels region to an input an color
* @param rows input number of rows in image
* @param cols input number of cols in image
* @param board reference to the board
* @param initial_color holds the intital color of the region
* @param replacement_color input color for the region to change to
* @return num_replaced number of pixels changed
*/
int flood_fill_helper(int row, int col, vector<vector<char>>& board, \
                                    char initial_color, char replacement_color) {
    // Base Case to make sure recursion doesn't go beyond board
    if (row < 0 || row >= board.size() || col < 0 || col >= board[0].size())
        return 0;
    if (board[row][col] != initial_color)
        return 0;
    board[row][col] = replacement_color;
    int num_replaced = 1;
    // Recursive Calls in all directions
    num_replaced += flood_fill_helper(row + 1, col, board, initial_color, replacement_color); // Down
    num_replaced += flood_fill_helper(row - 1, col, board, initial_color, replacement_color); // Up
    num_replaced += flood_fill_helper(row, col + 1, board, initial_color, replacement_color); // Right
    num_replaced += flood_fill_helper(row, col - 1, board, initial_color, replacement_color); // Left
    return num_replaced;
}

/**
* Calls the flood_fill_helper to recursively solve the problem
* @param rows input number of rows in image
* @param cols input number of cols in image
* @param board reference to the board
* @param initial_color holds the intital color of the region
* @param replacement_color input color for the region to change to
* @return flood_fill_helper
*/
int flood_fill(int start_row, int start_col, vector<vector<char>>& board, \
                                    char initial_color, char replacement_color) {
    return flood_fill_helper(start_row, start_col, board, initial_color, replacement_color);
}

/**
* Prints the board
* @param rows input number of rows in the board
* @param cols input number of cols in board
* @param board reference to the board
*/
void print_board(int rows, int cols, vector<vector<char>>& board) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}

/**
* Recursively finds the adjacent colors to the desired colors region
* @param rows input number of rows in image
* @param cols input number of cols in image
* @param board reference to the board
* @param initial_color color of region from input
* @param visited vector of vector of bools
* @return string of adjacent colors to the region
*/
string find_adjacent_colors_helper(int row, int col, vector<vector<char>>& board, \
                                    char initial_color, vector<vector<bool>>& visited) {
    // Check if the current cell is out of bounds or has been visited
    if (row < 0 || row >= board.size() || col < 0 || col >= board[0].size() || visited[row][col])
        return "";
    // Mark the current cell as visited
    visited[row][col] = true;
    char current_color = board[row][col];
    // If the current color is different from the initial color, return it
    if (current_color != initial_color) {
        string adjacent_colors;
        adjacent_colors += current_color;
        return adjacent_colors;
    }
    // Concatenate the results of recursive calls
    string adjacent_colors;
    adjacent_colors += find_adjacent_colors_helper(row + 1, col, board, initial_color, visited);
    adjacent_colors += find_adjacent_colors_helper(row - 1, col, board, initial_color, visited); 
    adjacent_colors += find_adjacent_colors_helper(row, col + 1, board, initial_color, visited); 
    adjacent_colors += find_adjacent_colors_helper(row, col - 1, board, initial_color, visited); 
    return adjacent_colors;
}

/**
* Initializes visited vector of vector of bools and calls adjacent_colors_helper
* @param rows input number of rows in image
* @param cols input number of cols in image
* @param board reference to the board
* @param initial_color color of region from input
* @return call to adjacent_colors_helper
*/
string find_adjacent_colors(int row, int col, vector<vector<char>>& board, char initial_color) {
    // Initialize the visited array
    vector<vector<bool>> visited;
    vector<bool> vrow;
    //Fill visited array with false
    for (int r = 0; r < board.size(); r++) {
        for (int c = 0; c < board[0].size(); c++) {
            vrow.push_back(false);
        }
    visited.push_back(vrow);
    }
    return find_adjacent_colors_helper(row, col, board, initial_color, visited);
}

/**
* Filters the adjacent color strings for only unique chars and sorts them alphabetically
* @param adjacent_colors string of adjacent colors 
* @return result string of adjacent colors to the region sorted alphabetically
*/
string filter_adjacent_colors(string adjacent_colors) {
    string result = "";
    // Iterate through each character in the string
    for (int i = 0; i < adjacent_colors.length(); i++) {
        char current_char = adjacent_colors[i];
        bool is_duplicate = false;
        // Check if the character is already in the result string
        for (int j = 0; j < result.length(); j++) {
            if (result[j] == current_char) {
                is_duplicate = true;
                break;
            }
        }
        // If the character is not a duplicate, add it to the result string
        if (!is_duplicate) 
            result += current_char;
    }
    //Sort alphabetically
    sort(result.begin(), result.end());
    return result;
}

/**
* Recursively finds the minimum number of steps to fill the board with one color
* @param row input starting row(always 0)
* @param cols input starting col(always 0)
* @param board reference to the board
* @return minimum number of moves to fill the board
*/
int game_helper(int row, int col, vector<vector<char>>& board) {
    vector<int> num_steps;
    // initialize board_copy
    vector<vector<vector<char>>> board_copy;
    string adjcol;
    adjcol = find_adjacent_colors(TOP_LEFT, TOP_LEFT, board, board[TOP_LEFT][TOP_LEFT]);
    //get adjacent colors and filter
    string filtered_adjcol = filter_adjacent_colors(adjcol);
    //base case if no adjacent colors
    if (adjcol.length() <= 0) {
        return 0;
    }
    else {
        //Recursively check each possible way to fill board and store in num_steps vector
        for (int i = 0; i < filtered_adjcol.length(); i++) {
            board_copy.push_back(board);
            flood_fill(TOP_LEFT, TOP_LEFT, board_copy[i], 
            board_copy[i][TOP_LEFT][TOP_LEFT], filtered_adjcol[i]);
            num_steps.push_back(game_helper(TOP_LEFT, TOP_LEFT, board_copy[i]));
        }
    }
    // CITE: https://www.geeksforgeeks.org/how-to-find-the-minimum-and-maximum-
    //element-of-a-vector-using-stl-in-c/
    // DESC: //find minimum element of vector
    return *min_element(num_steps.begin(), num_steps.end()) + 1;
}

/**
* calls recursive game function
* @param row input starting row(always 0)
* @param cols input starting col(always 0)
* @param board reference to the board
* @return call to game_helper
*/
int game(int row, int col, vector<vector<char>>& board) {
    return game_helper(row, col, board);
}

/**
* If the command is fill initializes all variables and asks for the correct input for the command
* Creates the board based on input rows and cols
*/
void fill() {
    //intialize all variables needed for flood_fill
    int start_row, start_col;
    char replacement_color;
    int rows, cols;
    vector<vector<char>> board;
    char initial_color;
    // Input for necesary variables
    cin >> start_row >> start_col >> replacement_color;
    cin >> rows >> cols;
    start_board(rows, cols, board);
    //set initial_color to the input start_row and start_col
    initial_color = board[start_row][start_col];
    int num_filled_pixels = flood_fill(start_row, start_col, board, initial_color, replacement_color);
    print_board(rows, cols, board);
    cout << "Number of pixels filled: " << num_filled_pixels << endl;
}

/**
* If the command is adjacent initializes all variables and asks for the correct input for the command
* Creates the board based on input rows and cols
*/
void adjacent() {
    //intialize all variables needed for adjacent task and takes in their input
    int start_row, start_col;
    int rows, cols;
    vector<vector<char>> board;
    char initial_color;
    cin >> start_row >> start_col;
    cin >> rows >> cols;
    start_board(rows, cols, board);
    initial_color = board[start_row][start_col];
    // Calls function to find adjacent colors
    string adjacent = find_adjacent_colors(start_row, start_col, board, initial_color);
    cout << filter_adjacent_colors(adjacent) << endl;
}

/**
* If the command is game initializes all variables and asks for the correct input for the command
* Creates the board based on input rows and cols
*/
void find_min() {
    //initialize variables for game command
    int rows, cols;
    vector<vector<char>> board;
    cin >> rows >> cols;
    start_board(rows, cols, board);
    //calls game function to find minimum steps to fill board
    int steps = game(TOP_LEFT, TOP_LEFT, board);
    cout << "Minimum number of steps to fill image with one color: " << steps << endl;
}

int main() {
    string arg;
    cin >> arg;
    if (arg == "fill") {
        fill();
    }
    else if (arg == "adjacent") {
        adjacent();
    }
    else {
        find_min();
    }
}

