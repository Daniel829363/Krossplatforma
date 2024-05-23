#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>// ��� ������� replace
#include <locale.h>
using namespace std;


class TextEditor {

    string currentFilename;
    string fileContent;

    // ������� ��� ������ ���� ������ ��������
    void printMenu()
    {
        cout << "\n==== ��������� �������� ====\n";
        cout << "1. ������� ����\n";
        cout << "2. ����������� ����������\n";
        cout << "3. ������������� ����������\n";
        cout << "4. ����� � ������\n";
        cout << "5. ��������� ���������\n";
        cout << "0. �����\n";
        cout << "�������� �������� (0-6): ";
    }

    // ������� ��� �������� ������ �����
    void createNewFile(const string& filename)
    {
        ofstream newFile(filename);
        if (newFile.is_open())
        {
            cout << "����� ���� '" << filename << "' ������.\n";
        }
        else
        {
            cout << "������ �������� ������ ����� '" << filename << "'.\n";
        }
    }

    // ������� ��� �������� �����
    void openFile(ifstream& file, string& filename, string& content)
    {
        cout << "������� ��� ����� ��� ��������: ";
        cin.ignore();
        getline(cin, filename);
        file.open(filename);
        if (!file.is_open())
        {
            cout << "������ �������� ����� '" << filename << "'. ������ ������� ����� ���� � ���� ������? (Y/N): ";
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

    // ������� ��� ��������� ����������� �����
    void viewFile(const string& content)
    {
        cout << "\n==== ���������� ����� ====\n";
        cout << content << endl;
    }

    // ������� ��� ������ � ������ ������ � ���������� �����
    void findAndReplace(string& content)
    {
        string findStr, replaceStr;
        cout << "������� ������ ��� ������: ";
        cin.ignore();
        getline(cin, findStr);
        cout << "������� ������ ��� ������: ";
        getline(cin, replaceStr);

        size_t pos = content.find(findStr);
        while (pos != string::npos)
        {
            content.replace(pos, findStr.length(), replaceStr);
            pos = content.find(findStr, pos + replaceStr.length());
        }
        cout << "������ ���������.\n";
    }

    // ������� ��� ���������� ��������� � �����
    void saveFile(ofstream& file, const string& filename, const string& content)
    {
        file.open(filename);
        if (!file.is_open())
        {
            cout << "������ �������� �����: " << filename << endl;
        }
        else
        {
            file << content;
            file.close();
            cout << "���� ������� ��������.\n";
        }
    }

    // ������� ��� �������������� ����������� �����
    void editFile(string& content)
    {
        cout << "������� ����� ���������� ����� (������� 'exit' ��� ���������� ��������������):\n";
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
                openFile(inputFile, currentFilename, fileContent); // ������� ����
                break;
            case 2:
                if (!currentFilename.empty())
                {
                    viewFile(fileContent); // �������� ����������� �����
                }
                else
                {
                    cout << "���� �� ������.\n";
                }
                break;
            case 3:
                if (!currentFilename.empty())
                {
                    editFile(fileContent); // ������������� ���������� �����
                }
                else
                {
                    cout << "���� �� ������.\n";
                }
                break;
            case 4:
                if (!currentFilename.empty())
                {
                    findAndReplace(fileContent); // ����� � ������ ������
                }
                else
                {
                    cout << "���� �� ������.\n";
                }
                break;
            case 5:
                if (!currentFilename.empty())
                {
                    saveFile(outputFile, currentFilename, fileContent);
                }
                else
                {
                    cout << "���� �� ������.\n";
                }
                break;
            case 0:
                cout << "����� �� ���������.\n";
                system("cls");
                return 0;
            default:
                cout << "������������ �����.\n";
                break;
            }
        }
    }
};