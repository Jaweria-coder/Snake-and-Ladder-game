#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

const int BOARD_SIZE = 100;
const int ROWS = 10;
const int COLS = 10;

/* ---------- STRUCTURES ---------- */
struct Player {
    string name;
    int position;
    char symbol;
};

struct Game {
    Player players[2];
    int board[BOARD_SIZE + 1];
};

/* ---------- UTILITIES ---------- */
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int rollDice() {
    return rand() % 4 + 1;
}

/* ---------- INITIALIZE BOARD ---------- */
void initSnakesAndLadders(Game& game) {
    for (int i = 0; i <= BOARD_SIZE; i++)
        game.board[i] = i;

    // Ladders
    game.board[4] = 14;
    game.board[9] = 31;
    game.board[28] = 84;
    game.board[40] = 59;
    game.board[63] = 81;
    game.board[82] = 95;

    // Snakes
    game.board[17] = 7;
    game.board[54] = 34;
    game.board[64] = 60;
    game.board[87] = 24;
    game.board[93] = 73;
    game.board[99] = 78;
}

/* ---------- PRINT SINGLE CELL ---------- */
void printCell(Game& game, int num) {
    if (num == game.players[0].position &&
        num == game.players[1].position) {
        setColor(9);  cout << game.players[0].symbol;
        setColor(12); cout << game.players[1].symbol << " ";
    }
    else if (num == game.players[0].position) {
        setColor(9); cout << " " << game.players[0].symbol << " ";
    }
    else if (num == game.players[1].position) {
        setColor(12); cout << " " << game.players[1].symbol << " ";
    }
    else if (game.board[num] > num) {
        setColor(10); cout << "L" << setw(3) << num;
    }
    else if (game.board[num] < num) {
        setColor(12); cout << "S" << setw(3) << num;
    }
    else {
        setColor(14); cout << setw(4) << num;
    }
    setColor(7);
}

/* ---------- PRINT BOARD ---------- */
void printBoard(Game& game) {
    int num = 100;
    bool leftToRight = true;

    setColor(11);
    cout << "\n========== SNAKE & LADDER BOARD ==========\n\n";
    setColor(7);

    for (int i = 0; i < ROWS; i++) {
        if (leftToRight) {
            for (int j = 0; j < COLS; j++) {
                cout << " | ";
                printCell(game, num--);
            }
        }
        else {
            int temp = num - COLS + 1;
            for (int j = 0; j < COLS; j++) {
                cout << " | ";
                printCell(game, temp++);
            }
            num -= COLS;
        }
        cout << "|\n";
        leftToRight = !leftToRight;
    }
}

/* ---------- FILE SAVE ---------- */
void saveGameToFile(Game& game) {
    ofstream file("game_data.txt");
    for (int i = 0; i < 2; i++) {
        file << game.players[i].name << endl;
        file << game.players[i].position << endl;
    }
    file.close();
    cout << "Game data saved successfully.\n";
}

/* ---------- VIEW FILE DATA ---------- */
void viewSavedData() {
    ifstream file("game_data.txt");
    if (!file) {
        cout << "No saved data found.\n";
        return;
    }

    string name;
    int pos;
    cout << "\n----- SAVED DATA -----\n";
    while (getline(file, name)) {
        file >> pos;
        file.ignore();
        cout << "Player: " << name << " | Position: " << pos << endl;
    }
    file.close();
}

/* ---------- GAME PLAY ---------- */
void startGame(Game& game) {
    int currentPlayer = 0;

    while (true) {
        printBoard(game);

        Player& p = game.players[currentPlayer];
        cout << "\n" << p.name << "'s turn. Press ENTER to roll dice";
        cin.get();

        int dice = rollDice();
        cout << p.name << " rolled the dice: " << dice << endl;

        int newPos = p.position + dice;
        if (newPos <= BOARD_SIZE) {
            if (game.board[newPos] != newPos) {
                if (game.board[newPos] > newPos)
                    cout << "Ladder! Go to " << game.board[newPos] << endl;
                else
                    cout << "Snake! Go to " << game.board[newPos] << endl;
                newPos = game.board[newPos];
            }
            p.position = newPos;
        }

        cout << p.name << " is now at position: " << p.position << endl;

        if (p.position == BOARD_SIZE) {
            cout << "\n------CONGRATULATIONS------\n";
            cout << p.name << " WINS THE GAME ....\n";
            return;
        }

        if (dice != 6)
            currentPlayer = (currentPlayer + 1) % 2;
    }
}

/* ---------- MENU ---------- */
void menu(Game& game) {
    int choice;
    while (true) {
        cout << "\n=========== MENU ===========\n";
        cout << "1. Start Game\n";
        cout << "2. Save Game Data\n";
        cout << "3. View Saved Data\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            game.players[0].position = 0;
            game.players[1].position = 0;
            startGame(game);
            break;
        case 2:
            saveGameToFile(game);
            break;
        case 3:
            viewSavedData();
            break;
        case 4:
            exit(0);
        default:
            cout << "Invalid option!\n";
        }
    }
}

/* ---------- MAIN ---------- */
int main() {
    srand(time(0));
    setColor(11);
    cout << "========== WELCOME TO SNAKE & LADDER ==========\n";
    cout << "Two Player Game | First to reach 100 wins\n\n";
    setColor(7);
    Game game;

    cout << "Enter Player 1 Name: ";
    getline(cin, game.players[0].name);
    cout << "Enter Player 2 Name: ";
    getline(cin, game.players[1].name);

    game.players[0].symbol = toupper(game.players[0].name[0]);
    game.players[1].symbol = toupper(game.players[1].name[0]);

    initSnakesAndLadders(game);
    menu(game);

    return 0;
}
