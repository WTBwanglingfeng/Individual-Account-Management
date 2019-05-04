#include <iostream>
#include "account.h"
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;
struct deleter {
	template<class T>void operator () (T *p) { delete p; }
};
void converse(vector<Account *>accounts){
	for (vector<Account*>::iterator iter = accounts.begin(); iter != accounts.end(); iter++) {
		for (int i = 0; (*iter)->getId()[i] != '\0'; i++) {
			if ((*iter)->getId()[i] >= 'a' && (*iter)->getId()[i] <= 'z')
				(*iter)->id[i] -= 32;
		}
	}
}
void sorting(vector<Account *> &accounts) {
	Account *temp;
	int min;
	for (size_t i = 0; (i + 1) < accounts.size(); i++) {
		min = i;
		for (size_t j = i + 1; j < accounts.size(); j++) {
			if (*accounts[min] > *accounts[j]) {
				min = j;
			}
		}
		if (min != i) {
			temp = accounts[min];
			accounts[min] = accounts[i];
			accounts[i] = temp;
		}
	}
}
int main()
{
	Date date(2008, 11, 1);
	vector<Account *>accounts;
	ofstream outfile;
	cout << "(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit" << endl;
	char cmd;
	do {
		date.show();
		cout << "\tTotal:" << Account::getTotal() << "\tcommand>";
		char type;
		int index, day;
		double amount, credit, rate, fee;
		string id, desc;
		Account *account;
		Date date1, date2;
		cin >> cmd;
		switch (cmd) {
		case 'a':
			cin >> type >> id;
			if (type == 's') {
				cin >> rate;
				account = new SavingsAccount(date, id, rate);
			}
			else {
				cin >> credit >> rate >> fee;
				account = new CreditAccount(date, id, credit, rate, fee);
			}
			accounts.push_back(account);
			break;
		case 'd':
			cin >> index >> amount;
			getline(cin, desc);
			accounts[index]->deposit(date, amount, desc);
			break;
		case 'w':
			cin >> index >> amount;
			getline(cin, desc);
			accounts[index]->withdraw(date, amount, desc);
			break;
		case 's':
			outfile.open("Personel account management");
			converse(accounts);
			sorting(accounts);		
			for (size_t i = 0; i < accounts.size(); i++) {
				cout << "[" << i << "]";
				accounts[i]->show();
				cout << endl;
				outfile << "[" << i << "]" << accounts[i]->id << "\tBalance:" << accounts[i]->balance << endl;
			}
			outfile.close();
			break;
		case 'c':
			cin >> day;
			if (day < date.getDay())
				cout << "You cannot specify a previous day";
			else if (day > date.getMaxDay())
				cout << "Invalid day";
			else
				date = Date(date.getYear(), date.getMonth(), day);
			break;
		case 'n':
			if (date.getMonth() == 12)
				date = Date(date.getYear() + 1, 1, 1);
			else
				date = Date(date.getYear(), date.getMonth() + 1, 1);
			for (vector<Account*>::iterator iter = accounts.begin();iter != accounts.end(); ++iter)
				(*iter)->settle(date);
			break;
		case 'q':
			date1 = Date::read();
			date2 = Date::read();
			Account::query(date1, date2);
			break;
		}
	} while (cmd != 'e');
	for_each(accounts.begin(), accounts.end(), deleter());
	return 0;
}
