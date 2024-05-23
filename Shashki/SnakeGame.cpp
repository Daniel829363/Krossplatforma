#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string>
using namespace std;

enum Move {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class SnakeGame {
public:
    const int rows = 15;
    const int columns = 30;
    const char mapSymbol = '#';
    const char snakeHead = 'O';
    const char snakeTail = '*';
    const int maxSnakeLength = (columns - 3) * (rows - 2);
    int* snakeX;
    int* snakeY;
    int snakeLength = 1;
    int foodX;
    int foodY;
    const char foodSymbol = '@';
    bool isGameRunning = true;
    Move snakeDirection = RIGHT;

    SnakeGame() {
        snakeX = new int[maxSnakeLength]();
        snakeY = new int[maxSnakeLength]();
    }

    ~SnakeGame() {
        delete[] snakeX;
        delete[] snakeY;
    }

    void PrintMap() {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                if (y == 0 || x == 0 || y == rows - 1 || x == columns - 1) {
                    cout << mapSymbol;
                }
                else {
                    cout << ' ';
                }
            }
            cout << endl;
        }
    }

    void SetCursorPosition(int x, int y) {
        COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(output, &structCursorInfo);
        SetConsoleCursorPosition(output, position);
        structCursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(output, &structCursorInfo);
    }

    /*void SetCursorPosition(int x, int y) {
        COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(output, position);
    }*/
    void GetKeyToMove() {
        if (GetAsyncKeyState('W') & 0x8000) {
            if (snakeDirection != DOWN) {
                snakeDirection = UP;
            }
        }

        if (GetAsyncKeyState('S') & 0x8000) {
            if (snakeDirection != UP) {
                snakeDirection = DOWN;
            }
        }

        if (GetAsyncKeyState('A') & 0x8000) {
            if (snakeDirection != RIGHT) {
                snakeDirection = LEFT;
            }
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            if (snakeDirection != LEFT) {
                snakeDirection = RIGHT;
            }
        }
        if (GetAsyncKeyState('0') & 0x8000) {
            isGameRunning = false;
        }
        ChangeHeadPosition();
    }


    void ChangeHeadPosition()
    { // Меняем координаты головы змейки в зависимости от направления движения
        switch (snakeDirection) {
        case UP:
            snakeY[0]--;
            break;
        case DOWN:
            snakeY[0]++;
            break;
        case LEFT:
            snakeX[0]--;
            break;
        case RIGHT:
            snakeX[0]++;
            break;
        }
    }

    void PrintFood() { // Отрисовка еды
        SetCursorPosition(foodX, foodY);
        cout << foodSymbol;
        if (snakeX[0] == foodX && snakeY[0] == foodY) { // Если голова змеи съела еду, то увеличиваем длину змеи и рисуем новую еду
            snakeLength++;
            PrintNewFood();
        }
    }

    void PrintNewFood() {
        bool foodSpawn = true; // Разрешение спавна еды
        foodX = 1 + (rand() % (columns - 3));
        foodY = 1 + (rand() % (rows - 2));
        CheckSpawnStatus(foodSpawn);
        while (foodSpawn == false) { // Пока запрещен спавн еды (совпадает со змеей), переопределяем координаты спавна
            foodSpawn = true;
            foodX = 1 + (rand() % (columns - 3));
            foodY = 1 + (rand() % (rows - 2));
            CheckSpawnStatus(foodSpawn);
        }
        SetCursorPosition(foodX, foodY);
        cout << foodSymbol;
    }

    void CheckSpawnStatus(bool& foodSpawn) { // Если координаты появления еды совпадают со змеей, то запрещаем спавн
        for (int i = 0; i <= snakeLength; i++) {
            if (snakeX[i] == foodX && snakeY[i] == foodY) {
                foodSpawn = false;
                break;
            }
        }
    }

    void CheckGameStatus() { // Проверяет пересекает ли голова змеи границы карты или свой хвост, если да, то игра завершается   
        for (int i = 2; i <= snakeLength; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
                isGameRunning = false;
                return;
            }
        }
        if (snakeX[0] == 0 || snakeX[0] == columns - 1 || snakeY[0] == 0 || snakeY[0] == rows - 1) {
            if (snakeX[0] == 0)snakeX[0] = columns - 2;
            else if (snakeX[0] == columns - 1)snakeX[0] = 0;
            else if (snakeY[0] == rows - 1)snakeY[0] = 0;
            else if (snakeY[0] == 0)snakeY[0] = rows - 1;
        }
    }

    void GetSnakeCoordinates() { // Заполнение массива координатами каждой части змеи
        for (int i = snakeLength; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
    }

    void PrintSnake() { // Печатает змейку на экран
        GetSnakeCoordinates();

        for (int i = 0; i <= snakeLength; i++) {
            SetCursorPosition(snakeX[i], snakeY[i]);
            if (i <= 1)
                cout << snakeHead;
            else
                cout << snakeTail;
        }
    }

    void GameOver() { // Выводит информацию о результатах игры
        SetCursorPosition(0, rows+2);
        cout << "GameOver! Your score is " << snakeLength - 1 << endl;
        Sleep(1500);
    }

    void SetInitialCoordinates() { // Устанавливает начальные координаты для змейки и еды
        snakeX[0] = columns / 2;
        snakeY[0] = rows / 2;
        foodX = 1 + (rand() % (columns - 3));
        foodY = 1 + (rand() % (rows - 2));
    }

    void Run() {
        //srand(time(0));
        srand(static_cast<unsigned>(time(0)));
        SetInitialCoordinates();

        while (isGameRunning) {
            SetCursorPosition(0, 0);
            PrintMap();
            PrintFood();
            GetKeyToMove();
            PrintSnake();
            CheckGameStatus();
            Sleep(150);
        }
        GameOver();
        system("pause");
    }
};