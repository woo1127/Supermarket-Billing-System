#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
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

struct ProductInfo {
    int productId;
    string productName;
    int productQty;
    double productPrice;
};

struct CartProductInfo {
    int id;
    string productName;
    int quantity;
    double productPrice;
    double amount;
};

struct LoginInfo {
    string userIndex;
    string userid;
    string password;
} loginInfo;

// ********** Pages **********
void welcomePage();
void loginPage();
void signupPage();
void mainPage();
void menuPage();
void accountPage();
void cartPage();
void productPage(string);
void paymentPage();

// ********** Helper function **********
Document readJsonFile(string);
void writeJsonFile(Document &, string);
int jsonFindUserPosition(Value &, Document::AllocatorType &);
int jsonCreateNewUser(Value &, Document::AllocatorType &);
int charToInt(char);
void lineDivider(char);
void margin();
void countSpaceBetween(int, int, int*, int*, int);

template <typename T>
void printCenter(string text, T (*color)(string), int width = WIDTH + 1) {
    int front = width / 2.0 + ceil(text.length() / 2.0);
    int back = width - front;

    margin();
    cout << setw(front) << right << color(text) << setw(static_cast<int>(width - front)) << color(" ") << '\n';
}

void margin() {
    cout << string(30, ' ');
}

void lineDivider(char symbol = '=') {
    cout << string(WIDTH + 1, symbol) << '\n';
}

int main() {
    welcomePage();
    
    system("exit");
}

void welcomePage() {
    int option = 0;

    cout << '\n';
    margin(); lineDivider();
    printCenter("Welcome to Tesco Supermarket", bg_blue);
    margin(); lineDivider();
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     1. Login"  << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     2. Signup" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "   Enter your choice: ";
    cin  >> option;
    
    system("cls");

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
    string storeUserIndex;
    string storeUserid;
    string storePassword;
    string userid;
    string password;
    string line, word;

    cout << '\n';
    margin(); lineDivider();
    printCenter("Login Page", bg_blue);
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "     Userid   : ";
    cin >> userid;
    margin(); cout << "     Password : ";
    cin >> password;

    fstream file(CREDENTIALS_FILE_PATH, ios::in);

    if (file.is_open()) {
        getline(file, line);

        while (getline(file, line)) {
            stringstream str(line);

            getline(str, storeUserIndex, ',');
            getline(str, storeUserid, ',');
            getline(str, storePassword, ',');

            if (storeUserid == userid && storePassword == password) {
                loginInfo.userIndex = storeUserIndex;
                loginInfo.userid = storeUserid;
                loginInfo.password = storePassword;

                system("cls");
                printCenter("Login Successfully!", bg_green);
                mainPage();
            }
        }
        file.close();
    }

    system("cls");
    printCenter("Invalid userid or password, please try again.", bg_red);
    welcomePage();
}

