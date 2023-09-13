#include <iostream>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <string>
#include <map>

using namespace std;

// Enum for specifying text colors
enum ConsoleColor
{
    Blue,
    Green,
    Aqua,
    Red,
    Purple,
    Yellow,
    White
};

// A map that associates color names with ConsoleColor values
map<string, ConsoleColor> colorMap =
    {
        {"Blue", Blue},
        {"Green", Green},
        {"Aqua", Aqua},
        {"Red", Red},
        {"Purple", Purple},
        {"Yellow", Yellow},
        {"White", White}};

// Enum for specifying directions
enum Directions
{
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
    DOWN = 4
};

// Function declarations
void Message();                                           // Function to display a message
void setColor(const string &foregroundColor);             // Function to set text color
void setPosition(int x, int y);                           // Function to set console cursor position
void drawBoard(const string &foregroundColor);            // Function to draw the game board
void drawFood();                                          // Function to draw the food on the game board
void logic();                                             // Function to handle game logic
void gameOver();                                          // Function to handle game over scenario
void startLevel(string colorName);                        // Function to start a game level with a specific color
void generateWall();                                      // Function to generate game walls
void restartGame();                                       // Function to restart the game
void box(const string &foregroundColor);                  // Function to draw a box
void displayScore(int sc, const string &foregroundColor); // Function to display the player's score

int directions, snakeSize = 5, i, foodXCoordinate, foodYCoordinate;                                 // Variables related to the snake and food
int speed, restart = 1, isEverythingOk, snakeXposition[100], snakeYposition[100], score, level = 1; // Variables related to the game state
int displayNewLevelBox = 0;                                                                         // Variable to control displaying a new level message
string colorName;                                                                                   // Name of the color theme
char color, mode, input_1, input_2;                                                                 // Characters used for color and mode selection
const int wallSize = 112, secondLevelScore = 5, winScore = 10;                                       // Constants for game walls and second level score requirement
int wallXposition[wallSize], wallYPostion[wallSize];                                                // Variables for game walls

int main()
{
    // Display a purple box with a message, wait for Enter key press
    box("Purple");
    do
    {
        Message();
        setPosition(27, 18);
        setColor("Red");
        cout << setw(15) << "Press \'Enter\' to continue...";

    } while (_getch() != 13); // Hold the screen until user press enter

    system("cls"); // Clear the screen

    // Display a green box to choose color c
    box("Green");

    do
    {
        setPosition(23, 3);
        setColor("Green");
        cout << "-- Pick a game color scheme now --";

        do
        {
            setPosition(26, 5);
            setColor("Aqua");
            cout << "Press \'A\' to choose aqua";

            setPosition(26, 6);
            setColor("Yellow");
            cout << "Press \'Y\' to choose yellow";

            setPosition(26, 7);
            setColor("White");
            cout << "Press \'W\' to choose white";

            setPosition(26, 8);
            setColor("Purple");
            cout << "Press \'P\' to choose purple";

            color = _getch();
            color = tolower(color);

        } while (color != 'a' && color != 'y' && color != 'w' && color != 'p');

        // Display a green box to choose difficulty level

        setPosition(23, 11);
        setColor("Green");
        cout << "---- Select a difficulty level ----";

        do
        {
            setPosition(26, 13);
            setColor("Green");
            cout << "Press \'E\' to pick easy mode";

            setPosition(26, 14);
            setColor("Yellow");
            cout << "Press \'M\' to pick medium mode";

            setPosition(26, 15);
            setColor("Red");
            cout << "Press \'H\' to pick hard mode";

            mode = _getch();
            mode = tolower(mode);

        } while (mode != 'e' && mode != 'm' && mode != 'h');

        setPosition(24, 18);
        setColor("Aqua");
        cout << setw(15) << "Press \'Enter\' to start the game!";

    } while (_getch() != 13); // Hold the screen until user press enter

    system("cls"); // Clear the screen

    // Set up the game board based on the chosen color
    switch (color)
    {
    case 'a':
        drawBoard("Aqua");
        colorName = "Aqua";
        break;

    case 'y':
        drawBoard("Yellow");
        colorName = "Yellow";
        break;

    case 'w':
        drawBoard("White");
        colorName = "White";
        break;

    case 'p':
        drawBoard("Purple");
        colorName = "Purple";
        break;
    }

    srand(time(NULL));

    while (true)
    {
        // Handle game restart
        if (restart)
        {
            restartGame();
        }

        // Display a new level box
        if (displayNewLevelBox)
        {
            startLevel(colorName);
        }

        while (!_kbhit() && !restart)
        {
            logic();

            // Display a new level box (if needed)
            if (displayNewLevelBox)
            {
                startLevel(colorName);
            }

            Sleep(speed); // Pause for a moment based on the game speed
        }

        // Handle game actions based on user input
        input_1 = _getch();

        switch (tolower(input_1))
        {
        case 'x':
            // Quit the game
            do
            {
                box("Red");

                setPosition(29, 9);
                setColor("Green");

                cout << "Thanks for playing !";

                setPosition(29, 12);
                setColor("Yellow");

                cout << "Press \'Enter\' to exit";

            } while (_getch() != 13); // Hold the screen until user press enter

            system("cls"); // Clear the screen

            return 0; // Exit from the program

        case ' ':
            // Pause/unpause the game
            while (true)
            {
                input_2 = _getch();

                if (input_2 == 'x')
                {
                    // Quit the game
                    do
                    {
                        box("Red");

                        setPosition(29, 9);
                        setColor("Green");

                        cout << "Thanks for playing !";

                        setPosition(29, 12);
                        setColor("Yellow");

                        cout << "Press \'Enter\' to exit";
                    } while (_getch() != 13); // Hold the screen until user press enter

                    system("cls"); // Clear the screen

                    return 0; // Exit from the program
                }
                if (input_2 == ' ')
                {
                    break;
                }
            }

            break;

        case 'w':
            // Change snake direction to UP
            if (directions != DOWN)
            {
                directions = UP;
            }
            break;

        case 's':
            // Change snake direction to DOWN
            if (directions != UP)
            {
                directions = DOWN;
            }
            break;

        case 'a':
            // Change snake direction to LEFT
            if (directions != RIGHT)
            {
                directions = LEFT;
            }
            break;

        case 'd':
            // Change snake direction to RIGHT
            if (directions != LEFT)
            {
                directions = RIGHT;
            }
            break;
        }
    }
    return 0; // Exit from the program
}

