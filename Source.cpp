
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
using namespace std;

//Function protoypes
void showMenu();
void adminMenu();
void adminSubMenu(char choice);
void productFile();
void inventoryControl();
void customerOrder();
string invoice();
void appendInvoice(int pNO, string name, int quantity, double price, double sum, string file);


int main()
{
    char choice = 0;

    //Inital display of menu for user input
    showMenu();
    cout << "Please select your Option <1-3>: ";
    cin >> choice;

    //Loop until input of the character '3' or '4' to terminate the loop
    //the decisions are shown in the menu and admin submenu
    while (choice != '3' && choice != '4') {
      cin.clear();
      cout << endl;
      switch (choice) {
      case '1'://customer purchasing product and showing invoice after
            customerOrder();
            choice = '3';
           break;
      case '2'://ADMIN SUB MENU
            adminMenu();
            cin >> choice;

            adminSubMenu(choice);
            break;
       case '3': // EXIT PROGRAM
            exit(0);
        }
    }
    return 0;
}

//Function to display the main menu
void showMenu() {
    cout << "MAIN MENU\n\n";

    cout << "01. CUSTOMER" << endl << "02. ADMINISTRATOR" << endl << "03. EXIT" << endl << endl;
}
//Function to display admin menu
void adminMenu() {
    cout << "\nADMIN SUB-MENU\n\n";

    cout << "01. INVENTORY CONTROL" << endl << "02. PRODUCT'S LIST" << endl << "03. RETURN TO MENU"
        << endl << "04. EXIT PROGRAM" << endl << endl;

    cout << "Please select your option <1-4> ";
}
//Function for admin submenu input
//Displays the option to view the inventorycontrol file, product list file, go back to menu or exit program entirely
void adminSubMenu(char choice) {
    switch (choice) {
    case '1': //inventory control function - read and display inventory control file
        inventoryControl();
        break;
    case '2': //display product list file function
        productFile();
        break;
    case '3': showMenu(); //Display main menu
        cin >> choice;
        break;
    case '4': //EXIT PROGRAM
        exit(0);
    }
}
//Function that reads and displays the content of product.txt file
void productFile() {
    fstream productList("product.txt");
    
    if (productList.is_open()) {
        cout << productList.rdbuf();
    }
    productList.close();
}
//Function that reads and displays the contents of the inventoryc.txt file
void inventoryControl() {
    fstream inventoryc("inventoryc.txt");

    if (inventoryc.is_open()) {
        cout << inventoryc.rdbuf();
    }
    inventoryc.close();
}
//Function to complete customer's order and produce invoice
void customerOrder() {
    //MultiMap implementation
    multimap<int, pair<double, string>> numberPrice;
    multimap<int, pair<double, string>>::iterator it;

    //keys and values of products
    numberPrice.insert(make_pair(001, make_pair(1.50, "TOOTHBRUSH")));
    numberPrice.insert(make_pair(002, make_pair(2.00, "SHAMPOO")));
    numberPrice.insert(make_pair(003, make_pair(2.50, "SOAP")));
    numberPrice.insert(make_pair(004, make_pair(4.00, "LOOFA")));
    
    char decision = 0;
    int pNO = 0;
    int quantity = 0;
    double price = 0.00;
    double total = 0.00;
    double sum = 0.00;
    string itemName = " ";

    //invoice function returns a string of the filename to be used later of the invoice 
    //that's been newly created at the beginning of the customer Order window
    string invoiceCreation = invoice();


    //display contents of product.txt file
    cout << endl;
    productFile();
    cout << endl << endl;

    do {
        cout << "\nPlease enter product number of item you wish to purchase: \n";
        cin >> pNO;

        //search if product number exists
        while (numberPrice.find(pNO) == numberPrice.end()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "That product number does not exist\nPlease input a correct product number: ";
            cin >> pNO;
        }
        //gets input of quantity variable while checking for succesful input of an integer
        do {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter the quantity you wish to order: \n";
            cin >> quantity;
        } while (cin.fail());


        //find the matching key to the pNO and calculate the total price from the value * quantity
        for (it = numberPrice.begin(); it != numberPrice.end(); it++) {
            if ((*it).first == pNO) {
                sum = (it->first * quantity);
                total += sum;
                itemName = it->second.second;
                price = it->second.first;
                break;
            }
        }
        
        cout << "\nWould you like to order another product?\n01.YES\n02.EXIT\n";
        cin >> decision;

        appendInvoice(pNO, itemName, quantity, price, sum, invoiceCreation);

    } while (decision != '2');

    fstream file(invoiceCreation, ios::app);
    
    if (file.is_open()) {
        file << fixed << setprecision(2);
        file << setw(57) << right << "Total: $" << total << endl;
    }

    file.close();


    

    //TO-DO:
    //append to inventorycontrol


}

//function to generate a new invoice for the current order
string invoice() {
    int id = 0;
    const int WIDTH = 16;
    string invoiceName = "INVOICE_";
    string invoice = invoiceName + to_string(id) + ".txt";
    

    while (fstream(invoice, ios::in)) {
        id++;
        invoice = invoiceName + to_string(id) + ".txt";
    }
    
    fstream file_create(invoice, ios::out);

    if (file_create.good()) {
        
        if (file_create.is_open()) {
            file_create << "===============================================================" << endl;
            file_create << setw(WIDTH * 2) << "  INVOICE" << endl
                << "===============================================================" << endl;
            file_create << setw(WIDTH - 5) << left << "PNO"
                << setw(WIDTH - 3) << left << "ITEM"
                << setw(WIDTH) << left << "QUANTITY"
                << setw(WIDTH) << left << "PRICE"
                << setw(WIDTH - 2) << left << "AMOUNT" << endl;
            file_create << "===============================================================" << endl;
        }

        //cout << "Invoice successfully created!\n"; Test to see if successfully created file
    }
    else {
        cout << "ERROR! Invoice not created!!\n";
    }

    file_create.close();

    return invoice;
}
//function to write to the invoice file that's been created for the current order
void appendInvoice(int pNO, string name, int quantity, double price, double sum, string file) {

    fstream textFile(file, ios::app);

    if (textFile.is_open()) {
        const int WIDTH = 16;

        textFile << fixed << setprecision(2);

        textFile << setw(WIDTH - 5) << left << pNO
            << setw(WIDTH - 3) << left << name
            << setw(WIDTH) << left << quantity
            << setw(WIDTH) << left << price
            <<  "$" << sum << endl;

        //cout << "data success append to file\n"; Test to see if successfully created file
    }
    else {
        cout << "ERROR\n";
    }

    textFile.close();
}