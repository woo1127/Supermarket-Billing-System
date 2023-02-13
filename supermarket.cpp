#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include "libraries/rapidjson/document.h"
#include "libraries/rapidjson/ostreamwrapper.h"
#include "libraries/rapidjson/istreamwrapper.h"
#include "libraries/rapidjson/prettywriter.h"
#include "libraries/color.hpp"

#define CART_FILE_PATH "data/cart.json"
#define CREDENTIALS_FILE_PATH "data/credentials.csv"
#define WIDTH 62

using namespace std;
using namespace rapidjson;

template<typename T>
const auto func1 = dye::bright_white_on_blue <T>;
const auto bg_blue = func1<string>;

template<typename T>
const auto func2 = dye::black_on_bright_white<T>;
const auto bg_white = func2<string>;

template<typename T>
const auto func3 = dye::bright_white_on_red<T>;
const auto bg_red = func3<string>;

template<typename T>
const auto func4 = dye::bright_white_on_green<T>;
const auto bg_green = func4<string>;

template<typename T>
const auto func5 = dye::vanilla<T>;
const auto bg_default = func5<string>;

struct ProductInfo {
    int productId;
    string productName;
    int productQty;
    double productPrice;
};

struct LoginInfo {
    string userid;
    string username;
    string password;
} loginInfo;

// Pages
void welcomePage();
void loginPage();
void signupPage();
void mainPage();
void menuPage();
void accountPage();
void cartPage();
void productPage(string);
void paymentPage();
void receiptPage();

// Helper function
Document readJsonFile(string);
void writeJsonFile(Document &, string);
int jsonFindUserPosition(Value &);
int jsonCreateNewUser(Value &, Document::AllocatorType &);
int charToInt(char);
void lineDivider(char);
void margin();
void countSpaceBetween(int, int, int*, int*, int);


/** 
 * @brief  Display the text in the center of the interface
 * @param  text    The text used to display on center
 * @param  color   The text and background color of the centered text
 * @param  border  Decide whether to show the border of the left and right side of the program
 * @param  width   The width of the interface
 */
template <typename T>
void printCenter(string text, T (*color)(string), bool border = false, int width = WIDTH + 1) {
    int front = 0;
    int back = 0;

    margin();

    if (border) {
        front = (width - 2) / 2.0 + ceil(text.length() / 2.0);
        back = width - front;
        cout << '|' << setw(front) << right << color(text) << setw(back - 2) << color(" ") << "|\n";
    } else {
        front = width / 2.0 + ceil(text.length() / 2.0);
        back = width - front;
        cout << setw(front) << right << color(text) << setw(back) << color(" ") << '\n';
    }
}

/**
 * @brief  Provide a specify left margin for the interface
 *         so it will not stick on the left side of console
 */
void margin() {
    cout << string(30, ' ');
}

/**
 * @brief  Display a separate line based on the width of interface
 * @param  symbol  The symbol or character used to draw the line
 */
void lineDivider(char symbol) {
    margin();
    cout << string(WIDTH + 1, symbol) << '\n';
}

int main() {
    welcomePage();
    
    system("exit");
}

void welcomePage() {
    int option = 0;

    cout << '\n';
    lineDivider('=');
    printCenter("Welcome to Tesco Supermarket", bg_blue);
    lineDivider('=');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     1. Login"  << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     2. Signup" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;
    
    system("cls||clear");

    switch (option) {
        case 1: 
            loginPage(); 
            break;
        case 2: 
            signupPage(); 
            break;
        default: 
            printCenter("Invalid option", bg_red);
            welcomePage(); 
            break;
    }
}

