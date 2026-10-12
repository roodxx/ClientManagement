
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <windows.h>
#include <cctype>

using namespace std;

const string ClientsFileName = "Clients.txt";

struct strClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};


enum enOptions
{
    ShowClient = 1, Add, Delete, Update, Find, Transactions, EXIT
};

enum enTransactions { Deposit = 1, Withdraw, TotalBalance, MainMenu };


vector<string> SplitString(string S1, string Delim)
{
    vector<string>vStr;
    string sWord;
    size_t pos = 0;
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vStr.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
        vStr.push_back(S1);
    return vStr;

}

strClient ConvertLineToClientRecord(string Line, string Separator = "#//#")
{
    strClient Client;
    vector<string> vClientData = SplitString(Line, Separator);

    if (vClientData.size() >= 5)
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }

    return Client;
}

string ConvertClientRecordToLine(strClient Client, string Separator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Separator;
    stClientRecord += Client.PinCode + Separator;
    stClientRecord += Client.Name + Separator;
    stClientRecord += Client.Phone + Separator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector<strClient> LoadClientsFromFile(string FileName)
{
    vector<strClient> vClients;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;

        while (getline(MyFile, Line))
        {
            strClient Client = ConvertLineToClientRecord(Line);

            if (!Client.AccountNumber.empty())
                vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

void SaveClientsToFile(string FileName, vector<strClient> vClients)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (strClient Client : vClients)
        {
            if (!Client.MarkForDelete)
                MyFile << ConvertClientRecordToLine(Client) << endl;
        }

        MyFile.close();
    }
}

void AddRecordToFile(string FileName, string DataLine)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}


string ReadAccountNumber()
{
    string AccountNumber = "";

    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;

    return AccountNumber;
}

short ReadMainMenuOption()
{
    short Option = 0;

    cout << "\t\t  Select an option [1-7]: ";
    cin >> Option;

    while (cin.fail() || Option < 1 || Option > 7)
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "\n\t\t  Invalid selection. Please enter a number between 1 and 7: ";
        cin >> Option;
    }

    return Option;
}

short ReadTransactionOption() {
    short Option = 0;

    cout << "\t\t  Select an option [1-4]: ";
    cin >> Option;

    while (cin.fail() || Option < 1 || Option > 4)
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "\n\t\t  Invalid selection. Enter a number between 1 and 4: ";
        cin >> Option;
    }

    return Option;
}


bool FindClientByAccountNumber(string AccountNumber, vector<strClient> vClients, strClient& Client)
{
    for (const strClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
        
    }

    return false;
}

bool IsClientExist(string AccountNumber, string FileName)
{

    vector<strClient> vClients = LoadClientsFromFile(FileName);

    for (strClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
            return false;
    }

    return true;
}

bool MarkClientForDeletion(string AccountNumber, vector<strClient>& vClients)
{
    for (strClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }

    return false;
}


