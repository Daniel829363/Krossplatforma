#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>// Для функции replace
#include <locale.h>
using namespace std;


class TextEditor {

    string currentFilename;
    string fileContent;

    // Функция для вывода меню выбора действий
    void printMenu()
    {
        cout << "\n==== Текстовый Редактор ====\n";
        cout << "1. Открыть файл\n";
        cout << "2. Просмотреть содержимое\n";
        cout << "3. Редактировать содержимое\n";
        cout << "4. Поиск и замена\n";
        cout << "5. Сохранить изменения\n";
        cout << "0. Выйти\n";
        cout << "Выберите действие (0-6): ";
    }

    // Функция для создания нового файла
    void createNewFile(const string& filename)
    {
        ofstream newFile(filename);
        if (newFile.is_open())
        {
            cout << "Новый файл '" << filename << "' создан.\n";
        }
        else
        {
            cout << "Ошибка создания нового файла '" << filename << "'.\n";
        }
    }

    // Функция для открытия файла
    void openFile(ifstream& file, string& filename, string& content)
    {
        cout << "Введите имя файла для открытия: ";
        cin.ignore();
        getline(cin, filename);
        file.open(filename);
        if (!file.is_open())
        {
            cout << "Ошибка открытия файла '" << filename << "'. Хотите создать новый файл с этим именем? (Y/N): ";
            char choice;
            cin >> choice;
            if (choice == 'Y' || choice == 'y')
            {
                createNewFile(filename);
            }
        }
        else
        {
            content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
        }
    }

    // Функция для просмотра содержимого файла
    void viewFile(const string& content)
    {
        cout << "\n==== Содержимое файла ====\n";
        cout << content << endl;
    }

    // Функция для поиска и замены текста в содержимом файла
    void findAndReplace(string& content)
    {
        string findStr, replaceStr;
        cout << "Введите строку для поиска: ";
        cin.ignore();
        getline(cin, findStr);
        cout << "Введите строку для замены: ";
        getline(cin, replaceStr);

        size_t pos = content.find(findStr);
        while (pos != string::npos)
        {
            content.replace(pos, findStr.length(), replaceStr);
            pos = content.find(findStr, pos + replaceStr.length());
        }
        cout << "Замена завершена.\n";
    }

    // Функция для сохранения изменений в файле
    void saveFile(ofstream& file, const string& filename, const string& content)
    {
        file.open(filename);
        if (!file.is_open())
        {
            cout << "Ошибка открытия файла: " << filename << endl;
        }
        else
        {
            file << content;
            file.close();
            cout << "Файл успешно сохранен.\n";
        }
    }

    // Функция для редактирования содержимого файла
    void editFile(string& content)
    {
        cout << "Введите новое содержимое файла (введите 'exit' для завершения редактирования):\n";
        content = "";
        string line;
        while (true)
        {
            getline(cin, line);
            if (line == "exit")
            {
                break;
            }
            content += line + "\n";
        }
    }

public:
    int run()
    {
        setlocale(LC_ALL, "russian");
        ifstream inputFile;
        ofstream outputFile;


        while (true)
        {
            printMenu();
            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
                openFile(inputFile, currentFilename, fileContent); // Открыть файл
                break;
            case 2:
                if (!currentFilename.empty())
                {
                    viewFile(fileContent); // Просмотр содержимого файла
                }
                else
                {
                    cout << "Файл не открыт.\n";
                }
                break;
            case 3:
                if (!currentFilename.empty())
                {
                    editFile(fileContent); // Редактировать содержимое файла
                }
                else
                {
                    cout << "Файл не открыт.\n";
                }
                break;
            case 4:
                if (!currentFilename.empty())
                {
                    findAndReplace(fileContent); // Поиск и замена текста
                }
                else
                {
                    cout << "Файл не открыт.\n";
                }
                break;
            case 5:
                if (!currentFilename.empty())
                {
                    saveFile(outputFile, currentFilename, fileContent);
                }
                else
                {
                    cout << "Файл не открыт.\n";
                }
                break;
            case 0:
                cout << "Выход из редактора.\n";
                system("cls");
                return 0;
            default:
                cout << "Недопустимый выбор.\n";
                break;
            }
        }
    }
};