void loginPage() {
    string storeUserid;
    string storeUsername;
    string storePassword;
    string username;
    string password;
    string line, word;

    cout << '\n';
    lineDivider('=');
    printCenter("Login Page", bg_blue);
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "     Username : ";
    cin  >> username;
    margin(); 
    cout << "     Password : ";
    cin  >> password;

    fstream file(CREDENTIALS_FILE_PATH, ios::in);

    if (file.is_open()) {
        getline(file, line);

        while (getline(file, line)) {
            stringstream str(line);

            getline(str, storeUserid, ',');
            getline(str, storeUsername, ',');
            getline(str, storePassword, ',');

            if (storeUsername == username && storePassword == password) {
                loginInfo = {storeUserid, storeUsername, storePassword};

                system("cls||clear");
                printCenter("Login Successfully!", bg_green);
                mainPage();
            }
        }
        file.close();
    }

    system("cls||clear");
    printCenter("Invalid userid or password, please try again.", bg_red);
    welcomePage();
}

void signupPage() {
    int numOfLine = 0;
    string newUserid;
    string newUsername;
    string newPassword;
    string line;

    cout << '\n';
    lineDivider('=');
    printCenter("Signup Page", bg_blue);
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "     New username : ";
    cin  >> newUsername;
    margin(); 
    cout << "     New password : ";
    cin  >> newPassword;

    fstream file(CREDENTIALS_FILE_PATH, ios::in);

    // get the number of line in the credentials txt file
    // as the new id for new user
    if (file.is_open()) {
        while (getline(file, line)) {
            numOfLine++;
        }
        file.close();
    }

    // open and append the new signup user information 
    // to the credentials txt file
    fstream file2(CREDENTIALS_FILE_PATH, ios::app);
    newUserid = to_string(numOfLine);

    file2 << "\n" 
          << newUserid << ','
          << newUsername << ','
          << newPassword;
    file2.close();

    loginInfo = {newUserid, newUsername, newPassword};

    system("cls||clear");
    printCenter("Account set up successfully", bg_green);
    mainPage();
}

void mainPage() {
    int option = 0;

    /*  
    ======================================
                Main Page              
    ======================================
                        
         1. Menu                   
         2. Account                    
         3. Cart
         4. Logout                        
    
    ======================================
    
       Enter your choice:
    */

    cout << '\n';
    lineDivider('=');
    printCenter("Main Page", bg_blue);
    lineDivider('=');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     1. Menu"    << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     2. Account" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     3. Cart"    << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     4. Logout"  << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    system("cls||clear");

    switch (option) {
        case 1: 
            menuPage(); 
            break;
        case 2: 
            accountPage(); 
            break;
        case 3: 
            cartPage();
            break;
        case 4: 
            welcomePage(); 
            break;
        default: 
            printCenter("Invalid option", bg_red);
            mainPage(); 
            break;
    }
}

void menuPage() {
    char option;

    /*  
    ======================================
                Menu Page              
    ======================================
    
       Categories:                        
         1. Canned Food                   
         2. Vegetables                    
         3. Fruits                        
    
    --------------------------------------
       Press (b): Back to previos page    
       Press (p): Proceed to checkout     
    ======================================
    
       Enter your choice:
    */

    cout << '\n';
    lineDivider('=');
    printCenter("Menu Page", bg_blue);
    lineDivider('=');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|   Categories:"      << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     1. Canned Food" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     2. Vegetables"  << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     3. Fruits"      << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|   Press (b): Back to previos page" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|   Press (p): Proceed to checkout"  << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    system("cls||clear");

    switch (option) {
        case '1': 
            productPage("data/canned_food.csv"); 
            break;
        case '2': 
            productPage("data/vegetables.csv"); 
            break;
        case '3': 
            productPage("data/fruits.csv"); 
            break;
        case 'b': 
            mainPage(); 
            break;
        case 'p': 
            cartPage(); 
            break;
        default: 
            printCenter("Invalid option", bg_red);
            break;
    }
}

/**
 * @brief  View information of current logged in user. 
 *         Support to change information.
 */
