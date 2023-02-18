/*
    Project    : Supermarket Billing System
    Created On : 21 January 2023
*/
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
#include <cctype>
#include <conio.h>
#include <map>
#include "libraries/rapidjson/document.h"
#include "libraries/rapidjson/ostreamwrapper.h"
#include "libraries/rapidjson/istreamwrapper.h"
#include "libraries/rapidjson/prettywriter.h"
#include "libraries/color.hpp"

#define CART_FILE_PATH "data/cart.json"
#define CREDENTIALS_FILE_PATH "data/credentials.csv"
#define WIDTH 70

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

struct{
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
        front = width / 2.0 + ceil(text.length() / 2.0);
        back = width - front;
        cout << '|' << setw(front - 1) << right << color(text) << setw(back - 1) << color(" ") << "|\n";
    } else {
        front = width / 2.0 + ceil(text.length() / 2.0);
        back = width - front;
        cout << setw(front) << right << color(text) << setw(back) << color(" ") << '\n';
    }
}

/**
 * @brief  Check the length of credentials not exceed the limit and 
 *         make sure doesn't have blank space
 * @param  credentials  The credentials that need to be checked
 * @param  page         Page to be display after error message showed
 */
template <typename T, size_t N>
void checkCredentials(T (&credentials)[N], void (*page)()) {
    string credentials_type = sizeof(credentials) == 13 ? "Username" : "Password";
    string credentials_size = to_string(sizeof(credentials) - 1);

    // check if the length of credentials is longer than the limit?
    if (sizeof(credentials) <= strlen(credentials)) {
            system("cls||clear");
            printCenter(credentials_type + " cannot exceed " + credentials_size + " characters", bg_red);            
            page();
    }

    // loop each char in credentials to check if found any blank character
    for (int i = 0; i < strlen(credentials); i++) {
        if (isspace(credentials[i])) {
            system("cls||clear");
            printCenter(credentials_type + " cannot contain blank", bg_red);
            page();
        }
    }
}

/**
 * @brief  Display products in a table form with specify headers
 * @param  headers     The headers of the table
 * @param  data        The data tha need to be display in the cells of table
 * @param  alloc       The allocator of a DOM of a json
 * @param  printTotal  Print the total amount in the last column of table
 */
template <size_t N>
void displayTable(string (&headers)[N], Value & data, Document::AllocatorType & alloc, bool printTotal) {
    int averageSpace = 0;
    int lastSpace = 0;
    int usedSpace = 0;
    int intervals = N + 1;
    int space = 0;
    double totalAmount = 0.0;

    map<string, int> tableMap;
    map<string, string> jsonMap;

    // key value pair of header names with its 
    // space used inside a column of table
    tableMap["No."] = 3;
    tableMap["Item"] = 30;
    tableMap["Qty"] = 4;
    tableMap["Price"] = 6;
    tableMap["Amount"] = 6;

    // map between the value of header names that 
    // displayed on screen with the keys in json
    jsonMap["No."] = "id";
    jsonMap["Item"] = "name";
    jsonMap["Qty"] = "quantity";
    jsonMap["Price"] = "price";
    jsonMap["Amount"] = "amount";

    for (int i = 0; i < N; i++)
        usedSpace += tableMap[headers[i]];

    countSpaceBetween(usedSpace, intervals, &averageSpace, &lastSpace, WIDTH);

    margin();
    cout << "|";

    // loop to display the header names
    int i = 0;
    while (i < N) {
        space = tableMap[headers[i]];

        cout << string(averageSpace, ' ') << setw(tableMap[headers[i]]);

        if (space == 3 || space == 30)
            cout << left << headers[i];
        else if (space == 4 || space == 6)
            cout << right << headers[i];
        i++;
    }
    
    cout << string(lastSpace, ' ') << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    
    // loop to display the data in table
    for (const auto & p: data.GetArray()) {
        margin(); 
        cout << "|";

        int i = 0;
        while (i < N) {
            string header1 = jsonMap[headers[i]];
            Value header(header1.c_str(), alloc);

            space = tableMap[headers[i]];

            cout << string(averageSpace, ' ') << setw(space);

            switch (space) {
                case 3:
                    cout << left << p[header].GetInt();
                    break;
                case 30:
                    cout << left << p[header].GetString();
                    break;
                case 4:
                    cout << right << p[header].GetInt();
                    break;
                case 6:
                    cout << right << fixed << setprecision(2) << p[header].GetDouble();
                    break;
            }
            i++;
        }
        cout << string(lastSpace, ' ') << "|\n";

        if (printTotal)
            totalAmount += p["amount"].GetDouble();
    }

    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');

    // display the total amount if printTotal set to true
    if (printTotal) {
        margin();
        cout << setw(16) << left << "|   Total Amount"
            << setw(WIDTH - 16 - lastSpace) << right << totalAmount
            << string(lastSpace, ' ') << "|\n";
        lineDivider('-');
    }
}