void restartGame()
{
    // Reset the snake size to 5 segments
    snakeSize = 5;

    // Clear the previous snake trail from the screen
    for (int k = 1; k < 75; k += 2)
    {
        for (int j = 0; j < 17; j++)
        {
            setPosition(k + 3, j + 2);
            cout << ' ';
        }
    }

    // Set the game speed based on the chosen difficulty level
    switch (mode)
    {
    case 'e':
        speed = 300; // Easy
        break;

    case 'm':
        speed = 200; // Medium
        break;

    case 'h':
        speed = 100; // Hard
        break;
    }

    // Clean up the snake's previous position
    for (int i = 0; i < wallSize; i++)
    {
        setPosition(wallXposition[i], wallYPostion[i]);
        cout << ' ';
    }

    // Reset the player's score to 0 and display it
    score = 0;

    displayScore(score, "White");

    // Set the initial direction of the snake to right
    directions = RIGHT;

    // Initialize the snake's starting position
    snakeXposition[0] = 20;

    // Initialize the snake's positions for all segments
    for (i = 0; i < snakeSize; i++)
    {
        snakeXposition[i] = snakeXposition[0] - i * 2;
        snakeYposition[i] = 10;
    }

    // Display the snake on the screen
    for (i = 0; i < snakeSize; i++)
    {
        setPosition(snakeXposition[i], snakeYposition[i]);
        cout << char(254);
    }

    // Place a new food item on the game board
    drawFood();

    // Reset the game level to 1 and clear the flags for level change and restart
    level = 1;
    restart = 0;
    displayNewLevelBox = 0;
}

void generateWall()
{
    // Create two rows of walls for the level 2, one at the top and one at the bottom of the game board

    // Initialize the top row of walls
    for (int j = 0; j < 56; j++)
    {
        // Set the X positions of the top row walls
        wallXposition[j] = j + 10;

        // Set the Y positions of the top row walls (constant value of 5 for the top row)
        wallYPostion[j] = 5;
    }

    // Initialize the bottom row of walls
    for (int j = 0; j < 56; j++)
    {
        // Set the X positions of the bottom row walls
        wallXposition[j + 56] = j + 10;

        // Set the Y positions of the bottom row walls (constant value of 15 for the bottom row)
        wallYPostion[j + 56] = 15;
    }
}

