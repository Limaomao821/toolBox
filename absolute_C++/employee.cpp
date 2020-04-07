#include <string>
#include <cstdlib>
#include <iostream>
#include "employee.h"

using std::string;
using std::cout;

namespace SavitchEmplpyees
{
	Employee::Employee():name("No name yet"), ssn("No number yet"), netPay(0)
	{

	}

	Employee::Employee(string theName, string theNumber)
		:name(theName), ssn(theNumber), netPay(0)
	{

	}

	string Employee::getName() const
	{
		return name;
	}

	string Employee::getSsn() const
	{
		return ssn;
	}

	double Employee::getNetPay() const
	{
		return netPay;
	}

	void Employee::setName(string newName)
	{
		name = newName;
	}

	void Employee::setSsn(string newSsn)
	{
		ssn = newSsn;
	}

	void Employee::setNetPay(double newNetPay)
	{
		netPay = newNetPay;
	}

	void Employee::printCheck() const
	{
		cout 	<< "\nERROR: printCheck FUNCTION CALLED FOR AN \n"
			<< "UNDIFFERENTIATED EMPLOYEE. Aborting the program.\n"
			<< "Check with the author of the program about this bug.\n";
		exit(1);
	}
}
