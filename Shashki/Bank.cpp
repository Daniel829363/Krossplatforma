#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <sstream>
#include <iomanip>
using namespace std;
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
#define SQL_QUERY_LEN 1000

class SQLConnectt {
private:
    SQLHANDLE sqlenvhandle;
    SQLHANDLE sqlconnectionhandle;
    SQLHANDLE sqlstatementhandle;
    SQLRETURN retcode;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
public:
    SQLConnectt()
    {
        // Allocate environment handle
        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle) != SQL_SUCCESS) {
            return;
        }

        // Set the ODBC version environment attribute
        if (SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS) {
            return;
        }

        // Allocate connection handle
        if (SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle) != SQL_SUCCESS) {
            return;
        }

        // Connect to the SQL Server
        retcode = SQLDriverConnectW(sqlconnectionhandle, NULL, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(L"DRIVER={SQL Server};SERVER=.;DATABASE=test;UID=sa2;PWD=1234;")), SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT);

        // Allocate statement handle
        if (SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle) != SQL_SUCCESS) {
            return;
        }
    }
    ~SQLConnectt()
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
        SQLDisconnect(sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
    }

    void showAllBankAccounts() //������ ������� ��� ����������� ������ �� ��.
    {
        SQLRETURN retcode;

        SQLWCHAR sqlquery[] = L"SELECT ID, AccountNumber, DepositCategory, PassportData, CurrentBalance, LastTransactionDate FROM BankAccounts";
        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, sqlquery, SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        // �������� ��������
        SQLINTEGER id;
        SQLWCHAR accountNumber[SQL_RESULT_LEN];
        SQLWCHAR depositCategory[SQL_RESULT_LEN];
        SQLWCHAR passportData[SQL_RESULT_LEN];
        SQLDOUBLE balance;
        SQL_TIMESTAMP_STRUCT lastDate;


        SQLBindCol(sqlstatementhandle, 1, SQL_INTEGER, &id, sizeof(id), NULL);
        SQLBindCol(sqlstatementhandle, 2, SQL_C_WCHAR, &accountNumber, SQL_RESULT_LEN, NULL);
        SQLBindCol(sqlstatementhandle, 3, SQL_C_WCHAR, &depositCategory, SQL_RESULT_LEN, NULL);
        SQLBindCol(sqlstatementhandle, 4, SQL_C_WCHAR, &passportData, SQL_RESULT_LEN, NULL);
        SQLBindCol(sqlstatementhandle, 5, SQL_C_DOUBLE, &balance, sizeof(balance), NULL);
        //����� ����
        //SQLBindCol(sqlstatementhandle, 6, SQL_DATE, &lastDate, sizeof(lastDate), NULL);
        SQLBindCol(sqlstatementhandle, 6, SQL_C_TYPE_TIMESTAMP, &lastDate, sizeof(lastDate), NULL);



        // ����� ���������� �������
        std::wcout << std::setw(4) << L"ID" << std::setw(20) << L"Account Number" << std::setw(20) << L"Deposit Category"
            << std::setw(20) << L"Passport Data" << std::setw(18) << L"Current Balance" << std::setw(20) << L"Last Date" << std::endl;

        // ��������� � ����� �����������
        while (SQLFetch(sqlstatementhandle) == SQL_SUCCESS) {
            std::wcout << std::setw(4) << id << std::setw(20) << accountNumber << std::setw(20) << depositCategory
                << std::setw(20) << passportData << std::setw(18) << balance
                << std::setw(20) << lastDate.year << L"-" << lastDate.month << L"-" << lastDate.day << std::endl;
        }
    }

    void executeInsert() {
        SQLRETURN retcode;

        // ������ ������ �� ������������
        std::wcout << L"Enter Account Number: ";
        std::wstring accountNumber;
        std::getline(std::wcin >> std::ws, accountNumber);

        std::wcout << L"Enter Deposit Category: ";
        std::wstring depositCategory;
        std::getline(std::wcin >> std::ws, depositCategory);

        std::wcout << L"Enter Passport Data: ";
        std::wstring passportData;
        std::getline(std::wcin >> std::ws, passportData);

        std::wcout << L"Enter Current Balance: ";
        double currentBalance;
        std::wcin >> currentBalance;

        // ���������� SQL ������� � �������������� ��������������
        SQLWCHAR sqlquery[1024];
        swprintf_s(sqlquery, L"INSERT INTO BankAccounts (AccountNumber, DepositCategory, PassportData, CurrentBalance, LastTransactionDate) VALUES ('%s', '%s', '%s', %f, GETDATE())", accountNumber.c_str(), depositCategory.c_str(), passportData.c_str(), currentBalance);
        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, sqlquery, SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"Insertion successful" << std::endl;
            system("pause");
            system("cls");
        }
        else {
            std::wcerr << L"Insertion failed" << std::endl;
        }
    }

    bool doesAccountExist(const std::wstring& accountNumber) {
        SQLRETURN retcode;

        // ���������� SQL ������� ��� �������� ������� ����� � ���� ������
        std::wstring SQLQuery = L"SELECT COUNT(*) FROM BankAccounts WHERE AccountNumber = '" + accountNumber + L"'";
        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(SQLQuery.c_str())), SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
            std::wcerr << L"Error while executing SQL query." << std::endl;
            return false; // � ������ ������ ������ false
        }

        SQLINTEGER rowCount = 0;
        SQLBindCol(sqlstatementhandle, 1, SQL_C_LONG, &rowCount, sizeof(rowCount), NULL);

        retcode = SQLFetch(sqlstatementhandle);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
            std::wcerr << L"Error while fetching row count." << std::endl;
            return false; // � ������ ������ ������ false
        }

        // ���� ���������� ����� ������ ����, �� ���� � ����� ������� ��� ����������
        return rowCount > 0;
    }

    void depositToAccount(const std::wstring& accountNumber, double amount) {
        SQLRETURN retcode;

        // ���������� SQL ������� ��� ���������� ������� �����
        std::wstring SQLQuery = L"UPDATE BankAccounts SET CurrentBalance = CurrentBalance + " + std::to_wstring(amount) + L", LastTransactionDate = GETDATE() WHERE AccountNumber = '" + accountNumber + L"'";

        // ���������� SQL �������
        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(SQLQuery.c_str())), SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"Deposit successful" << std::endl;
            system("pause");
            system("cls");
        }
        else {
            // �������� ��������������� ��������� �� ������
            SQLWCHAR state[6]; // ����������� ������ ��� ���� ���������
            SQLINTEGER error;
            SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH];
            SQLSMALLINT messageLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, sqlstatementhandle, 1, state, &error, message, SQL_MAX_MESSAGE_LENGTH, &messageLength);

            // ������� ��������� �� ������
            std::wcerr << L"Deposit failed. SQL state: " << state << L", Error: " << error << L", Message: " << message << std::endl;
        }

    }

    void drawToAccount(const std::wstring& accountNumber, double amount) {
        SQLRETURN retcode;
        amount = -1 * amount;
        // ���������� SQL ������� ��� ���������� ������� �����
        std::wstring SQLQuery = L"UPDATE BankAccounts SET CurrentBalance = CurrentBalance + " + std::to_wstring(amount) + L", LastTransactionDate = GETDATE() WHERE AccountNumber = '" + accountNumber + L"'";

        // ���������� SQL �������
        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(SQLQuery.c_str())), SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"Deposit successful" << std::endl;
            system("pause");
            system("cls");

        }
        else {
            // �������� ��������������� ��������� �� ������
            SQLWCHAR state[6]; // ����������� ������ ��� ���� ���������
            SQLINTEGER error;
            SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH];
            SQLSMALLINT messageLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, sqlstatementhandle, 1, state, &error, message, SQL_MAX_MESSAGE_LENGTH, &messageLength);

            // ������� ��������� �� ������
            std::wcerr << L"Deposit failed. SQL state: " << state << L", Error: " << error << L", Message: " << message << std::endl;
        }

    }

    void callCalculateInterest(const std::wstring& accountNumber) {
        SQLRETURN retcode;

        // ���������� SQL ������� ��� ������ �������� ��������� CalculateInterest
        std::wstring SQLQuery = L"EXEC dbo.CalculateInterest '" + accountNumber + L"'";

        // ���������� SQL �������
        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(SQLQuery.c_str())), SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"CalculateInterest procedure called successfully" << std::endl;
            system("pause");
            system("cls");
        }
        else {
            // �������� ��������������� ��������� �� ������
            SQLWCHAR state[6]; // ����������� ������ ��� ���� ���������
            SQLINTEGER error;
            SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH];
            SQLSMALLINT messageLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, sqlstatementhandle, 1, state, &error, message, SQL_MAX_MESSAGE_LENGTH, &messageLength);

            // ������� ��������� �� ������
            std::wcerr << L"Failed to call CalculateInterest procedure. SQL state: " << state << L", Error: " << error << L", Message: " << message << std::endl;
        }
    }


    void showBankAccountByNumber(const std::wstring& accountNumber) // ������� ��� ����������� ������ �� ������ �����
    {
        SQLRETURN retcode;
        SQLCloseCursor(sqlstatementhandle);
        // �������� SQL ������� � ����������
        SQLWCHAR sqlquery[SQL_QUERY_LEN];
        swprintf(sqlquery, SQL_QUERY_LEN, L"SELECT ID, AccountNumber, DepositCategory, PassportData, CurrentBalance, LastTransactionDate FROM BankAccounts WHERE AccountNumber = '%s'", accountNumber.c_str());

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, sqlquery, SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        // �������� ��������
        SQLINTEGER id;
        SQLWCHAR accountNumberResult[SQL_RESULT_LEN];
        SQLWCHAR depositCategory[SQL_RESULT_LEN];
        SQLWCHAR passportData[SQL_RESULT_LEN];
        SQLDOUBLE balance;
        SQL_TIMESTAMP_STRUCT lastDate;

        SQLBindCol(sqlstatementhandle, 1, SQL_INTEGER, &id, sizeof(id), NULL);
        SQLBindCol(sqlstatementhandle, 2, SQL_C_WCHAR, &accountNumberResult, SQL_RESULT_LEN, NULL);
        SQLBindCol(sqlstatementhandle, 3, SQL_C_WCHAR, &depositCategory, SQL_RESULT_LEN, NULL);
        SQLBindCol(sqlstatementhandle, 4, SQL_C_WCHAR, &passportData, SQL_RESULT_LEN, NULL);
        SQLBindCol(sqlstatementhandle, 5, SQL_C_DOUBLE, &balance, sizeof(balance), NULL);
        // SQLBindCol(sqlstatementhandle, 6, SQL_DATE, &lastDate, sizeof(lastDate), NULL);
        SQLBindCol(sqlstatementhandle, 6, SQL_C_TYPE_TIMESTAMP, &lastDate, sizeof(lastDate), NULL);
        // ��������� � ����� �����������
        while (SQLFetch(sqlstatementhandle) == SQL_SUCCESS) {
            std::wcout << L"ID: " << id << L", Account Number: " << accountNumberResult << std::endl;
            std::wcout << L"Deposit Category: " << depositCategory << std::endl;
            std::wcout << L"Passport Data: " << passportData << std::endl;
            std::wcout << L"Current Balance: " << balance << std::endl;
            // �������������� ���� � ������ ��� ������
            std::wcout << L"Last Transaction Date: " << lastDate.year << L"-" << lastDate.month << L"-" << lastDate.day << std::endl;
            std::cout << std::endl;
        }
    }

    void calculateInterest() {
        SQLRETURN retcode;

        // ���������� SQL ������� ��� ���������� ���������
        SQLWCHAR sqlquery[] = L"UPDATE BankAccounts SET CurrentBalance = \
                            CASE \
                                WHEN DepositCategory = 'long_term' THEN CurrentBalance * 1.04 \
                                WHEN DepositCategory = 'short_term' THEN CurrentBalance * 1.02 \
                                ELSE CurrentBalance \
                            END";

        SQLCloseCursor(sqlstatementhandle); // ������� ������, ���� �� ������

        // ���������� SQL �������
        retcode = SQLExecDirectW(sqlstatementhandle, sqlquery, SQL_NTS);
        std::wcout << L"Return code: " << retcode << std::endl; // ��������� ��� ������ ��� �������
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"Interest calculation successful" << std::endl;
            system("pause");
            system("cls");

        }
        else {
            // �������� ��������������� ��������� �� ������
            SQLWCHAR state[6]; // ����������� ������ ��� ���� ���������
            SQLINTEGER error;
            SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH];
            SQLSMALLINT messageLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, sqlstatementhandle, 1, state, &error, message, SQL_MAX_MESSAGE_LENGTH, &messageLength);

            // ������� ��������� �� ������
            std::wcerr << L"Interest calculation failed. SQL state: " << state << L", Error: " << error << L", Message: " << message << std::endl;
        }
    }


};

