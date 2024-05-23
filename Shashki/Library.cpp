#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <windows.h> 
#include <exception>
using namespace std;

// Структура для представления информации о книге
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
        cout << "Введите информацию о книге:\n";
        cout << "Авторы: ";
        getline(cin, newBook.authors);
        cout << "Название: ";
        getline(cin, newBook.title);
        cout << "Издательство: ";
        getline(cin, newBook.publisher);
        cout << "Жанр: ";
        getline(cin, newBook.genre);
        cout << "Наличие (1 - доступна, 0 - отсутствует): ";
        cin >> newBook.available;
        cout << "Оценка (от 1 до 10): ";
        cin >> newBook.rating;
        cin.ignore(); // Игнорируем символ новой строки после ввода числа

        library.push_back(newBook);
        cout << "Книга успешно добавлена в библиотеку.\n";
    }

    void saveLibraryToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка открытия файла.\n";
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
        cout << "Библиотека успешно сохранена в файле.\n";
    }

    void loadLibraryFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка открытия файла.\n";
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
        cout << "Библиотека успешно загружена из файла.\n";
    }

    void displayLibrary() const {
        if (library.empty()) {
            cout << "Библиотека пуста.\n";
            return;
        }

        cout << "Список книг в библиотеке:\n";
        cout << left << setw(5) << "ID" << setw(20) << "Авторы" << setw(30) << "Название" << setw(20) << "Издательство"
            << setw(15) << "Жанр" << setw(10) << "Наличие" << setw(10) << "Оценка" << endl;
        for (size_t i = 0; i < library.size(); ++i) {
            const auto& book = library[i];
            cout << left << setw(5) << i << setw(20) << book.authors << setw(30) << book.title << setw(20) << book.publisher
                << setw(15) << book.genre << setw(10) << boolalpha << book.available << setw(10) << book.rating << endl;
        }
    }

    void chooseBook() {
        if (library.empty()) {
            cout << "Библиотека пуста.\n";
            return;
        }

        displayLibrary();

        int bookId;
        cout << "Введите ID книги для изменения статуса: ";
        cin >> bookId;
        cin.ignore(); // Игнорируем символ новой строки после ввода числа

        if (bookId < 0 || static_cast<size_t>(bookId) >= library.size()) {
            cout << "Неверный ID книги.\n";
            return;
        }

        Book& book = library[bookId];
        if (book.available) {
            cout << "Книга \"" << book.title << "\" взята.\n";
            book.available = false;
        }
        else {
            cout << "Книга \"" << book.title << "\" возвращена.\n";
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
        cout << "Меню:\n";
        cout << "1. Добавить книгу\n";
        cout << "2. Сохранить библиотеку в файл\n";
        cout << "3. Загрузить библиотеку из файла\n";
        cout << "4. Показать все книги\n";
        cout << "5. Изменить статус книги (взять/вернуть)\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        try {
            cin >> choice;
            cin.ignore(); // Игнорируем символ новой строки после ввода числа

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
                cout << "Выход из программы.\n";
                system("cls");
                break;
            default:
                cout << "Неверный выбор, попробуйте снова.\n";
            }
        }
        catch (exception& e) {
            cout << "Ошибка "<<e.what()<<"\n";
        }
    } while (choice != 0);

    return 0;
}

