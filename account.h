#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__
#include "date.h"
#include "accumulator.h"
#include <map>
#include <string>
class Account;
class AccountRecord {
public:
	Date date;
	const Account *account;
	double amount;
	double balance;
	std::string desc;
	AccountRecord(const Date &date, const Account *account, double amount, double balance, const std::string &desc);
	void show() const;
	bool operator > (const AccountRecord &account) const {
		return (balance > account.balance);
	}
};
typedef std::multimap<Date, AccountRecord>RecordMap;
class Account {
public:
	std::string id;
	double balance;
	static double total;
	static RecordMap recordMap;
protected:
	Account(const Date &date, const std::string &id);
	void record(const Date &date, double amount, const std::string &desc);
	void error(const std::string &msg) const;
public:
	const std::string &getId() const { return id; }
	double getBalance() const { return balance; }
	static double getTotal() { return total; }
	virtual void deposit(const Date &date, double amount, const std::string &desc) = 0;
	virtual void withdraw(const Date &date, double amount, const std::string &desc) = 0;
	virtual void settle(const Date &date) = 0;
	virtual void show() const;
	static void query(const Date &begin, const Date &end);
	bool operator > (const Account &account) const {
		return (balance > account.getBalance());
	}
};
class SavingsAccount : public Account {
private:
	Accumulator acc;
	double rate;
public:
	SavingsAccount(const Date &date, const std::string &id, double rate);
	double getRate() const { return rate; }
	void deposit(const Date &date, double amount, const std::string &desc);
	void withdraw(const Date &date, double amount, const std::string &desc);
	void settle(const Date &date);
	bool operator > (const SavingsAccount &account) const {
		return (balance > account.getBalance());
	}
};
class CreditAccount : public Account {
private:
	Accumulator acc;
	double credit;
	double rate;
	double fee;
	double getDebt() const {
		double balance = getBalance();
		return (balance < 0 ? balance : 0);
	}
public:
	CreditAccount(const Date &date, const std::string &id, double credit, double rate, double fee);
	double getCredit() const { return credit; }
	double getRate() const { return rate; }
	double getFee() const { return fee; }
	double getAvailableCredit() const {
		if (getBalance() < 0)
			return credit + getBalance();
		else
			return credit;
	}
	void deposit(const Date &date, double amount, const std::string &desc);
	void withdraw(const Date &date, double amount, const std::string &desc);
	void settle(const Date &date);
	void show() const;
	bool operator > (const CreditAccount &account) const {
		return (balance > account.getBalance());
	}
};																			
#endif