class BankAccount : public SQLConnectt {
private:
    string account_number; //������� ���� ��������
    string deposit_category; // ��� ��������
    string passport_data; // ����������� ������
    double current_balance; // ������� ������
    tm last_transaction_date; // ���� ��������� ��������
public:
    BankAccount()
        : account_number(""), deposit_category(""), passport_data(""), current_balance(0.0) {
        time_t now;
        time(&now);
        localtime_s(&last_transaction_date, &now);
    }

    BankAccount(std::string acc_number, std::string deposit_cat, std::string pass_data, double balance)
        : account_number(acc_number), deposit_category(deposit_cat), passport_data(pass_data), current_balance(balance) {
        time_t now;
        time(&now);
        localtime_s(&last_transaction_date, &now);
    }

    void deposit(double amount) {
        current_balance += amount;
        time_t now;
        time(&now);
        localtime_s(&last_transaction_date, &now);
        std::cout << "Deposit of " << amount << " made successfully. Current balance: " << current_balance << std::endl;
    }

    void withdraw(double amount) {
        if (amount > current_balance) {
            std::cout << "Insufficient funds!" << std::endl;
        }
        else {
            current_balance -= amount;
            time_t now;
            time(&now);
            localtime_s(&last_transaction_date, &now);
            std::cout << "Withdrawal of " << amount << " made successfully. Current balance: " << current_balance << std::endl;
        }
    }
    void calculateInterest2() {
        double interest_rate = 0.0;
        if (deposit_category == "long_term") {
            interest_rate = 0.05; // ���������� ������ ��� ������������� ��������
        }
        else if (deposit_category == "short_term") {
            interest_rate = 0.03; // ���������� ������ ��� �������� ��������
        }

        // ������ ���������


        double interest = current_balance * interest_rate / 12.0;
        current_balance += interest;

        std::cout << "Interest calculated and added to the account. Current balance: " << current_balance << std::endl;
    }

