#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cctype>
#include <queue> // For transaction tracking
using namespace std;

// Define account and node classes
class account {
    int acno;
    char name[50];
    int deposit;
    char type;

public:
    void create_account();
    void show_account() const;
    void modify();
    void dep(int);
    void draw(int);
    void report() const;
    int retacno() const;
    int retdeposit() const;
    char rettype() const;

    void save_to_file(ofstream &out);
    void load_from_file(ifstream &in);
};

class node {
public:
    account ac;
    node *next;
    node() : next(nullptr) {}
};

node *head = nullptr;
queue<string> transactions; // Global transaction queue

// Account member functions
void account::create_account() {
    system("CLS");
    cout << "\n\t\t\tEnter the Account No.: ";
    while (!(cin >> acno)) {
        cout << "\n\t\t\tPlease enter a valid integer account number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cout << "\n\t\t\tEnter the Name of the Account holder: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tEnter Type of the Account (C/S): ";
    while (!(cin >> type) || (type != 'C' && type != 'S')) {
        cout << "\n\t\t\tPlease enter a valid account type (C/S): ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    type = toupper(type);
    cout << "\n\t\t\tEnter The Initial amount: ";
    while (!(cin >> deposit) || deposit <= 0) {
        cout << "\n\t\t\tPlease enter a valid positive deposit amount: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cout << "\n\n\t\t\tAccount Created.";
}

void account::show_account() const {
    cout << "\n\t\t\tAccount No.      : " << acno;
    cout << "\n\t\t\tAccount Holder   : " << name;
    cout << "\n\t\t\tType of Account  : " << type;
    cout << "\n\t\t\tBalance Amount   : " << deposit;
}

void account::modify() {
    cout << "\n\t\t\tAccount No.: " << acno;
    cout << "\n\t\t\tModify Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tModify Type of Account (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tModify Balance Amount: ";
    cin >> deposit;
}

void account::dep(int x) {
    deposit += x;
}

void account::draw(int x) {
    deposit -= x;
}

void account::report() const {
    string acno_str = to_string(acno);
    cout << acno_str;
    cout << string(12 - acno_str.length(), ' ') << name;
    cout << string(16 - strlen(name), ' ') << type;
    cout << string(8, ' ') << deposit << endl;
}

int account::retacno() const { return acno; }
int account::retdeposit() const { return deposit; }
char account::rettype() const { return type; }

// File operations
void account::save_to_file(ofstream &out) {
    out << acno << '\n'
        << name << '\n'
        << type << '\n'
        << deposit << '\n';
}

void account::load_from_file(ifstream &in) {
    in >> acno;
    in.ignore();
    in.getline(name, 50);
    in >> type >> deposit;
    in.ignore();
}

// Functionality functions
void write_account() {
    account ac;
    node *new_node = new node;
    ac.create_account();
    new_node->ac = ac;
    new_node->next = head;
    head = new_node;

    ofstream out("accounts.txt", ios::app);
    if (out) {
        ac.save_to_file(out);
    }
}

void load_accounts() {
    ifstream in("accounts.txt");
    if (in) {
        while (in.peek() != EOF) {
            node *new_node = new node;
            new_node->ac.load_from_file(in);
            if (in) {
                new_node->next = head;
                head = new_node;
            } else {
                delete new_node;
            }
        }
    }
}

void display_sp(int n) {
    node *temp = head;
    bool found = false;
    while (temp) {
        if (temp->ac.retacno() == n) {
            temp->ac.show_account();
            found = true;
            break;
        }
        temp = temp->next;
    }
    if (!found) {
        cout << "\n\n\t\t\tAccount number does not exist.";
    }
}

void delete_account(int n) {
    node *temp = head;
    node *prev = nullptr;
    bool found = false;

    while (temp) {
        if (temp->ac.retacno() == n) {
            if (prev) {
                prev->next = temp->next;
            } else {
                head = temp->next;
            }
            delete temp;
            cout << "\n\nRecord Deleted.";
            found = true;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!found) {
        cout << "\n\nAccount not found.";
    }

    ofstream out("accounts.txt");
    node *current = head;
    while (current) {
        current->ac.save_to_file(out);
        current = current->next;
    }
}

void modify_account(int n) {
    node *temp = head;
    bool found = false;

    while (temp) {
        if (temp->ac.retacno() == n) {
            temp->ac.show_account();
            cout << "\n\n\t\t\tEnter The New Details of account:" << endl;
            temp->ac.modify();
            cout << "\n\n\t\t\tRecord Updated.";
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "\n\n\t\t\tRecord Not Found.";
    }

    ofstream out("accounts.txt");
    node *current = head;
    while (current) {
        current->ac.save_to_file(out);
        current = current->next;
    }
}

void log_transaction(int account_no, const string &type, int amount, int new_balance) {
    string record = "Account No: " + to_string(account_no) +
                    "\nTransaction: " + type +
                    "\nAmount: " + to_string(amount) +
                    "\nNew Balance: " + to_string(new_balance) +
                    "\n-----------------------------\n";
    transactions.push(record);

    ofstream receipt("receipt.txt", ios::app);
    if (receipt) {
        receipt << record;
        receipt.close();
    }
}

void deposit_withdraw(int n, int option) {
    int amt;
    bool found = false;
    node *temp = head;

    while (temp) {
        if (temp->ac.retacno() == n) {
            temp->ac.show_account();
            if (option == 1) {
                cout << "\n\n\t\t\tTO DEPOSIT AMOUNT";
                cout << "\n\n\t\t\tEnter The amount to deposit: ";
                cin >> amt;
                temp->ac.dep(amt);
                log_transaction(n, "Deposit", amt, temp->ac.retdeposit());
            } else if (option == 2) {
                cout << "\n\n\t\t\tTO WITHDRAW AMOUNT";
                cout << "\n\n\t\t\tEnter The amount to withdraw: ";
                cin >> amt;
                if (amt <= temp->ac.retdeposit()) {
                    temp->ac.draw(amt);
                    log_transaction(n, "Withdrawal", amt, temp->ac.retdeposit());
                } else {
                    cout << "\n\n\t\t\tInsufficient balance.";
                }
            }
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "\n\n\t\t\tRecord Not Found.";
    }

    ofstream out("accounts.txt");
    node *current = head;
    while (current) {
        current->ac.save_to_file(out);
        current = current->next;
    }
}

void transfer_amount() {
    if (!head || !head->next) {
        cout << "\n\n\t\t\tTransfer operation cannot be performed. At least two accounts must exist.";
        return;
    }

    int from_ac, to_ac, amount;
    node *from_node = nullptr, *to_node = nullptr;

    cout << "\n\n\t\t\tEnter the Account No. to transfer FROM: ";
    cin >> from_ac;
    cout << "\n\n\t\t\tEnter the Account No. to transfer TO: ";
    cin >> to_ac;

    if (from_ac == to_ac) {
        cout << "\n\n\t\t\tYou cannot transfer to the same account.";
        return;
    }

    node *temp = head;
    while (temp) {
        if (temp->ac.retacno() == from_ac) {
            from_node = temp;
        } else if (temp->ac.retacno() == to_ac) {
            to_node = temp;
        }
        temp = temp->next;
    }

    if (!from_node || !to_node) {
        cout << "\n\n\t\t\tOne or both account numbers do not exist.";
        return;
    }

    cout << "\n\n\t\t\tEnter the amount to transfer: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "\n\n\t\t\tInvalid transfer amount.";
        return;
    }

    if (from_node->ac.retdeposit() < amount) {
        cout << "\n\n\t\t\tInsufficient balance in the source account.";
        return;
    }

    from_node->ac.draw(amount);
    to_node->ac.dep(amount);

    log_transaction(from_ac, "Transfer Out", amount, from_node->ac.retdeposit());
    log_transaction(to_ac, "Transfer In", amount, to_node->ac.retdeposit());

    cout << "\n\n\t\t\tTransfer completed successfully.";

    ofstream out("accounts.txt");
    node *current = head;
    while (current) {
        current->ac.save_to_file(out);
        current = current->next;
    }
}

void display_transactions() {
    cout << "\nTransaction Log:\n";
    while (!transactions.empty()) {
        cout << transactions.front() << endl;
        transactions.pop();
    }
}

void bubble_sort_accounts() {
    if (!head || !head->next) return;

    bool swapped;
    do {
        swapped = false;
        node *current = head;
        node *prev = nullptr;

        while (current && current->next) {
            if (current->ac.retacno() < current->next->ac.retacno()) {
                swapped = true;

                if (prev) {
                    node *temp = current->next->next;
                    prev->next = current->next;
                    current->next->next = current;
                    current->next = temp;
                } else {
                    node *temp = current->next->next;
                    head = current->next;
                    current->next->next = current;
                    current->next = temp;
                }
            }
            prev = current;
            current = current->next;
        }
    } while (swapped);
}

void display_all() {
    bubble_sort_accounts();
    node *temp = head;
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n " ;
    cout << "A/c No.      NAME             Type    Balance\n";
    cout << "====================================================\n";
    while (temp) {
        temp->ac.report();
        temp = temp->next;
    }
}

int main() {
    char ch;
    int num;

    load_accounts();

    do {
        system("CLS");
        cout << "\n\n\t\t\tBANK MANAGEMENT SYSTEM\n";
        cout << "\n\t\t\t1. NEW ACCOUNT";
        cout << "\n\t\t\t2. DEPOSIT AMOUNT";
        cout << "\n\t\t\t3. WITHDRAW AMOUNT";
        cout << "\n\t\t\t4. BALANCE ENQUIRY";
        cout << "\n\t\t\t5. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t\t\t6. CLOSE AN ACCOUNT";
        cout << "\n\t\t\t7. MODIFY AN ACCOUNT";
        cout << "\n\t\t\t8. DISPLAY TRANSACTIONS";
        cout << "\n\t\t\t9. TRANSFER AMOUNT";
        cout << "\n\t\t\t0. EXIT";
        cout << "\n\n\t\t\tSelect Your Option (0-9): ";
        cin >> ch;

        switch (ch) {
        case '1':
            write_account();
            break;
        case '2':
            cout << "\n\t\t\tEnter Account No.: ";
            cin >> num;
            deposit_withdraw(num, 1);
            break;
        case '3':
            cout << "\n\t\t\tEnter Account No.: ";
            cin >> num;
            deposit_withdraw(num, 2);
            break;
        case '4':
            cout << "\n\t\t\tEnter Account No.: ";
            cin >> num;
            display_sp(num);
            break;
        case '5':
            display_all();
            break;
        case '6':
            cout << "\n\t\t\tEnter Account No.: ";
            cin >> num;
            delete_account(num);
            break;
        case '7':
            cout << "\n\t\t\tEnter Account No.: ";
            cin >> num;
            modify_account(num);
            break;
        case '8':
            display_transactions();
            break;
        case '9':
            transfer_amount();
            break;
        case '0':
            cout << "\n\n\t\t\tThanks for using the bank management system.";
            break;
        default:
            cout << "\n\n\t\t\tInvalid option. Please try again.";
        }
        cin.ignore();
        cin.get();
    } while (ch != '0');

    return 0;
}