void startLevel(string colorName)
{
    // Clear the console screen
    system("cls");

    // Draw a box for the game area
    box("Green");

    Message();

    drawBoard(colorName);

    for (int k = 1; k < 75; k += 2)
    {
        for (int j = 0; j < 17; j++)
        {
            setPosition(k + 3, j + 2);
            cout << ' ';
        }
    }

    // Initialize the snake size and display the player's score
    snakeSize = 5;

    displayScore(score, "White");

    // Set the initial direction of the snake to RIGHT
    directions = RIGHT;

    // Initialize the snake's position
    snakeXposition[0] = 20;

    // Initialize the snake's body positions
    for (i = 0; i < snakeSize; i++)
    {
        snakeXposition[i] = snakeXposition[0] - i * 2;
        snakeYposition[i] = 10;
    }

    for (i = 0; i < snakeSize; i++)
    {
        setPosition(snakeXposition[i], snakeYposition[i]);
        cout << char(254);
    }

    // Draw the initial food on the board
    drawFood();

    displayNewLevelBox = 0;

    // Check if the current level is 2 and generate walls if necessary
    if (level == 2)
    {
        char verticalBar = 254u, dash = 254u;

        // Generate and display walls on the board
        generateWall();
        setColor(colorName);

        for (int k = 0; k < 46; k++)
        {
            setPosition(wallXposition[k], wallYPostion[k]);
            cout << dash;
        }

        for (int k = 46; k < wallSize; k++)
        {
            setPosition(wallXposition[k], wallYPostion[k]);
            cout << verticalBar;
        }
        setColor("White");
    }
}

void logic()
{
    // Check if the snake's head is at the same position as the food
    if (snakeXposition[0] == foodXCoordinate && snakeYposition[0] == foodYCoordinate)
    {
        // Redraw the food at a new position
        drawFood();

        snakeSize++; // Increase the snake's size
        score++;     // Increase the player's score

        if (speed > 30)
        {
            speed -= 3;
        }

        // Check if the player reached the score required to advance to level 2
        if (score == secondLevelScore)
        {
            level = 2;
            displayNewLevelBox = 1;
        }

        displayScore(score, "White");

        if (level == 2)
        {
            if (score == winScore)
            {
                do
                {
                    box("Red");

                    setPosition(29, 9);
                    setColor("Blue");

                    cout << "***Congradulations***";

                    setPosition(36, 12);
                    setColor("Yellow");

                    cout << "You Won!!";
                } while (_getch() != 13); // Hold the screen until user press enter

                system("cls");

                restart = 1;
                // Reset the game level to 1
                level = 1;
                displayNewLevelBox = 0;

                // _getch();
            }
        }
    }

    setPosition(snakeXposition[snakeSize - 1], snakeYposition[snakeSize - 1]);

    for (i = snakeSize - 1; i > 0; i--)
    {
        snakeXposition[i] = snakeXposition[i - 1];
        snakeYposition[i] = snakeYposition[i - 1];
    }

    // Move the snake's head based on the current direction
    switch (directions)
    {
    case RIGHT:
        snakeXposition[i] += 2;
        break;

    case LEFT:
        snakeXposition[i] -= 2;
        break;

    case UP:
        snakeYposition[i]--;
        break;

    case DOWN:
        snakeYposition[i]++;
        break;
    }

    // Check for collision with the snake's own body
    for (i = 1; i < snakeSize; i++)
    {
        if (snakeXposition[i] == snakeXposition[0] && snakeYposition[i] == snakeYposition[0])
        {
            gameOver(); // End the game if there's a collision with the snake's body
            break;
        }
    }

    // If the game is in level 2, check for collision with walls
    if (level == 2)
    {
        for (i = 1; i < wallSize; i++)
        {
            if (wallXposition[i] == snakeXposition[0] && wallYPostion[i] == snakeYposition[0])
            {
                gameOver(); // End the game if there's a collision with a wall
                break;
            }
        }
    }

    // Check if the snake has hit the boundaries of the game area
    if (snakeXposition[0] > 76 || snakeXposition[0] < 4 || snakeYposition[0] < 2 || snakeYposition[0] > 18)
    {
        gameOver(); // End the game if the snake hits the boundaries
    }

    else
    {
        cout << ' '; // Clear the previous position of the snake's head
        setPosition(snakeXposition[0], snakeYposition[0]);
        cout << char(2); // Display the snake's head
        setPosition(snakeXposition[1], snakeYposition[1]);
        cout << char(254); // Display the snake's body
    }
}