void accountPage() {
    char option;
    string line, word;
    string newUserid;
    string newPassword;
    string tempCredentials = "";

    /*
    ======================================
                 Account Page             
    ======================================

         Userid   : loginInfo.userid
         Password : loginInfo.password 

    --------------------------------------
       Press (b): Back to previous page
       Press (p): Change account info
    ======================================

       Enter your choices:
    */

    cout << '\n';
    lineDivider('=');
    printCenter("Account Page", bg_blue);
    lineDivider('=');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(17) << "|     Userid   : " << setw(WIDTH - 17) << loginInfo.username << "|\n";
    margin(); 
    cout << setw(17) << "|     Password : " << setw(WIDTH - 17) << loginInfo.password << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|     Press (p): Change account info"   << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     Press (b): Back to previous page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    fstream fileIn(CREDENTIALS_FILE_PATH, ios::in);
        
    if (option == 'p') {
        margin();
        cout << "   Enter new userid   : ";
        cin >> newUserid;
        margin();
        cout << "   Enter new password : ";
        cin >> newPassword;

        // iterate every line in file
        while (getline(fileIn, line)) {
            // split the line into multiple words separated by comma
            stringstream str(line);
            getline(str, word, ',');

            if (word != loginInfo.userid) {
                tempCredentials += line + '\n';
            }
            else if (word == loginInfo.userid) {
                tempCredentials += loginInfo.userid + ',' + newUserid + ',' + newPassword + '\n';

                loginInfo.userid = newUserid;
                loginInfo.password = newPassword;
            }

            for (size_t i = 0; i < 2; i++) 
                getline(str, word, ',');
        }

        fileIn.close();

        fstream fileOut(CREDENTIALS_FILE_PATH, ios::out);
        fileOut << tempCredentials;
        fileOut.close();

        system("cls||clear");
        printCenter("Account info changed successfully", bg_green);
        accountPage();
    } 
    else if (option == 'b') {
        system("cls||clear");
        mainPage();
    } 
    else {
        system("cls||clear");
        printCenter("Invalid option", bg_red);
        accountPage();
    }
}

/**
 * @brief  Count the blank space that should be used as the 
 *         padding between columns
 * @param  usedSpace  The space that have been filled with characters
 * @param  intervals  The number of intervals
 * @param  averageSpace  The average blank space between each columns
 * @param  lastSpace  The blank space of last column
 * @param  width  The width of our program measured by number of characters
 */
void countSpaceBetween(int usedSpace, int intervals, int* averageSpace, int* lastSpace, int width = WIDTH) {
    int totalSpace = width - usedSpace - 2;
    int avgSpace = totalSpace / intervals;

    int leftSpace = totalSpace - avgSpace * intervals;
    int finalSpace = avgSpace + leftSpace + 1;

    *averageSpace = avgSpace;
    *lastSpace = finalSpace;
}

/**
 * @brief  The interface of cart page.
 *         User is allow to view and decide whether to
 *         remove product from cart or proceed to checkout.
 */