strClient ReadNewClientDetails()
{
    strClient Client;


    cout << "Account Number          : ";
    getline(cin >> ws, Client.AccountNumber);

    while (!IsClientExist(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nError: Account Number [" << Client.AccountNumber << "] Already Exists.\n\n";
        cout << "Enter a Different Account Number: ";

        getline(cin, Client.AccountNumber);
    }

    cout << "Security PIN            : ";
    getline(cin, Client.PinCode);

    cout << "Account  Name           : ";
    getline(cin, Client.Name);

    cout << "Contact Number          : ";
    getline(cin, Client.Phone);

    cout << "Initial Account Balance : ";
    cin >> Client.AccountBalance;

    cout << "\n==============================================\n";

    return Client;
}
strClient ReadUpdatedClientDetails(string AccountNumber)
{
    strClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n============================================================\n";
    cout << "                  UPDATE CLIENT INFORMATION\n";
    cout << "============================================================\n\n";

    cout << "Security PIN            : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Account Holder Name     : ";
    getline(cin, Client.Name);

    cout << "Contact Number          : ";
    getline(cin, Client.Phone);

    cout << "Account Balance         : $";
    cin >> Client.AccountBalance;

    cout << "\n============================================================\n";

    return Client;
}


void PrintClientRecord(strClient Client)
{
    cout << "\t\t  | " << left << setw(15) << Client.AccountNumber
        << " | " << setw(30) << Client.Name
        << " | " << setw(13) << Client.Phone
        << " | " << right << setw(12) << fixed << setprecision(2) << Client.AccountBalance << " |\n";
}

void PrintClientDetails(strClient Client)
{
    cout << "\n";
    cout << "\t\t  ================================================================\n";
    cout << "\t\t                       CLIENT ACCOUNT DETAILS\n";
    cout << "\t\t  ================================================================\n\n";

    cout << "\t\t    Account Number    : " << Client.AccountNumber << '\n';
    cout << "\t\t    Account Holder    : " << Client.Name << '\n';
    cout << "\t\t    Contact Number    : " << Client.Phone << '\n';

    cout << "\t\t    Account Balance   : $"<< fixed << setprecision(2) << Client.AccountBalance << '\n';

    cout << "\n\t\t  ================================================================\n";
}

  void PrintClientsTable(vector<strClient> vClients)
    {
        system("cls");

        const string Border ="+-----------------+--------------------------------+---------------+--------------+";

        cout << "\n\n";
        cout << "\t\t\t                       [ CLIENT DIRECTORY ]\n\n";


        cout << "\t\t  Total Client Records: " << vClients.size() << "\n";

        cout << "\t\t  " << Border << "\n";
        cout << "\t\t  | " << left << setw(15) << "Account Number"
            << " | " << setw(30) << "Account Name"
            << " | " << setw(13) << "Phone Number"
            << " | " << right << setw(12) << "Balance" << " |\n";

        cout << "\t\t  " << Border << "\n";

        for (const strClient& Client : vClients)
        {
            PrintClientRecord(Client);

        }

        cout << "\t\t  " << Border << "\n\n";
    }


void AddNewClient()
{
    strClient Client = ReadNewClientDetails();

    AddRecordToFile(ClientsFileName, ConvertClientRecordToLine(Client));

    cout << "\nClient added successfully.\n";
}

void AddClients()
{
    char AddMore = 'y';

    do
    {
        system("cls");
        cout << "\n============================================================\n";
        cout << "                  CLIENT REGISTRATION\n";
        cout << "============================================================\n";

        AddNewClient();

        cout << "\nAdd another client? [Y/N]: ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
}
    

bool UpdateClientByAccountNumber(string AccountNumber,vector<strClient>& vClients)
{
    strClient Client;
    char Answer = 'N';

    if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nError: Client with account number [" << AccountNumber << "] was not found.\n";

        return false;
    }

    PrintClientDetails(Client);

    cout << "\nConfirm client information update? [Y/N]: ";
    cin >> Answer;

    if (toupper(Answer) != 'Y')
    {
        cout << "\n============================================================\n";
        cout << "                   UPDATE CANCELLED\n";
        cout << "============================================================\n";

        return false;
    }

    for (strClient& CurrentClient : vClients)
    {
        if (CurrentClient.AccountNumber == AccountNumber)
        {
            CurrentClient = ReadUpdatedClientDetails(AccountNumber);
            break;
        }
    }

    SaveClientsToFile(ClientsFileName, vClients);

    cout << "\nClient information updated successfully.\n";

    return true;
}

bool DeleteClientByAccountNumber(string AccountNumber,vector<strClient>& vClients)
{
    strClient Client;
    char Answer = 'N';

    if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nError: Client with account number ["<< AccountNumber << "] was not found.\n";

        return false;
    }

    PrintClientDetails(Client);

    cout << "\nConfirm deletion of this client account? [Y/N]: ";
    cin >> Answer;

    if (toupper(Answer) != 'Y')
    {
        cout << "\n============================================================\n";
        cout << "                   DELETION CANCELLED\n";
        cout << "============================================================\n";

        return false;
    }

    if (!MarkClientForDeletion(AccountNumber, vClients))
        return false;

    SaveClientsToFile(ClientsFileName, vClients);

    vClients = LoadClientsFromFile(ClientsFileName);

    cout << "\nClient account deleted successfully.\n";

    return true;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double DepositAmount,vector<strClient>& vClients)
{
    while (cin.fail() || DepositAmount <= 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\nInvalid amount. Enter a positive deposit amount: ";
        cin >> DepositAmount;
    }

    char Answer = 'N';

    cout << "\nConfirm deposit transaction? [Y/N]: ";
    cin >> Answer;

    if (toupper(Answer) != 'Y')
    {
        cout << "\n============================================================\n";
        cout << "                 DEPOSIT TRANSACTION CANCELLED\n";
        cout << "============================================================\n";

        return false;
    }

    for (strClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.AccountBalance += DepositAmount;

            SaveClientsToFile(ClientsFileName, vClients);

            cout << "\n============================================================\n";
            cout << "                 DEPOSIT COMPLETED SUCCESSFULLY\n";
            cout << "============================================================\n";

            cout << "\nDeposit Amount : $"
                << fixed << setprecision(2) << DepositAmount;

            cout << "\nNew Balance    : $"
                << fixed << setprecision(2) << Client.AccountBalance;

            cout << "\n============================================================\n";

            return true;
        }
    }

    cout << "\nError: Client account was not found.\n";

    return false;
}