void signupPage() {
    int numOfLine = 0;
    string newUserIndex;
    string line;
    string newUserid;
    string newPassword;

    cout << '\n';
    margin(); lineDivider();
    printCenter("Signup Page", bg_blue);
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "     New userid   : ";
    cin >> newUserid;
    margin(); cout << "     New password : ";
    cin >> newPassword;

    fstream file(CREDENTIALS_FILE_PATH, ios::in);

    if (file.is_open()) {
        while (getline(file, line)) {
            numOfLine++;
        }
        file.close();
    }

    fstream file2(CREDENTIALS_FILE_PATH, ios::app);
    newUserIndex = to_string(numOfLine);

    file2 << "\n" 
          << newUserIndex << ','
          << newUserid << ','
          << newPassword;
    file2.close();

    loginInfo.userIndex = newUserIndex;
    loginInfo.userid = newUserid;
    loginInfo.password = newPassword;

    system("cls");
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
    margin(); lineDivider();
    printCenter("Main Page", bg_blue);
    margin(); lineDivider();
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     1. Menu"    << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     2. Account" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     3. Cart"    << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     4. Logout"  << "|\n";
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "   Enter your choice: ";
    cin  >> option;

    system("cls");

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
    margin(); lineDivider();
    printCenter("Menu Page", bg_blue);
    margin(); lineDivider();
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|   Categories:"      << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     1. Canned Food" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     2. Vegetables"  << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     3. Fruits"      << "|\n";
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(WIDTH) << left << "|   Press (b): Back to previos page" << "|\n";
    margin(); cout << setw(WIDTH) << left << "|   Press (p): Proceed to checkout"  << "|\n";
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "   Enter your choice: ";
    cin  >> option;

    system("cls");

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

void accountPage() {
    int option = 0;
    string line, word;
    string newUserid;
    string newPassword;
    vector<string> tempCredentials;

    /*
    ======================================
                 Account Page             
    ======================================

       Userid   : loginInfo.userid
       Password : loginInfo.password 

    --------------------------------------
       1. Change account info             
       2. Back to previous page           
    ======================================

       Enter your choices:
    */

    cout << '\n';
    margin(); lineDivider();
    printCenter("Account Page", bg_blue);
    margin(); lineDivider();
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); cout << setw(17) << "|     Userid   : " << setw(WIDTH - 17) << loginInfo.userid   << "|\n";
    margin(); cout << setw(17) << "|     Password : " << setw(WIDTH - 17) << loginInfo.password << "|\n";
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(WIDTH) << left << "|     1. Change account info"   << "|\n";
    margin(); cout << setw(WIDTH) << left << "|     2. Back to previous page" << "|\n";
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "   Enter your choice: ";
    cin  >> option;

    fstream fileIn(CREDENTIALS_FILE_PATH, ios::in);

    if (option == 1) {
        cout << "   Enter new userid   : ";
        cin >> newUserid;
        cout << "   Enter new password : ";
        cin >> newPassword;

        while (getline(fileIn, line)) {
            stringstream str(line);
            getline(str, word, ',');

            if (word == loginInfo.userIndex) {                    
                tempCredentials.push_back(loginInfo.userIndex + ',' + 
                                            newUserid + ',' + 
                                            newPassword);
                loginInfo.userid = newUserid;
                loginInfo.password = newPassword;
            } 
            else {
                tempCredentials.push_back(line);
            }

            for (size_t i = 0; i < 2; i++) {
                getline(str, word, ',');
            }
            fileIn.close();
        }

        fstream fileOut(CREDENTIALS_FILE_PATH, ios::out);

        for (auto v: tempCredentials) {
            fileOut << v << '\n';
        }

        tempCredentials.clear();
        fileOut.close();
        system("cls");
        printCenter("Account info changed successfully", bg_green);
        accountPage();
    } 
    else if (option == 2) {
        system("cls");
        mainPage();
    } 
    else {
        system("cls");
        printCenter("Invalid option", bg_red);
        accountPage();
    }
}

void countSpaceBetween(int usedSpace, int interval, int* averageSpace, int* lastSpace, int width = WIDTH) {
    int totalSpace = width - usedSpace - 2;
    int avgSpace = totalSpace / interval;

    int leftSpace = totalSpace - avgSpace * interval;
    int finalSpace = avgSpace + leftSpace + 1;

    *averageSpace = avgSpace;
    *lastSpace = finalSpace;
}

