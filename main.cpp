/*
Name: Marlon Alejandro
NSHE: 5002573038
Assignment: 6
Description: Read in account information and allow access to users with the proper credentials
Input: Excel file that contains all account information and user inputs
Output: Options and account details
*/

#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std; 

struct bankType
{
	struct transactionType
	{
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}

		bool type;
		double amount;
	};

	bankType(string first, string last, string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	string name;
	string acctNo;
	double balance;
	unsigned int pin;
	bool locked;

	std::vector<transactionType> transactions;
};


// Read in file and store into hashmap
void readFile(hashMap<string, hashMap<string, bankType> >& bank);
// validate branch exist in our hashmap
bool validBranch(string branch, hashMap<string, hashMap<string, bankType> >& bank);
// validate if account number exist in our hashmap
bool validAcc(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank);
// prompt fro continuation
bool promptContinue();
// Check pin, lock account if more than 3 times
bool validPin(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank);
// Deposit to account
void deposit(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank);
// Withdraw from account
void withdraw(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank);
// View account details
void viewAcc(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank);
//read in user inputs and proccess them
void userInput(hashMap<string, hashMap<string, bankType >>& bank);

int main()
{
	cout << std::fixed << std::setprecision(2);
	hashMap<string, hashMap<string, bankType >> bank;
    readFile(bank);
    userInput(bank);
	return 0;
}

void userInput(hashMap<string, hashMap<string, bankType >>& bank){
    string branch;
    string concat;
    string accNum;
    string option;

    while(true){
		branch = "";
		concat = ""; 
        accNum = "";

		cout << "\nPlease enter bank branch location: ";
		cin >> branch;

        //concatenate two word branches
		if(cin.peek() == ' ') {
			cin >> concat;
			branch = branch + " " + concat; 
		}

        //reformat user inputed branch and validate
		transform(branch.begin(), branch.end(), branch.begin(),::tolower); // Turn user input into lowercase
		if(!validBranch(branch, bank)){
			cout << "\nBank branch not found";
			if(!promptContinue()){
                break;
            }
			else{
                continue;
            } 
		}

        //verify if account is accessible and/or found
		cout << "\nEnter last 4 digits of account: ";
		cin >> accNum;
		if(bank[branch][accNum].locked == true){
			cout << " Account has a lock";
			if(!promptContinue()){
                break;
            }
			else{
                continue;
            } 
		}
		if(!validAcc(accNum, branch, bank)){
			cout << "Account not found";
			if(!promptContinue()){
                break;
            }
			else{
                continue;
            } 
		}

        //validate pin and lock account after 3 attempts
		if(!validPin(accNum, branch, bank)){
			cout << "\n3 failed login attempts, account is now locked";
			if(!promptContinue()){
                break;
            }
			else{
                continue;
            } 
		}

		//prompt user what they would like to do and preform the neccessary operations
        while(true){
			option = "";

			cout << "\n(D)eposit\n" << "(W)ithdrawal\n" << "(V)iew account\n"
					<< "(E)xit account\n" << "Enter choice: "; 
			cin >> option;

			transform(option.begin(), option.end(), option.begin(),::tolower);

			if(option == "d"){
				deposit(accNum, branch, bank);
			}
			else if(option == "w"){
				withdraw(accNum, branch, bank);
			}
			else if(option == "v"){
				viewAcc(accNum, branch, bank);
				}
			else {
				break;
			}
		}
        if(!promptContinue()){
            break;
        }
        else{
            continue;
        } 
    }
}