/**
 * @brief  Provide a specify left margin for the interface
 *         so it will not stick on the left side of console
 */
void margin() {
    cout << string(25, ' ');
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
    char option;

    cout << '\n';
    lineDivider('*');
    printCenter("Welcome to Tesco Supermarket", bg_blue);
    lineDivider('*');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     1. Login"  << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     2. Signup" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     3. Quit" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;
    
    system("cls||clear");

    switch (option) {
        case '1': 
            loginPage(); 
            break;
        case '2': 
            signupPage(); 
            break;
        case '3':
            exit(0);
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
    lineDivider('*');
    printCenter("Login Page", bg_blue);
    lineDivider('*'); 
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
    printCenter("Invalid username or password, please try again.", bg_red);
    welcomePage();
}

void signupPage() {
    int numOfLine = 0;
    string line;
    string newUserid;
    char newUsername[13];
    char newPassword[9];

    cout << '\n';
    lineDivider('*');
    printCenter("Signup Page", bg_blue);
    lineDivider('*'); 
    cout << '\n';
    margin(); 
    cout << "     New username : ";
    cin.ignore();
    cin.getline(newUsername, 256);
    checkCredentials(newUsername, &signupPage);

    margin(); 
    cout << "     New password : ";
    cin.getline(newPassword, 256);
    checkCredentials(newPassword, &signupPage);

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
    char option;

    cout << '\n';
    lineDivider('*');
    printCenter("Main Page", bg_blue);
    lineDivider('*');
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
        case '1': 
            menuPage(); 
            break;
        case '2': 
            accountPage(); 
            break;
        case '3': 
            cartPage();
            break;
        case '4': 
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

    cout << '\n';
    lineDivider('*');
    printCenter("Menu Page", bg_blue);
    lineDivider('*');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|   Categories:"            << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     1. Biscuit & Cookies" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     2. Beverages"         << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     3. Health & Beauty"   << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|   Enter (b): Back to previous page" << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|   Enter (p): Proceed to checkout"  << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    system("cls||clear");

    switch (option) {
        case '1': 
            productPage("data/biscuit.json"); 
            break;
        case '2': 
            productPage("data/beverages.json"); 
            break;
        case '3': 
            productPage("data/health.json"); 
            break;
        case 'b': 
            mainPage(); 
            break;
        case 'p': 
            cartPage(); 
            break;
        default: 
            printCenter("Invalid option", bg_red);
            menuPage();
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
    string tempCredentials = "";
    char newUsername[13];
    char newPassword[9];

    cout << '\n';
    lineDivider('*');
    printCenter("Account Page", bg_blue);
    lineDivider('*');
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    margin(); 
    cout << setw(17) << "|     Username : " << setw(WIDTH - 17) << loginInfo.username << "|\n";
    margin(); 
    cout << setw(17) << "|     Password : " << setw(WIDTH - 17) << loginInfo.password << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|" << "|\n";
    lineDivider('-');
    margin(); 
    cout << setw(WIDTH) << left << "|     Enter (p): Change account info"   << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|     Enter (b): Back to previous page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    fstream fileIn(CREDENTIALS_FILE_PATH, ios::in);
        
    if (option == 'p') {
        margin();
        cout << "   Enter new username : ";
        cin.ignore();
        cin.getline(newUsername, 256);
        checkCredentials(newUsername, &accountPage);

        margin();
        cout << "   Enter new password : ";
        cin.getline(newPassword, 256);
        checkCredentials(newPassword, &accountPage);

        // iterate every line in file
        while (getline(fileIn, line)) {
            // split the line into multiple words separated by comma
            stringstream str(line);
            getline(str, word, ',');

            if (word != loginInfo.userid) {
                tempCredentials += line + '\n';
            }
            else if (word == loginInfo.userid) {
                tempCredentials += loginInfo.userid + ',' + newUsername + ',' + newPassword + '\n';

                loginInfo.username = newUsername;
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
 * @brief  The interface of cart page.
 *         User is allow to view and decide whether to
 *         remove product from cart or proceed to checkout.
 */
void cartPage() {
    char option; 

    Document doc = readJsonFile(CART_FILE_PATH);
    Document::AllocatorType & allocator = doc.GetAllocator();
    
    // Get the value of "users" key
    Value & users = doc["users"];
    int userPosition = jsonFindUserPosition(users);

    // if the username is not exist in the file, create and add a new one
    if (userPosition == -1) 
        userPosition = jsonCreateNewUser(users, allocator);

    // Get the value of "cart" key
    Value & cart = users[userPosition]["cart"];

    string headers[] = {"No.", "Item", "Qty", "Price", "Amount"};

    cout << '\n';
    lineDivider('*');
    printCenter("Cart Page", bg_blue);
    lineDivider('*');

    displayTable(headers, cart, allocator, true);

    margin(); 
    cout << setw(WIDTH) << left << "|   Enter (p): Proceed to checkout"   << "|\n";
    margin(); 
    cout << setw(WIDTH) << left << "|   Enter (b): Back to previous page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter number to unselect product: ";
    cin  >> option;

    system("cls||clear");

    if (option == 'p') {
        // don't allow checkout if no item in cart
        if (cart.Size() == 0) {
            printCenter("Cart has no item, please add one", bg_red);
            cartPage();
        }
        else
            paymentPage();
    }
    else if (option == 'b') {
        mainPage();
    }
    // stoi(&option) convert option from char to int
    else if (stoi(&option) > 0 && stoi(&option) <= cart.Size()) {
        // remove selected product from cart
        cart.Erase(cart.Begin() + stoi(&option) - 1);

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
    int selectedProductId = 0;
    int selectedProductQty = 0;

    Document doc = readJsonFile(filepath);
    Document::AllocatorType & allocator = doc.GetAllocator();

    Value & category = doc["category"];
    Value & products = doc["products"];

    string headers[] = {"No.", "Item", "Qty", "Price"};

    cout << '\n';
    lineDivider('*');
    printCenter(category.GetString(), bg_blue);
    lineDivider('*');

    displayTable(headers, products, allocator, false);

    margin(); 
    cout << setw(WIDTH) << left << "|   Enter (b): Back to previous page" << "|\n";
    lineDivider('='); 
    cout << '\n';
    margin(); 
    cout << "   Enter your choice: ";
    cin  >> option;

    try {
        selectedProductId = stoi(&option);
    }
    catch (...) {
        selectedProductId = 0;
    }

    if (option == 'b') {
        system("cls||clear"); 
        menuPage();
    }
    else if (selectedProductId > 0 && selectedProductId <= products.Size()){
        margin();
        cout << "   Enter the quantity: ";
        cin >> selectedProductQty;
        cout << "\n";

        Value & selectedProduct = products[selectedProductId - 1];
        string name = selectedProduct["name"].GetString();
        Value productName(name.c_str(), allocator);

        Document doc2 = readJsonFile(CART_FILE_PATH);
        Document::AllocatorType & allocator2 = doc2.GetAllocator();

        Value & users = doc2["users"];

        int userPosition = jsonFindUserPosition(users);
        
        // if user info not found in json, then create a new user in json
        if (userPosition == -1) {
            userPosition = jsonCreateNewUser(users, allocator2);
        }

        Value & cart = users[userPosition]["cart"];
        Value newProduct(kObjectType);

        // add the information of selected product in a key value pair
        newProduct.AddMember("id", cart.Size() + 1, allocator2);
        newProduct.AddMember("name", productName, allocator2);
        newProduct.AddMember("quantity", selectedProductQty, allocator2);
        newProduct.AddMember("price", selectedProduct["price"].GetDouble(), allocator2);
        newProduct.AddMember("amount", selectedProduct["price"].GetDouble() * selectedProductQty, allocator2);

        // append the product details into the cart array of logged user
        cart.PushBack(newProduct, allocator2);

        // decrease the product quantity in store
        selectedProduct["quantity"] = selectedProduct["quantity"].GetInt() - selectedProductQty;

        writeJsonFile(doc, filepath);
        writeJsonFile(doc2, CART_FILE_PATH);
    }
    else {
        system("cls||clear");
        printCenter("Invalid Option", bg_red);
        productPage(filepath);
    }

    margin();
    cout << "   Item has been added to cart? Continue to add? [y/n]: ";
    cin  >> option;

    option = tolower(option);

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

    cout << '\n';
    lineDivider('*');
    printCenter("Payment Page", bg_blue);
    lineDivider('*');
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
    cout << setw(WIDTH) << left << "|   Enter (b): Back to main page" << "|\n";
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
    else if (option == 'b') 
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
    char datetime1[50];
    string datetime2;

    // get the currect date and time
    time_t t = time(0);
    // format the date and time into specify format, in this case 
    // is dd/mm/yyyy and time in 12-hour clock
    strftime(datetime1, 50, "%d / %m / %Y   %I:%M %p", localtime(&t));
    // convert char array to string
    datetime2 = datetime1;

    Document doc = readJsonFile(CART_FILE_PATH);
    
    Value & users = doc["users"];
    int userPosition = jsonFindUserPosition(users);
    Value & cart = users[userPosition]["cart"];

    string headers[] = {"Qty", "Item", "Amount"};

    cout << '\n';
    lineDivider('*');
    printCenter("Receipt", bg_blue);
    lineDivider('*');
    printCenter("Tesco", bg_default, true);
    printCenter("TEL: 043456789", bg_default, true);
    lineDivider('-');

    displayTable(headers, cart, doc.GetAllocator(), true);

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
        if (users[i]["userid"].GetString() == loginInfo.userid) {
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

    // initialize a empty value of json object
    Value newCart(kArrayType);
    Value newUsersObject(kObjectType);
    Value userid(loginInfo.userid.c_str(), allocator) ;

    // exp: 
    // {
    //    "useid": 1,
    //    "cart": []
    // }
    newUsersObject.AddMember("userid", userid, allocator);
    newUsersObject.AddMember("cart", newCart, allocator);

    position = users.Size();
    users.PushBack(newUsersObject, allocator);

    return position;
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
void countSpaceBetween(int usedSpace, int intervals, int* averageSpace, int* lastSpace, int width) {
    int totalSpace = width - usedSpace - 2;
    int avgSpace = totalSpace / intervals;

    int leftSpace = totalSpace - avgSpace * intervals;
    int finalSpace = avgSpace + leftSpace + 1;

    *averageSpace = avgSpace;
    *lastSpace = finalSpace;
}