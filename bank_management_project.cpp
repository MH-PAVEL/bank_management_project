#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

struct Account {
    string accountNumber;
    string name;
    string password;
    double balance;
    Account* next;
};

class BankManagementSystem {
private:
    Account* head;
    string dataFile; // Name of the file for data persistence

public:
    BankManagementSystem() : head(nullptr), dataFile("accounts.txt") {
        // Initialize the head pointer to nullptr to start with an empty list.
        head = nullptr;
    }

    void addAccount() {
        Account* newAccount = new Account;
        cout << "    Enter Account Number: ";
        cin >> newAccount->accountNumber;

        if (isAccountNumberUnique(newAccount->accountNumber)) {
            cout << "    Enter Name: ";
            cin.ignore();
            getline(cin, newAccount->name);
            cout << "    Set Password: ";
            cin >> newAccount->password;
            cout << "    Enter Initial Balance: ";
            cin >> newAccount->balance;

            newAccount->next = nullptr;
            if (head == nullptr) {
                head = newAccount;
            } else {
                Account* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newAccount;
            }

            cout << "\n    Account Added Successfully!" << endl;

            // Saving the account to the file
            saveAccountsToFile();
        } else {
            cout << "\n    Account Number is not unique. Please enter a unique account number." << endl;
            delete newAccount; // Removing memory for the invalid account
        }
    }

    bool isAccountNumberUnique(const string& accountNumber) {
        Account* current = head;
        while (current != nullptr) {
            if (current->accountNumber == accountNumber) {
                return false; // Account number already exists
            }
            current = current->next;
        }
        return true; // Account number is unique
    }

    Account* findAccount(const string& accountNumber, const string& password) {
        Account* current = head;
        while (current != nullptr) {
            if (current->accountNumber == accountNumber && current->password == password) {
                return current; // Account found with matching account number and password
            }
            current = current->next;
        }
        return nullptr; // Account not found or password doesn't match
    }

    void searchAccount() {
        string accountNumber;
        cout << "    Enter Account Number: ";
        cin >> accountNumber;

        string password;
        cout << "    Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);
        if (account != nullptr) {
            cout << "\n    Account Number: " << account->accountNumber << endl;
            cout << "    Name: " << account->name << endl;
            cout << "    Balance: " << fixed << setprecision(2) << account->balance << endl;
        } else {
            cout << "\n    Account Not Found or Invalid Password!" << endl;
        }
    }

    void depositFunds() {
        string accountNumber;
        cout << "    Enter Account Number: ";
        cin >> accountNumber;

        string password;
        cout << "    Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);
        if (account != nullptr) {
            double amount;
            cout << "    Enter Deposit Amount: ";
            cin >> amount;

            account->balance += amount;
            cout << "\n    Deposit Successful!" << endl;
            // Saving the updated balance to the file
            saveAccountsToFile();
        } else {
            cout << "\n    Account Not Found or Invalid Password!" << endl;
        }
    }

    void withdrawFunds() {
        string accountNumber;
        cout << "    Enter Account Number: ";
        cin >> accountNumber;

        string password;
        cout << "    Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);
        if (account != nullptr) {
            double amount;
            cout << "    Enter Withdrawal Amount: ";
            cin >> amount;

            if (account->balance >= amount) {
                account->balance -= amount;
                cout << "\n    Withdrawal Successful!" << endl;
                // Saving the updated balance to the file
                saveAccountsToFile();
            } else {
                cout << "\n    Insufficient Funds!" << endl;
            }
        } else {
            cout << "\n    Account Not Found or Invalid Password!" << endl;
        }
    }

    void displayAccounts() {
        cout << "\n    --- All Accounts ---" << endl;
        Account* current = head;
        while (current != nullptr) {
            cout << "\n    Account Number: " << current->accountNumber << endl;
            cout << "    Name: " << current->name << endl;
            cout << "    Balance: " << fixed << setprecision(2) << current->balance << endl;
            current = current->next;
        }
        cout << endl;
    }

    void saveAccountsToFile() {
        ofstream outputFile(dataFile);

        if (outputFile.is_open()) {
            Account* current = head;
            while (current != nullptr) {
                outputFile << current->accountNumber << "," << current->name << "," << current->password << "," << fixed << setprecision(2) << current->balance << endl;
                current = current->next;
            }

            outputFile.close();

            cout << "\n    Accounts Saved To File: " << dataFile << endl;
        } else {
            cout << "\n    Error: Unable to open file for writing." << endl;
        }
    }

    void loadAccountsFromFile() {
        ifstream inputFile(dataFile);

        if (inputFile.is_open()) {
            string line;
            while (getline(inputFile, line)) {
                stringstream ss(line);
                string accountNumber, name, password;
                double balance;

                getline(ss, accountNumber, ',');
                getline(ss, name, ',');
                getline(ss, password, ',');
                ss >> balance;

                Account* newAccount = new Account;
                newAccount->accountNumber = accountNumber;
                newAccount->name = name;
                newAccount->password = password;
                newAccount->balance = balance;
                newAccount->next = nullptr;

                if (head == nullptr) {
                    head = newAccount;
                } else {
                    Account* current = head;
                    while (current->next != nullptr) {
                        current = current->next;
                    }
                    current->next = newAccount;
                }
            }

            inputFile.close();

            cout << "\n    Accounts loaded from file: " << dataFile << endl;
        } else {
            cout << "\n    Error: Unable to open file for reading." << endl;
        }
    }

    void menu() {
        loadAccountsFromFile();
        int choice;

        while (true) {
            cout << "\n    --- Bank Management System ---" << endl;
            cout << "    1. Add Account" << endl;
            cout << "    2. Search Account" << endl;
            cout << "    3. Deposit Funds" << endl;
            cout << "    4. Withdraw Funds" << endl;
            cout << "    5. Display All Accounts" << endl;
            cout << "    6. Exit Menu" << endl;
            cout << "    Enter Your Choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addAccount();
                    break;
                case 2:
                    searchAccount();
                    break;
                case 3:
                    depositFunds();
                    break;
                case 4:
                    withdrawFunds();
                    break;
                case 5:
                    displayAccounts();
                    break;
                case 6:
                    return;
                default:
                    cout << "    Invalid Choice!" << endl;
            }
        }
    }
};

int main() {
    BankManagementSystem bankManagementSystem;
    bankManagementSystem.menu();
    return 0;
}