void cartPage() {
    char option; 
    double totalPrice = 0.0;
    int averageSpace = 0;
    int lastSpace = 0;
    int usedSpace = 39;
    int interval = 6;

    countSpaceBetween(usedSpace, interval, &averageSpace, &lastSpace);

    /*
    ======================================
                  Cart Page               
    ======================================
      No. Item       Qty    Price   Amount
    --------------------------------------
       
       1. Food Name    2     4.20    8.40
       2. Food Name    5     5.40   27.00
       
    --------------------------------------
       Total Amount :               35.40
    --------------------------------------
       Press (p): Proceed to checkout
       Press (b): Back to previos page
    ======================================

       Enter number to unselect product:
    */

   cout << '\n';
   lineDivider('=');
   printCenter("Cart Page", bg_blue);
   lineDivider('=');
   margin(); 
   cout << "|" 
        << string(averageSpace, ' ') << setw(3)  << left  << "No."
        << string(averageSpace, ' ') << setw(20) << left  << "Item"
        << string(averageSpace, ' ') << setw(4)  << right << "Qty"
        << string(averageSpace, ' ') << setw(6)  << right << "Price"
        << string(averageSpace, ' ') << setw(6)  << right << "Amount"
        << string(lastSpace, ' ')    << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";

    Document doc = readJsonFile(CART_FILE_PATH);
    Document::AllocatorType & allocator = doc.GetAllocator();
    
    // Get the value of "users" key
    Value & users = doc["users"];
    int userPosition = jsonFindUserPosition(users);

    // if the username is not exist in the file, create and add a new one
    if (userPosition == -1) {
        userPosition = jsonCreateNewUser(users, allocator);
    }

    // Get the value of "cart" key
    Value & cart = users[userPosition]["cart"];

    // loop and display the products from cart array
    for (const auto & p: cart.GetArray()) {
        margin(); 
        cout << "|";

        cout << string(averageSpace, ' ') << setw(3)  << left  << p["id"].GetInt();
        cout << string(averageSpace, ' ') << setw(20) << left  << p["productName"].GetString();
        cout << string(averageSpace, ' ') << setw(4)  << right << p["quantity"].GetInt();
        cout << string(averageSpace, ' ') << setw(6)  << right << fixed 
                                          << setprecision(2) << p["productPrice"].GetDouble();
        cout << string(averageSpace, ' ') << setw(6)  << right << fixed 
                                          << setprecision(2) << p["amount"].GetDouble();
        cout << string(lastSpace, ' ')    << "|\n";

        totalPrice += p["amount"].GetDouble();
    }

    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(16) << left << "|   Total Amount"
         << setw(WIDTH - 16 - lastSpace) << right << totalPrice
         << string(lastSpace, ' ') << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|   Press (p): Proceed to checkout"   << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|   Press (b): Back to previous page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    system("cls||clear");

    if (option == 'p') {
        paymentPage();
    }
    else if (option == 'b') {
        mainPage();
    }
    else if (charToInt(option) > 0 && charToInt(option) <= cart.Size()) {
        // remove selected product from cart
        cart.Erase(cart.Begin() + charToInt(option) - 1);

        // reorder the id of each product in cart numerically
        for (SizeType i = 0; i < cart.Size(); i++) {
            cart[i]["id"] = i + 1;
        }
        writeJsonFile(doc, CART_FILE_PATH);

        printCenter("Item has been removed", bg_green);
        cartPage();
    }
    else {
        printCenter("Invalid option", bg_red);
        cartPage();
    }
}

/**
 * @brief  The interface of product page, the page will display 
 *         different category of products based on the given filepath.
 *         User can add product to cart by input option
 * @param  filepath  The path of the file that store the data 
 *                   of particular category
 */