    double getBalance() const {
        return current_balance;
    }
};

int bank()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    SQLConnectt sqlconnect;
    BankAccount bankAccout;
    int choice = -1;

    while (choice != 0) {
        cout << "=== ������� ���� ===" << endl;

        cout << "1 - ���� ������ ������������ �����" << endl;
        cout << "2 - ����� ���������� � ���� ������������� �����" << endl;
        cout << "3 - ������ � ��������� �������������" << endl;
        cout << "4 - ������� ��������� ��� ���� �������������" << endl;
        cout << "0 - �����" << endl;
        cout << "�������� �����: ";
        cin >> choice;
        system("cls");
        switch (choice) {
        case 0:
            cout << "����� �� ���������." << endl;
            break;
        case 1:
        {

            // ������ ��� ����� ������ ������������ �����
            bankAccout.executeInsert();
            system("cls");
            break;
        }
        case 2:
        {
            // ������ ��� ������ ���������� � ���� ������������� �����
            bankAccout.showAllBankAccounts();
            system("pause");
            system("cls");
            break;
        }
        case 3:
        {
            // ������ ��� ������ � ��������� �������������
            //��� ���� ������� ����� 
            std::wstring accountNumber;
            cout << "������� ����� �����: ";
            getline(std::wcin >> std::ws, accountNumber);
            if (bankAccout.doesAccountExist(accountNumber)) //��� �������� �� �� ��� ���� �� ����� ������������ � ����� Account Number
            {

                int userChoice = -1;
                while (userChoice != 0) {
                    cout << "=== ���� ������ � ��������� ������������� ===" << endl;
                    cout << "0 - ����� � ������� ����" << endl;
                    cout << "1 - �������" << endl;
                    cout << "2 - ����������" << endl;
                    cout << "3 - Calculate Interest" << endl;
                    cout << "4 - ������ ����������" << endl;


                    cout << "�������� �����: ";
                    cin >> userChoice;

                    switch (userChoice) {
                    case 0:
                        cout << "����� � ������� ����." << endl;
                        system("cls");
                        break;
                    case 1:
                    {
                        double dep = 0;
                        cout << "������� ����� ��������: ";
                        cin >> dep;
                        bankAccout.depositToAccount(accountNumber, dep);
                        // ������ ��� ��������

                        break;
                    }
                    case 2:
                    {
                        double dep = 0;
                        cout << "������� ����� �������������: ";
                        cin >> dep;
                        bankAccout.drawToAccount(accountNumber, dep);
                        // ������ ��� �������������
                        break;
                    }
                    case 3:
                    {
                        bankAccout.callCalculateInterest(accountNumber);
                        //cout << "�������� ���������� � ��������� �� ����.\n";
                        // ������ ��� ������� ���������
                        break;
                    }
                    case 4:
                    {
                        bankAccout.showBankAccountByNumber(accountNumber);
                        system("pause");
                        system("cls");
                        break;
                    }

                    default:
                        cout << "������������ �����. ����������, �������� ����� �� 0 �� 4." << endl;
                        break;
                    }
                }
            }
            else//���� ��� �� �������� ��������������� ���������
            {
                cout << "���� ������ ������������.\n";
            }
            break;
        }
        case 4:
        {
            bankAccout.calculateInterest();
            break;
        }
        default:
            cout << "������������ �����. ����������, �������� ����� �� 0 �� 4." << endl;
            break;
        }
    }

    system("pause");
    return 0;
}

