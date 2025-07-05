#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include <cctype> // For tolower()
using namespace std;

class InventorySystem {
private:
    string item_id, item_name;
    int item_quantity, item_price;
    fstream file, file1;
    int total_Amount = 0;
    string search;
    char _choice;

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int getPositiveInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail() || value < 0) {
                cout << "Invalid input. Please enter a positive number.\n";
                clearInputBuffer();
            } else {
                clearInputBuffer();
                return value;
            }
        }
    }

public:
    void add_Product();
    void view_Product();
    void buy_Product();
    void showMainMenu();
};

int main() {
    InventorySystem inventory;
    inventory.showMainMenu();
    return 0;
}

void InventorySystem::showMainMenu() {
    char choice;
    
    while (true) {
        cout << "\n===== Inventory Management System =====\n";
        cout << "1. Add Product\n";
        cout << "2. View Products\n";
        cout << "3. Buy Products\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case '1':
                add_Product();
                break;
            case '2':
                view_Product();
                break;
            case '3':
                buy_Product();
                break;
            case '0':
                cout << "Exiting system. Goodbye!\n";
                return;
            default:
                cout << "Invalid selection. Please try again.\n";
                break;
        }
    }
}

void InventorySystem::add_Product() {
    cout << "\n===== Add New Product =====\n";
    cout << "Enter Product ID: ";
    getline(cin, item_id);
    cout << "Enter Product Name: ";
    getline(cin, item_name);
    item_quantity = getPositiveInput("Enter Product Quantity: ");
    item_price = getPositiveInput("Enter Product Price: ");

    file.open("data.txt", ios::out | ios::app);
    if (!file) {
        cerr << "Error opening file!\n";
        return;
    }
    file << item_id << "\t" << item_name << "\t" << item_quantity << "\t" << item_price << endl;
    file.close();
    
    cout << "Product added successfully!\n";
}

void InventorySystem::view_Product() {
    cout << "\n===== Product Inventory =====\n";
    file.open("data.txt", ios::in);
    if (!file) {
        cout << "No products found. Inventory is empty.\n";
        return;
    }

    cout << "________________________________________________________\n";
    cout << left << setw(15) << "Product ID" << setw(20) << "Product Name" 
         << setw(10) << "Quantity" << setw(15) << "Price" << endl;
    cout << "________________________________________________________\n";

    while (file >> item_id >> item_name >> item_quantity >> item_price) {
        cout << left << setw(15) << item_id << setw(20) << item_name 
             << setw(10) << item_quantity << "$" << setw(14) << item_price << endl;
    }
    cout << "________________________________________________________\n";
    file.close();
}

void InventorySystem::buy_Product() {
    total_Amount = 0; // Reset total amount for new purchase
    _choice = 'y';
    int quantity = 0; // Declare quantity variable here

    while (tolower(_choice) == 'y') {
        view_Product(); // Show available products first

        cout << "\nEnter Product ID you want to buy: ";
        getline(cin, search);
        quantity = getPositiveInput("Enter Quantity: ");

        bool productFound = false;
        file.open("data.txt", ios::in);
        file1.open("temp.txt", ios::out);

        if (!file || !file1) {
            cerr << "Error opening files!\n";
            return;
        }

        while (file >> item_id >> item_name >> item_quantity >> item_price) {
            if (item_id == search) {
                productFound = true;
                if (quantity > item_quantity) {
                    cout << "Not enough stock available! Only " << item_quantity << " left.\n";
                    file1 << item_id << "\t" << item_name << "\t" << item_quantity << "\t" << item_price << endl;
                } else {
                    item_quantity -= quantity;
                    file1 << item_id << "\t" << item_name << "\t" << item_quantity << "\t" << item_price << endl;

                    int item_total = quantity * item_price;
                    total_Amount += item_total;
                    
                    cout << "\nAdded to cart: " << quantity << " x " << item_name << " @ $" << item_price << endl;
                    cout << "Item total: $" << item_total << endl;
                }
            } else {
                file1 << item_id << "\t" << item_name << "\t" << item_quantity << "\t" << item_price << endl;
            }
        }

        file.close();
        file1.close();

        if (!productFound) {
            cout << "Product not found!\n";
            remove("temp.txt"); // Delete the temporary file
        } else {
            remove("data.txt");
            rename("temp.txt", "data.txt");
        }

        cout << "\nCurrent total: $" << total_Amount << endl;
        cout << "Continue shopping? (Y/N): ";
        cin >> _choice;
        clearInputBuffer();
    }

    if (total_Amount > 0) {
        cout << "\n===== Final Receipt =====\n";
        cout << "Total Purchase Amount: $" << total_Amount << endl;
        cout << "Thank you for your purchase!\n";
        cout << "=========================\n";
    }
}