void productPage(string filepath) {
    char option;
    string line, word;
    int selectedProductId = 0;
    int selectedProductQty = 0;
    vector<ProductInfo> products;

    int averageSpace = 0;
    int lastSpace = 0;
    int usedSpace = 33;
    int interval = 5;

    countSpaceBetween(usedSpace, interval, &averageSpace, &lastSpace);

    fstream file(filepath, ios::in);
    getline(file, line);
    
    /*
    ======================================
                Product Page             
    ======================================
        No.  Item          Qty    Price
    --------------------------------------
       
        1.   Food Name     10     4.20
        2.   Food Name      5     5.40
       
    --------------------------------------
       Press (b): Back to previos page
    ======================================

       Enter your choices:
    */

    cout << '\n';
    lineDivider('=');
    printCenter("Product Page", bg_blue);
    lineDivider('=');
    margin(); 
    cout << "|" 
         << string(averageSpace, ' ') << setw(3)  << left  << "No."
         << string(averageSpace, ' ') << setw(20) << left  << "Item"
         << string(averageSpace, ' ') << setw(4)  << right << "Qty"
         << string(averageSpace, ' ') << setw(6)  << right << "Price"
         << string(lastSpace, ' ')    << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream str(line);
            ProductInfo p;

            margin(); cout << "|"; 

            getline(str, word, ',');
            cout << string(averageSpace, ' ') << setw(3)  << left << word;
            p.productId = stoi(word);

            getline(str, word, ',');
            cout << string(averageSpace, ' ') << setw(20) << left  << word;
            p.productName = word;

            getline(str, word, ',');
            cout << string(averageSpace, ' ') << setw(4)  << right << word;
            p.productQty = stoi(word);

            getline(str, word, ',');
            cout << string(averageSpace, ' ') << setw(6)  << right << word;
            p.productPrice = stod(word);

            cout << string(lastSpace, ' ') << "|\n";
            products.push_back(p);
        }
        file.close();
    }

    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|   Press (b): Back to previous page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    selectedProductId = charToInt(option);

    if (option == 'b') {
        system("cls||clear"); 
        menuPage();
    }
    else if (selectedProductId > 0 && selectedProductId <= products.size()){
        margin();
        cout << "   Enter the quantity: ";
        cin >> selectedProductQty;
        cout << "\n";

        ProductInfo selectedProduct = products[selectedProductId - 1];

        Document doc = readJsonFile(CART_FILE_PATH);
        Document::AllocatorType & allocator = doc.GetAllocator();

        Value & users = doc["users"];
        Value productName(selectedProduct.productName.c_str(), allocator);

        int userPosition = jsonFindUserPosition(users);
        
        // if user info not found in json, then create a new user in json
        if (userPosition == -1) {
            userPosition = jsonCreateNewUser(users, allocator);
        }

        Value & cart = users[userPosition]["cart"];
        Value newProduct(kObjectType);

        // add the information of selected product in a key value pair
        newProduct.AddMember("id", cart.Size() + 1, allocator);
        newProduct.AddMember("productName", productName, allocator);
        newProduct.AddMember("quantity", selectedProductQty, allocator);
        newProduct.AddMember("productPrice", selectedProduct.productPrice, allocator);
        newProduct.AddMember("amount", selectedProduct.productPrice * selectedProductQty, allocator);

        // append the product details into the cart array of logged user
        cart.PushBack(newProduct, allocator);

        writeJsonFile(doc, CART_FILE_PATH);
    }
    else {
        system("cls||clear");
        printCenter("Invalid Option", bg_red);
        productPage(filepath);
    }

    margin();
    cout << "   Item has been added to cart? Continue to add? [y/n]: ";
    cin  >> option;

    if (option == 'y') {
        system("cls||clear");
        productPage(filepath);
    } 
    else if (option == 'n') {
        system("cls||clear");
        menuPage();
    } 
    else {
        margin();
        cout << "   Invalid option" << "\n\n";
        margin();
        cout << "   Item has been added to cart? Continue to add? [y/n]: ";
    }
}

void paymentPage() {
    char option;

    /*  
    ======================================
                Payment Page              
    ======================================

       Choose payment method:
         1. Credit Card
         2. Online Banking
       
    ======================================
    
       Enter your choice:
    */

    cout << '\n';
    lineDivider('=');
    printCenter("Payment Page", bg_blue);
    lineDivider('=');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     1. Credit Card"    << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     2. Online Banking" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|   Press (b): Back to main page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    system("cls||clear");

    if (option == '1' || option == '2') {
        printCenter("Payment Successful", bg_green);
        receiptPage();
    }
    else if (option = 'b') 
        mainPage();
    else {
        printCenter("Invalid option", bg_red);
        paymentPage();
    }
}

/**
 * @brief  The design of the supermarket's receipts.
 *         The receipt is print after the user make payment.
 */