void gameOver()
{
    // Set text color to red to indicate game over
    setColor("Red");
    cout << char(254);
    // Position the cursor at the second segment of the snake's body
    setPosition(snakeXposition[1], snakeYposition[1]);
    cout << char(153);

    for (i = 2; i < snakeSize; i++)
    {
        setPosition(snakeXposition[i], snakeYposition[i]);
        cout << char(254);
    }

    setColor("White");

    restart = 1;
    // Reset the game level to 1
    level = 1;
    displayNewLevelBox = 0;

    _getch();
}

void drawFood()
{
    for (isEverythingOk = 1; true;)
    {
        // Generate random X and Y coordinates for the food within the game board
        // Ensure the X coordinate is even
        do
        {
            foodXCoordinate = rand() % 75 + 3;
        } while (foodXCoordinate % 2 != 0);

        // Ensure the X coordinate is even
        foodYCoordinate = rand() % 17 + 2;

        // Check if the generated food coordinates collide with the snake's body
        for (i = 0; i < snakeSize; i++)
        {
            // If the food coordinates overlap with the snake's body, mark as invalid
            if (foodXCoordinate == snakeXposition[i] && foodYCoordinate == snakeYposition[i])
            {
                isEverythingOk = 0;
                break;
            }
        }

        // If the game is in level 2, check if the food coordinates collide with walls
        if (level == 2)
        {
            for (i = 0; i < wallSize; i++)
            {
                // If the food coordinates overlap with a wall, mark as invalid
                if (foodXCoordinate == wallXposition[i] && foodYCoordinate == wallYPostion[i])
                {
                    isEverythingOk = 0;
                    break;
                }
            }
        }

        // If the food coordinates are valid, exit the loop
        if (isEverythingOk)
        {
            break;
        }
    }

    // Set the cursor position to display the food at the selected coordinates
    setPosition(foodXCoordinate, foodYCoordinate);
    // Set the color for the food (e.g., green)
    setColor("Green");
    // Display the food using a character
    cout << char(220);
    setColor("White");
}

// Function to set the text color in the console
void setColor(const string &foregroundColor)
{
    map<string, ConsoleColor>::iterator it = colorMap.find(foregroundColor);

    if (it != colorMap.end())
    {
        ConsoleColor foregroundColorIndex = it->second;

        HANDLE hanldeConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

        int attribute = static_cast<int>(foregroundColorIndex) + 1; // Adding 1 to match the enum values

        SetConsoleTextAttribute(hanldeConsoleOutput, attribute);
    }
}

