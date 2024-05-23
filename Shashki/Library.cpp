#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <windows.h> 
#include <exception>
using namespace std;

// ��������� ��� ������������� ���������� � �����
struct Book {
    string authors;
    string title;
    string publisher;
    string genre;
    bool available;
    int rating;
};

class Library {
public:
    void addBook() {
        Book newBook;
        cout << "������� ���������� � �����:\n";
        cout << "������: ";
        getline(cin, newBook.authors);
        cout << "��������: ";
        getline(cin, newBook.title);
        cout << "������������: ";
        getline(cin, newBook.publisher);
        cout << "����: ";
        getline(cin, newBook.genre);
        cout << "������� (1 - ��������, 0 - �����������): ";
        cin >> newBook.available;
        cout << "������ (�� 1 �� 10): ";
        cin >> newBook.rating;
        cin.ignore(); // ���������� ������ ����� ������ ����� ����� �����

        library.push_back(newBook);
        cout << "����� ������� ��������� � ����������.\n";
    }

    void saveLibraryToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "������ �������� �����.\n";
            return;
        }

        for (const auto& book : library) {
            file << book.authors << '\n';
            file << book.title << '\n';
            file << book.publisher << '\n';
            file << book.genre << '\n';
            file << book.available << '\n';
            file << book.rating << '\n';
        }

        file.close();
        cout << "���������� ������� ��������� � �����.\n";
    }

    void loadLibraryFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "������ �������� �����.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            Book book;
            book.authors = line;

            if (!getline(file, book.title)) break;
            if (!getline(file, book.publisher)) break;
            if (!getline(file, book.genre)) break;

            string availableStr;
            if (!getline(file, availableStr)) break;
            book.available = (availableStr == "1");

            string ratingStr;
            if (!getline(file, ratingStr)) break;
            book.rating = stoi(ratingStr);

            library.push_back(book);
        }

        file.close();
        cout << "���������� ������� ��������� �� �����.\n";
    }

    void displayLibrary() const {
        if (library.empty()) {
            cout << "���������� �����.\n";
            return;
        }

        cout << "������ ���� � ����������:\n";
        cout << left << setw(5) << "ID" << setw(20) << "������" << setw(30) << "��������" << setw(20) << "������������"
            << setw(15) << "����" << setw(10) << "�������" << setw(10) << "������" << endl;
        for (size_t i = 0; i < library.size(); ++i) {
            const auto& book = library[i];
            cout << left << setw(5) << i << setw(20) << book.authors << setw(30) << book.title << setw(20) << book.publisher
                << setw(15) << book.genre << setw(10) << boolalpha << book.available << setw(10) << book.rating << endl;
        }
    }

    void chooseBook() {
        if (library.empty()) {
            cout << "���������� �����.\n";
            return;
        }

        displayLibrary();

        int bookId;
        cout << "������� ID ����� ��� ��������� �������: ";
        cin >> bookId;
        cin.ignore(); // ���������� ������ ����� ������ ����� ����� �����

        if (bookId < 0 || static_cast<size_t>(bookId) >= library.size()) {
            cout << "�������� ID �����.\n";
            return;
        }

        Book& book = library[bookId];
        if (book.available) {
            cout << "����� \"" << book.title << "\" �����.\n";
            book.available = false;
        }
        else {
            cout << "����� \"" << book.title << "\" ����������.\n";
            book.available = true;
        }
    }

private:
    vector<Book> library;
};


int library() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Library library;
    int choice;

    do {
        cout << "����:\n";
        cout << "1. �������� �����\n";
        cout << "2. ��������� ���������� � ����\n";
        cout << "3. ��������� ���������� �� �����\n";
        cout << "4. �������� ��� �����\n";
        cout << "5. �������� ������ ����� (�����/�������)\n";
        cout << "0. �����\n";
        cout << "�������� ��������: ";
        try {
            cin >> choice;
            cin.ignore(); // ���������� ������ ����� ������ ����� ����� �����

            switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.saveLibraryToFile("library.txt");
                break;
            case 3:
                library.loadLibraryFromFile("library.txt");
                break;
            case 4:
                library.displayLibrary();
                break;
            case 5:
                library.chooseBook();
                break;
            case 0:
                cout << "����� �� ���������.\n";
                system("cls");
                break;
            default:
                cout << "�������� �����, ���������� �����.\n";
            }
        }
        catch (exception& e) {
            cout << "������ "<<e.what()<<"\n";
        }
    } while (choice != 0);

    return 0;
}