void receiptPage() {
    double totalPrice = 0.0;
    int averageSpace = 0;
    int lastSpace = 0;
    int usedSpace = 30;
    int intervals = 4;
    char datetime1[50];
    string datetime2;

    // get the currect date and time
    time_t t = time(0);
    // format the date and time into specify format, in this case 
    // is dd/mm/yyyy and time in 12-hour clock
    strftime(datetime1, 50, "%d / %m / %Y   %I:%M %p", localtime(&t));
    // convert char array to string
    datetime2 = datetime1;

    countSpaceBetween(usedSpace, intervals, &averageSpace, &lastSpace);

    /* 
    ======================================
                  Receipt
    ======================================
                   TESCO
               TEL: 043456789
    --------------------------------------
       Qty   Item                 Amount
    --------------------------------------

        3    Food one             20.20      
        1    Food two              4.50

    --------------------------------------
       Total:                      24.60
    --------------------------------------
                THANK YOU!

            11/2/2023 08:22 pm
    ======================================
    */

    cout << '\n';
    lineDivider('=');
    printCenter("Receipt", bg_blue);
    lineDivider('=');
    printCenter("Tesco", bg_default, true);
    printCenter("TEL: 043456789", bg_default, true);
    lineDivider('-');
    margin(); 
    cout << "|" 
         << string(averageSpace, ' ') << setw(4)  << right << "Qty"
         << string(averageSpace, ' ') << setw(20) << left  << "Item"
         << string(averageSpace, ' ') << setw(6)  << right << "Amount"
         << string(lastSpace, ' ')    << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";

    Document doc = readJsonFile(CART_FILE_PATH);
    
    Value & users = doc["users"];
    int userPosition = jsonFindUserPosition(users);
    Value & cart = users[userPosition]["cart"];

    // iterate and print the products information store in cart
    for (const auto & p: cart.GetArray()) {
        margin(); 
        cout << "|";
        cout << string(averageSpace, ' ') << setw(4)  << right << p["quantity"].GetInt();
        cout << string(averageSpace, ' ') << setw(20) << left  << p["productName"].GetString();                                  
        cout << string(averageSpace, ' ') << setw(6)  << right << fixed 
                                          << setprecision(2) << p["amount"].GetDouble();
        cout << string(lastSpace, ' ')    << "|\n";

        totalPrice += p["amount"].GetDouble();
    }

    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin();
    cout << setw(16) << left << "|   Total Amount"
         << setw(WIDTH - 16 - lastSpace) << right << totalPrice
         << string(lastSpace, ' ') << "|\n";
    lineDivider('-');
    printCenter("THANK YOU!", bg_default, true);
    margin();
    cout << setw(WIDTH) << left << "|" << "|\n";
    printCenter(datetime2, bg_default, true);
    lineDivider('=');
    cout << '\n';
    margin();
    cout << "   Press any key to back to main page: ";
    getch();

    // erase all products in user's cart since payment has make
    cart.Erase(cart.Begin(), cart.Begin() + cart.Size());
    writeJsonFile(doc, CART_FILE_PATH);
   
    system("cls||clear");
    mainPage();
}

/**
 * @brief  Read a json file and parse it into a DOM(document object model)
 * @param  readPath  The location of a json file
 * @return  The DOM of a json object
 */ 
Document readJsonFile(string readPath) {
    fstream file(readPath, ios::in);
    IStreamWrapper isw(file);

    Document doc;
    doc.ParseStream(isw);

    return doc;
}

/**
 * @brief  Write and save a json file
 * @param  doc  The DOM of a json object
 * @param  savePath  The save location of a json file
 */
void writeJsonFile(Document& doc, string savePath) {
    fstream file(savePath, ios::out);
    OStreamWrapper osw(file);

    PrettyWriter<OStreamWrapper> writer(osw);
    doc.Accept(writer);
}

/** 
 * @brief  Find and return the position of logged user in json file
 * @param  users  The "users" array in json file
 * @param  allocator  The allocator of DOM
 */
int jsonFindUserPosition(Value & users) {
    int position = -1;
    
    // loop and check if the value of "userid" key match with the value of userid in
    // loginInfo structure
    for (SizeType i = 0; i < users.Size(); i++) {
        if (users[i]["username"].GetString() == loginInfo.username) {
            position = i;
        } 
    }

    // return -1 if user not found
    return (position != -1) ? position : -1;
}

/**
 * @brief  Create and append the info of new user into the json file
 * @param  users  The "users" array in json file
 * @param  allocator  The allocator of DOM
 * @return  The position of the new user in json file
 */
int jsonCreateNewUser(Value & users, Document::AllocatorType & allocator) {
    int position = 0;

    Value newCart(kArrayType);
    Value newUsersObject(kObjectType);
    Value username(loginInfo.username.c_str(), allocator) ;

    newUsersObject.AddMember("username", username, allocator);
    newUsersObject.AddMember("cart", newCart, allocator);

    position = users.Size();
    users.PushBack(newUsersObject, allocator);

    return position;
}

int charToInt(char num) {
    // if '0' == 48 in ascii, minus 48 will get 0 that become the integer of '0'
    return static_cast<int>(num) - 48;
}