void readFile(hashMap<string, hashMap<string, bankType> >& bank){
	
    fstream file;
    string String = "";
    string delimiter =",";
    string firstName = "";
    string lastName = "";
    string location = "";
    string acctNo = "";
    float balance = 0.0;
    int pin = 0;

    file.open("data.csv");

    // each line will be formatted as:
    // Firstname, Lastname, Location, Account#, Balance, Pin#
    // delimiter ','

	for(getline(file, String), getline(file, String); !file.eof(); getline(file, String)){
		firstName = String.substr(0, String.find(delimiter)); 
		String.erase(0, String.find(delimiter) + 1);
		
		lastName = String.substr(0, String.find(delimiter)); 
		String.erase(0, String.find(delimiter) + 1);
		
		location = String.substr(0, String.find(delimiter));
		String.erase(0, String.find(delimiter) + 1);
		
		acctNo = String.substr(0, String.find(delimiter));
		String.erase(0, String.find(delimiter) + 1);
		
		balance = std::stof(String.substr(0, String.find(delimiter)));
		String.erase(0, String.find(delimiter) + 1);
		
		pin = stoi(String.substr(0, String.find("\n")));
		String.erase(0, String.find("\n")); 
		
        //reformat location string
		transform(location.begin(), location.end(), location.begin(),::tolower); 

		bank[location][acctNo.substr(6)] = bankType(firstName, lastName, acctNo, balance, pin); 
		bank[location]["0000"].name = "INVALID";
	}

	file.close();
}

bool validBranch(string branch, hashMap<string, hashMap<string, bankType> >& bank){
    //validate branch
	if(bank[branch]["0000"].name != "INVALID"){ 
		return false;
    }
	return true;
}

bool validAcc(string accNum, string branch,hashMap<string, hashMap<string, bankType> >& bank){
    //contains valid string
	if(bank[branch][accNum].name == ""){
    	return false;   
    }
	
	return true;
}

bool validPin(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank){
	int pin = 0;
    size_t attempts=0;

	while(true){
		cout << "\nEnter a Pin: "; 
		cin >> pin;
        //validate inputted pin 
		if(bank[branch][accNum].pin == pin){
			return true;
        }

		cout << "\nInvalid Pin" << endl;
        ++attempts;

        //allowed 3 attempts
        if(attempts>=3){
            bank[branch][accNum].locked = true;
            return false;
        }
	}
}


bool promptContinue(){
	string String = "";
    
	cout << "\nContinue? (Y/N): ";
    cin >> String;
	transform(String.begin(), String.end(), String.begin(),::tolower); 

	if (String == "y"){
        return true;
    } 
	return false;
}

void deposit(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank){
	float amount = 0.0;

	cout << "\nEnter deposit amount: "; 
    cin >> amount;

    // update account balance
	bank[branch][accNum].balance += amount;

    //note account activity
	bank[branch][accNum].transactions.push_back(bankType::transactionType(true, amount)); 
}

void withdraw(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank){
	float amount = 0;

	cout << "\nEnter withdrawal amount: "; 
    cin >> amount;
    
    //validate if account has available balance
	if(amount > bank[branch][accNum].balance){
		cout << "\nNot enough funds to make transaction" << endl;;
		return;
	}

    // update account balance
	bank[branch][accNum].balance -= amount;

    //note account activity
	bank[branch][accNum].transactions.push_back(bankType::transactionType(false, amount)); // Store in transactions
}

void viewAcc(string accNum, string branch, hashMap<string, hashMap<string, bankType> >& bank){
    //showcase account information
	cout << "\nName: " << bank[branch][accNum].name << endl;
    cout<< "Balance: $" << bank[branch][accNum].balance<< endl;
    // If there is no transaction history
	if(bank[branch][accNum].transactions.size() == 0){
		cout << "\nNo Transactions"<< endl;;
    }
	else{
		cout << "\nList of transactions";
        //true- Deposit, false - withdrawal
		for(int i = 0; i < bank[branch][accNum].transactions.size(); i++){
			if(bank[branch][accNum].transactions[i].type == true) {
				cout << "\nDeposit amount $" << bank[branch][accNum].transactions[i].amount;
            }
			else {
				cout << "\nWithdrawal amount $" << bank[branch][accNum].transactions[i].amount;
                }
		}
		cout << '\n';
	}
}