bool WithDrawBalanceToClientByAccountNumber(string AccountNumber,double WithDrawAmount,vector<strClient>& vClients)
{
    for (strClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            while (cin.fail() || WithDrawAmount <= 0 ||   WithDrawAmount > Client.AccountBalance)
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nInvalid withdrawal amount.\n";
                cout << "Available Balance : $" << fixed << setprecision(2)<< Client.AccountBalance;
                cout << "\nEnter a valid withdrawal amount: ";
                cin >> WithDrawAmount;
            }

            char Answer = 'N';

            cout << "\nConfirm withdrawal transaction? [Y/N]: ";
            cin >> Answer;

            if (toupper(Answer) != 'Y')
            {
                cout << "\n============================================================\n";
                cout << "                WITHDRAWAL TRANSACTION CANCELLED\n";
                cout << "============================================================\n";

                return false;
            }

            Client.AccountBalance -= WithDrawAmount;

            SaveClientsToFile(ClientsFileName, vClients);

            cout << "\n============================================================\n";
            cout << "               WITHDRAWAL COMPLETED SUCCESSFULLY\n";
            cout << "============================================================\n";

            cout << "\nWithdrawal Amount : $" << fixed << setprecision(2) << WithDrawAmount;

            cout << "\nRemaining Balance : $" << fixed << setprecision(2) << Client.AccountBalance;

            cout << "\n============================================================\n";

            return true;
        }
    }

    cout << "\nError: Client account was not found.\n";

    return false;
}

void DisplayTransactionsMenu();
void RunMainMenu();
void TransactionsSelection(enTransactions Option);
void WaitAndReturnToMainMenu();
void ReturnToTransactionsMenu();

void DisplayMainMenu()
{
    system("cls");

    cout << "\n\n";

    cout << "\t\t  +================================================================+\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |                 CLIENT MANAGEMENT SYSTEM                       |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  +================================================================+\n";

    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |                         MAIN MENU                              |\n";
    cout << "\t\t  |________________________________________________________________|\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [1]   List All Clients                                   |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [2]   Add New Client                                     |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [3]   Delete Client                                      |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [4]   Update Client Information                          |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [5]   Search Clients                                     |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [6]   Manage Transactions                                |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  |       [7]   Exit Application                                   |\n";
    cout << "\t\t  |                                                                |\n";
    cout << "\t\t  +================================================================+\n\n";

}

void DisplayClientsList()
{
    system("cls");

    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    PrintClientsTable(vClients);
}

void DisplayAddClientsScreen()
{
    system("cls");

    AddClients();
}

void DisplayDeleteClientScreen()
{
    system("cls");



    cout << "\n============================================================\n";
    cout << "                      CLIENT DELETION\n";
    cout << "============================================================\n";
    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void DisplayUpdateClientScreen()
{
    system("cls");

    cout << "\n============================================================\n";
    cout << "                      UPDATE CLIENT\n";
    cout << "============================================================\n";
    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void DisplayFindClientScreen()
{
    system("cls");
    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    strClient Client;
    cout << "\n============================================================\n";
    cout << "                       CLIENT SEARCH\n";
    cout << "============================================================\n";
    string AccountNumber = ReadAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientDetails(Client);
    else
        cout << "\nClient with account number (" << AccountNumber << ") was not found.\n";
}

void DisplayExitScreen()
{
    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "\n\n";

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\t\t|==================================================|\n";
    cout << "\t\t|                                                  |\n";

    SetConsoleTextAttribute(hConsole, 14);
    cout << "\t\t|           PROGRAM ENDED SUCCESSFULLY             |\n";

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\t\t|                                                  |\n";
    cout << "\t\t|==================================================|\n";

    SetConsoleTextAttribute(hConsole, 15);
    cout << "\t\t|          Thank You For Using Our Program!        |\n";

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\t\t|==================================================|\n\n";

    SetConsoleTextAttribute(hConsole, 7);
    cout << "\n\t\tPress any key to exit...";
    system("pause > 0");
}

void ShowDepositScreen(){

    system("cls");

    cout << "\n================================================================\n";
    cout << "\t\t      [ DEPOSIT SCREEN ]\n";
    cout << "================================================================\n\n"; 

    strClient Client;
    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with account number (" << AccountNumber << ") was NOT found.\n";
        cout << "Enter another Account Number: ";
        getline(cin >> ws, AccountNumber);
    }
        PrintClientDetails(Client);

         double DepositAmount = 0;
        cout << "\nEnter Deposit Amount ?";
        cin >> DepositAmount;

        DepositBalanceToClientByAccountNumber(AccountNumber, DepositAmount, vClients);

 

    }

void ShowWithdrawScreen(){
    system("cls");
    cout << "\n================================================================\n";
    cout << "\t\t      [ WITHDRAW SCREEN ]\n";
    cout << "================================================================\n\n";

    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    strClient Client;
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with account number (" << AccountNumber << ") was NOT found.\n";
        cout << "Enter another Account Number: ";
        getline(cin >> ws, AccountNumber);

    }
    PrintClientDetails(Client);

     double WithDrawAmount = 0;
    cout << "\nEnter WithDraw Amount ?";
    cin >> WithDrawAmount;

    while (WithDrawAmount > Client.AccountBalance)
    {
        cout << "\nAmount exceeds the available balance!\n";
        cout << "Current Balance is : $" << Client.AccountBalance << "\n";
        cout << "\nPlease enter a smaller amount : ";
        cin >> WithDrawAmount;
    }
    WithDrawBalanceToClientByAccountNumber(AccountNumber, WithDrawAmount, vClients);


}