void cartPage() {
    char option; 
    double totalPrice;
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
   margin(); lineDivider();
   printCenter("Cart Page", bg_blue);
   margin(); lineDivider();
   margin(); cout << "|" 
                   << string(averageSpace, ' ') << setw(3)  << left  << "No."
                   << string(averageSpace, ' ') << setw(20) << left  << "Item"
                   << string(averageSpace, ' ') << setw(4)  << right << "Qty"
                   << string(averageSpace, ' ') << setw(6)  << right << "Price"
                   << string(averageSpace, ' ') << setw(6)  << right << "Amount"
                   << string(lastSpace, ' ')    << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";

    Document doc = readJsonFile(CART_FILE_PATH);
    Document::AllocatorType & allocator = doc.GetAllocator();
    
    Value & users = doc["users"];
    int userPosition = jsonFindUserPosition(users, allocator);

    if (userPosition == -1) {
        userPosition = jsonCreateNewUser(users, allocator);
    }

    Value & cart = users[userPosition]["cart"];

    for (auto & p: cart.GetArray()) {
        margin(); cout << "|";

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

    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(16) << left << "|   Total Amount"
                   << setw(WIDTH - 16 - lastSpace) << right << totalPrice
                   << string(lastSpace, ' ') << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(WIDTH) << left << "|   Press (p): Proceed to checkout"   << "|\n";
    margin(); cout << setw(WIDTH) << left << "|   Press (b): Back to previous page" << "|\n";
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "   Enter your choice: ";
    cin >> option;

    system("cls");

    if (option == 'p') {
        paymentPage();
    }
    else if (option == 'b') {
        mainPage();
    }
    else if (charToInt(option) > 0 && charToInt(option) < cart.Size()) {
        cart.Erase(cart.Begin() + charToInt(option) - 1);

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

void productPage(string filepath) {
    char option;
    string line, word;
    int productId = 0;
    int productQty = 0;
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
    margin(); lineDivider();
    printCenter("Product Page", bg_blue);
    margin(); lineDivider();
    margin(); cout << "|" 
                   << string(averageSpace, ' ') << setw(3)  << left  << "No."
                   << string(averageSpace, ' ') << setw(20) << left  << "Item"
                   << string(averageSpace, ' ') << setw(4)  << right << "Qty"
                   << string(averageSpace, ' ') << setw(6)  << right << "Price"
                   << string(lastSpace, ' ')    << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(WIDTH) << left << "|" << "|\n";

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

    margin(); cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); lineDivider('-');
    margin(); cout << setw(WIDTH) << left << "|   Press (b): Back to previous page" << "|\n";
    margin(); lineDivider(); cout << '\n';
    margin(); cout << "   Enter your choice: ";
    cin  >> option;

    if (option == 'b') {
        system("cls"); 
        menuPage();
    }
    else  {
        productId = charToInt(option);

        cout << "   Enter the quantity: ";
        cin >> productQty;
        cout << "\n";

        ProductInfo selectedProduct = products[productId - 1];

        Document doc = readJsonFile(CART_FILE_PATH);
        Document::AllocatorType & allocator = doc.GetAllocator();

        Value & users = doc["users"];
        Value productName(selectedProduct.productName.c_str(), allocator);

        int userPosition = jsonFindUserPosition(users, allocator);

        if (userPosition == -1) {
            userPosition = jsonCreateNewUser(users, allocator);
        }

        Value & cart = users[userPosition]["cart"];
        Value newProduct(kObjectType);

        newProduct.AddMember("id", cart.Size() + 1, allocator);
        newProduct.AddMember("productName", productName, allocator);
        newProduct.AddMember("quantity", productQty, allocator);
        newProduct.AddMember("productPrice", selectedProduct.productPrice, allocator);
        newProduct.AddMember("amount", selectedProduct.productPrice * productQty, allocator);

        cart.PushBack(newProduct, allocator);

        writeJsonFile(doc, CART_FILE_PATH);
    }

    cout << "   Item has been added to cart? Continue to add? [y/n]: ";
    cin >> option;

    if (option == 'y') {
        system("cls");
        productPage(filepath);
    } 
    else if (option == 'n') {
        system("cls");
        menuPage();
    } 
    else {
        cout << "   Invalid option" << "\n\n";
        cout << "   Item has been added to cart? Continue to add? [y/n]: ";
    }
}

void paymentPage() {
    int option = 0;

    cout << "--------------------------------------" << "\n";
    cout << "             Payment Page             " << "\n";
    cout << "--------------------------------------" << "\n\n";
    cin >> option;
    
}

Document readJsonFile(string readPath) {
    fstream file(readPath, ios::in);
    IStreamWrapper isw(file);

    Document doc;
    doc.ParseStream(isw);

    return doc;
}

void writeJsonFile(Document& doc, string savePath) {
    fstream file(savePath, ios::out);
    OStreamWrapper osw(file);

    PrettyWriter<OStreamWrapper> writer(osw);
    doc.Accept(writer);
}

int jsonFindUserPosition(Value & users, Document::AllocatorType & allocator) {
    int position = -1;

    for (SizeType i = 0; i < users.Size(); i++) {
        if (users[i]["userid"].GetString() == loginInfo.userid) {
            position = i;
        } 
    }

    return (position != -1) ? position : -1;
}

int jsonCreateNewUser(Value & users, Document::AllocatorType & allocator) {
    int position = 0;

    Value newCart(kArrayType);
    Value newUsersObject(kObjectType);
    Value userid(loginInfo.userid.c_str(), allocator) ;

    newUsersObject.AddMember("userid", userid, allocator);
    newUsersObject.AddMember("cart", newCart, allocator);

    position = users.Size();
    users.PushBack(newUsersObject, allocator);

    return position;
}

int charToInt(char num) {
    return static_cast<int>(num) - 48;
}
