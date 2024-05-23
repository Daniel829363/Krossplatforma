#include <iostream>
#include <string>
#include "TextEditor.cpp"
#include "SnakeGame.cpp"
#include "Header.h"

using namespace std;
void printMenu()
{
    cout << "\n�������� ���� �� ���������\n";
    cout << "============================\n";
    cout << "1. ��������� ��������\n";
    cout << "2. ���� ������\n";
    cout << "3. ������ � ���������\n";
    cout << "4. ����������\n";
    cout << "5. ����\n";
    cout << "6. �����\n";
    cout << "0. �����\n";
    cout << "�������� �������� (0-6): ";
}


void pressEnter() {
    // �������� ������� ������� Enter
    keybd_event(VK_RETURN, 0, 0, 0);
    // �������� ���������� ������� Enter
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