void ShowTotalBalancesScreens(){
        system("cls");
    vector<strClient> vClients = LoadClientsFromFile(ClientsFileName);
    const string Border = "\t\t+-----------------+-----------+-----------------------------------+";

    cout << "\n\t\t===================================================================\n";
    cout << "\t\t                        TOTAL BALANCSE \n";
    cout << "\t\t===================================================================\n\n";
    cout << "\t\t\t\t\tTotal Records: " << vClients.size() << "\n\n";
    cout << Border << '\n';
    cout << "\t\t| " << left << setw(15) << "Account Number" << " | "
        << left << setw(30) << "Client Name" << " | "
        << left << setw(12) << "Balance" << " |\n";
    cout << Border << '\n';

     double TotalBalances = 0;
     if (vClients.size() == 0)
         cout << "\t\tDon't there Clients in the system.";
     
     else 
        for (const strClient& Client : vClients)
        {

            cout << "\t\t| "
                << left << setw(15) << Client.AccountNumber << " | "
                << left << setw(30) << Client.Name << " | "
                << left << setw(12) << Client.AccountBalance << " |\n";
            TotalBalances += Client.AccountBalance;
        }
        cout << Border << "\n\n";
        cout << "\t\tTotal Balances = " << TotalBalances << "\n\n";
    
    
}

void TransactionsSelection(enTransactions Option)
{
    switch (Option)
    {
    case enTransactions::Deposit:

        ShowDepositScreen();
        ReturnToTransactionsMenu();

        break;

    case enTransactions::Withdraw:

        ShowWithdrawScreen();
        ReturnToTransactionsMenu();
        break;

    case enTransactions::TotalBalance:

        ShowTotalBalancesScreens();
        ReturnToTransactionsMenu();
        break;

    case enTransactions::MainMenu:

        RunMainMenu();

        break;
    }
}

void DisplayTransactionsMenu() {

    system("cls");
    cout << "\t\t  ================================================================\n";
    cout << "\t\t                      Transactions  Menu\n";
    cout << "\t\t  ================================================================\n";
    cout << "\n\n";
    cout << "\t\t          [1]   Deposit.\n\n";
    cout << "\t\t          [2]   Withdraw.\n\n";
    cout << "\t\t          [3]   Total Balances\n\n";
    cout << "\t\t          [4]   Main Menu\n\n";
    cout << "\n";
    cout << "\t\t  ================================================================\n";

  
    TransactionsSelection((enTransactions)ReadTransactionOption());

}

void MainMenuSelection(enOptions Option)
{
    switch (Option)
    {
    case enOptions::ShowClient:
        DisplayClientsList();
        WaitAndReturnToMainMenu();
        break;

    case enOptions::Add:
        DisplayAddClientsScreen();
        WaitAndReturnToMainMenu();
        break;

    case enOptions::Delete:
        DisplayDeleteClientScreen();
        WaitAndReturnToMainMenu();
        break;

    case enOptions::Update:
        DisplayUpdateClientScreen();
        WaitAndReturnToMainMenu();
        break;

    case enOptions::Find:
        DisplayFindClientScreen();
        WaitAndReturnToMainMenu();
        break;
    case enOptions::Transactions:
        DisplayTransactionsMenu();
        break;

    case enOptions::EXIT:
        DisplayExitScreen();
        return;
    }
}

void WaitAndReturnToMainMenu()
{
    cout << "\n\t\t----------------------------------------------\n";
    cout << "\t\tPress any key to return to the Main Menu...";
    system("pause>0");
    RunMainMenu();
}

void ReturnToTransactionsMenu()
{
    cout << "\n\t\t----------------------------------------------\n";
    cout << "\t\tPress any key to return to Transactions Menu...";
    system("pause>0");
    DisplayTransactionsMenu();
}


void RunMainMenu()
{
    enOptions Option;
    DisplayMainMenu();
    Option = (enOptions)ReadMainMenuOption();
    MainMenuSelection(Option);

}


int main()
{
    RunMainMenu();
    return 0;
}