// Function to set the cursor position in the console window
void setPosition(int x, int y)
{
    // Create a COORD structure to hold the X and Y coordinates
    COORD coordinate;
    // Set the X and Y coordinates based on the function's parameters
    coordinate.X = x;
    coordinate.Y = y;
    // Use SetConsoleCursorPosition to move the cursor to the specified position
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

// Function to draws the game board
void drawBoard(const string &foregroundColor)
{
    char squre = 178;
    int i; // Variable for all for-loops (make cde more effiecient and effective)
    HANDLE handleConsolOutput;
    /*
        A data type that represents a handle to a resource, such as a file, a window, or a memory block.
        In this case, the `HANDLE` variable is used to store the handle to the console output
    */
    CONSOLE_CURSOR_INFO cursor;
    /*
        A structure that contains information about the cursor, such as its position and visibility.
        In this case, the `CONSOLE_CURSOR_INFO` variable is used to store the cursor info for the console output
    */

    handleConsolOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    cursor.dwSize = 1;
    cursor.bVisible = false;

    SetConsoleCursorInfo(handleConsolOutput, &cursor);

    // Set the console title and clear the screen
    system("mode con:cols=80 lines=25");
    system("title Snake Game");
    system("cls");
    setColor(foregroundColor);

    cout << "\n  " << squre;

    // Print the top row of the board.
    for (i = 0; i < 75; i++)
    {
        cout << squre;
    }

    cout << squre << "  ";

    // Print the vertical lines of the board
    for (i = 0; i < 17; i++)
    {
        setPosition(2, i + 2);
        cout << squre;

        setPosition(78, i + 2);
        cout << squre << ' ';
    }

    cout << "  " << squre;

    for (i = 0; i < 75; i++)
    {
        cout << squre;
    }

    cout << squre;

    // Print the score
    setPosition(55, 22);
    setColor(foregroundColor);
    cout << "  Score :         ";

    // Print the exit, pause, and respawn instructions
    setPosition(4, 21);
    cout << "Press \'X\' to exit";
    setPosition(4, 22);
    cout << "Press \'space\' to pause the game";
    setPosition(4, 23);
    cout << "Press \'Enter\' to respawn";
}

// Function to function displays the score
void displayScore(int score, const string &foregroundColor)
{
    // Set the text color & print the score
    setPosition(65, 22);
    setColor(foregroundColor);
    cout << setw(3) << score * 10;
}

// Function to displays the welcome message
void Message()
{
    // Continuously display the message until the user presses Enter
    do
    {
        setPosition(11, 2);
        setColor("Yellow");
        cout << "==+==+==+==+==+==";

        setPosition(29, 2);
        setColor("Green");
        cout << "SNAKE GAME - Level " << level;

        setPosition(51, 2);
        setColor("Yellow");
        cout << "==+==+==+==+==+==";

        // Print the control instructions
        string instructionList[] = {
            "W - MOVE SNAKE UPWORDS",
            "S - MOVE SNAKE DOWNWARDS",
            "A - MOVE SNAKE LEFTWARDS",
            "D - MOVE SNAKE RIGHTWARD",
            "space - PAUSE THE GAME",
            "X - QUIT THE GAME",
        };

        // Print the level instructions
        setPosition(5, 4);
        setColor("Yellow");
        cout << "Controls,";
        int i = 0;
        for (; i < 6; i++)
        {
            setPosition(10, (5 + i));
            setColor("White");
            cout << (i + 1) << ") " << instructionList[i];
        }

        setPosition(5, 13);
        setColor("Yellow");
        cout << "Instructions,";

        setPosition(10, 14);
        setColor("White");

        // Display instructions that related to level two
        if (level == 2)
        {
            string levelInstruction[] = {
                "COLLECT ALL FOOD ITEMS", "AVOID SELF - COLLISIONS", "AVOID CENTER WALL"};
            for (int i = 0; i < 3; i++)
            {
                setPosition(10, (14 + i));
                cout << (i + 1) << ") " << levelInstruction[i];
            }
        }
        else
        {
            string levelInstruction[] = {
                "COLLECT ALL FOOD ITEMS", "AVOID SELF - COLLISIONS"};
            for (int i = 0; i < 2; i++)
            {
                setPosition(10, (14 + i));
                cout << (i + 1) << ") " << levelInstruction[i];
            }
        }

        setPosition(27, 18);
        setColor("Red");
        cout << setw(15) << "Press \'Enter\' to continue...";
    } while (_getch() != 13);
}

// Function to draw a box on the console
void box(const string &foregroundColor)
{
    char verticalBar = 179, topRightConer = 191, bottomLeftConer = 192, dash = 196, bottomRightConer = 217, squre = 218;

    // Set the console title and clear the screen
    system("mode con:cols=80 lines=25");
    system("title Snake Game");
    system("cls");

    // Set a color
    setColor(foregroundColor);

    // Draw the box using appropriate characters
    cout << "\n  " << squre;

    int x;
    for (x = 0; x < 75; x++)
    {
        cout << dash;
    }

    cout << topRightConer << "  ";

    for (x = 0; x < 17; x++)
    {
        setPosition(2, x + 2);
        cout << verticalBar;

        setPosition(78, x + 2);
        cout << verticalBar << ' ';
    }

    cout << "  " << bottomLeftConer;

    for (x = 0; x < 75; x++)
    {
        cout << dash;
    }

    cout << bottomRightConer << "   ";
    
}
