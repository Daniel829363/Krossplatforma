#include <iostream>
#include <string>
#include "TextEditor.cpp"
#include "SnakeGame.cpp"
#include "Header.h"

using namespace std;
void printMenu()
{
    cout << "\nВыберите один из вариантов\n";
    cout << "============================\n";
    cout << "1. Текстовый Редактор\n";
    cout << "2. Игра змейка\n";
    cout << "3. Запись о студентах\n";
    cout << "4. Библиотека\n";
    cout << "5. Банк\n";
    cout << "6. Гараж\n";
    cout << "0. Выйти\n";
    cout << "Выберите действие (0-6): ";
}


void pressEnter() {
    // Имитация нажатия клавиши Enter
    keybd_event(VK_RETURN, 0, 0, 0);
    // Имитация отпускания клавиши Enter
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

int main() {
    setlocale(LC_ALL, "russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char n;
    TextEditor texteditor;
    while (true) {
        SnakeGame game;
        
        printMenu();
        cin >> n;
        system("cls");
        if (n == '0')break;
        switch (n)
        {
        case '1':texteditor.run(); break;
        case '2':game.Run(); break;
        case '3':studentkurs(); break;
        case '4':library(); break;
        case '5':bank(); break;
        case '6':garaj(); break;
        default:
            break;
        }

    }

    return 0;
}