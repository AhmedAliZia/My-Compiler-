#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "Symbol.h"
using namespace std;


//Print Error and Terminate
void PrintError2(string str)
{
	cout << str << endl;
	abort();
}

//Helper Struct for Calculations
struct Calculation
{
	string value;
	string op;
	string value2;
};


class VM
{
	vector<string> lines;
	map<string, Symbol> SymbolTable;
	vector<bool> defined;
	vector<int> data;              //Stores values of variables
	int currentLine = 0;

public:
	VM(map<string, Symbol>& table)
	{
		SymbolTable = table;
		//for (map<string, Symbol>::const_iterator it = SymbolTable.begin();
		//	it != SymbolTable.end(); ++it)
		//{
		//	std::cout << it->first << " " << it->second.type << " " << it->second.address << "\n";
		//}
		for (int i = 0; i < SymbolTable.size(); i++)
		{
			defined.push_back(false);
			data.push_back(-1);
		}
	}

	//Get string after index s 
	string truncate(string str, int s)
	{
		string temp = "";
		if (str != "")
		{
			for (int i = s; i < str.size(); i++)
			{
				temp += str[i];
			}
		}
		return temp;
	}

	//Read from File machinecode.txt
	void readMachineCode()
	{
		ifstream fin;
		fin.open("machinecode.txt");
		char* line;
		string temp;
		if (fin.is_open())
		{
			while (!fin.eof())
			{
				line = new char[100];
				fin.getline(line, 100);

				temp = line;
				if (line != NULL)
				{
					int colon = temp.find(':');
					string replace = truncate(temp, colon + 2);
					lines.push_back(replace);
				}
			}
		}
	}

	void displayLines()
	{
		for (int i = 0; i < lines.size(); i++)
		{
			cout << lines[i] << endl;
		}
	}

	//Stores value of Int
	void storeInt(string name, int value)
	{
		int adrs = SymbolTable.at(name).address;
		defined[adrs] = true;
		data[adrs] = value;
	}


	//Gets value of variable from Symbol Table
	int getInt(string name)
	{
		int adrs = SymbolTable.at(name).address;

		if (defined[adrs] == true)
		{
			return data[adrs];
		}
		else
		{
			PrintError2("Variable " + name + "is not initialized");
		}
		return -1;
	}


	//Printing 
	void OUT(string parameter)
	{
		int check = parameter.find("\'");
		int check2 = parameter.find("\"");

		if (check != -1)
		{
			string temp = remove_commas(parameter);
			cout << temp;
		}
		else if (check2 != -1)
		{
			string temp = remove_commas(parameter);
			cout << temp;
		}
		else if (parameter.find("\\N") != -1)
		{
			cout << endl;
		}
		else
		{
			Symbol var = SymbolTable.at(parameter);
			if (var.type.compare("int") == 0)
			{
				int temp = getInt(parameter);
				cout << temp;
			}
			else if (var.type.compare("char") == 0)
			{
				char temp = getInt(parameter);
				cout << temp;
			}
		}
	}


	//Inputs
	void IN(string variable)
	{
		int value;
		cin >> value;
		storeInt(variable, value);
	}

	int GOTO(string number)
	{
		int no = stoi(number);
		return no;
	}

	bool check_operation(string str)
	{
		if (str.find("+") != -1)
			return true;
		if (str.find("-") != -1)
			return true;
		if (str.find("*") != -1)
			return true;
		if (str.find("/") != -1)
			return true;
		return false;
	}

	bool check_digits(string str)
	{
		bool flag = false;

		for (int i = 0; i < 10; i++)
		{
			if (str.at(0) == '0' + i)
			{
				flag = true;
			}
		}
		return flag;
	}

	//Checking constant char values
	bool check_char(string str)
	{
		if (str.at(0) == '\'')
		{
			return true;
		}
		return false;
	}

	//Removes inverted commas
	string remove_commas(string str)
	{
		string temp = "";
		char a = NULL;
		for (int i = 0; i < str.size(); i++)
		{
			a = str.at(i);
			if (a != '\'' && a != '\"')
			{
				temp += str.at(i);
			}
		}
		return temp;
	}


	bool checkop(char sign)
	{
		if (sign == '+')
			return true;
		if (sign == '-')
			return true;
		if (sign == '*')
			return true;
		if (sign == '/')
			return true;
		if (sign == '>')
			return true;
		if (sign == '<')
			return true;
		if (sign == '=')
			return true;
		if (sign == '!')
			return true;
		return false;
	}

	bool doubleop(char sign)
	{
		if (sign == '>')
			return true;
		if (sign == '<')
			return true;
		if (sign == '=')
			return true;
		if (sign == '!')
			return true;
		return false;
	}

	int findop(string str)
	{
		int count = 0;
		bool flag = true;
		int i = 0;

		while (flag)
		{
			if (checkop(str.at(i)))
			{
				flag = false;
			}
			i++;
			count++;
		}
		return count;
	}

	//Helps in calculation of two variables
	Calculation calculation(string str)
	{
		Calculation cal;

		int tilop = findop(str) - 1;
		cal.value = truncate_back(str, tilop);      //First variable
		string opcode;
		opcode += str.at(tilop);                   //Operation
		if (doubleop(str.at(tilop)))              //Check if relational operator
		{
			if (str.at(tilop + 1) == '=')           //Checks for >= <= != ==
			{
				opcode += str.at(tilop + 1);
				tilop++;
			}
			cal.op = opcode;
			cal.value2 = truncate(str, tilop + 1);     //Second Value
		}
		else
		{
			cal.op = opcode;
			cal.value2 = truncate(str, tilop + 1);
		}

		return cal;
	}


	int calculate(Calculation cal)
	{
		int val1 = getInt(cal.value);
		int val2 = getInt(cal.value2);
		if (cal.op.compare("+") == 0)
			return val1 + val2;
		else if (cal.op.compare("-") == 0)
			return val1 - val2;
		else if (cal.op.compare("*") == 0)
			return val1 * val2;
		else
			return val1 / val2;
	}

	//Assignments
	void Assignment(string variable, string rightside)
	{
		int check = rightside.find('=');
		string rest = truncate(rightside, check + 1);

		if (!check_operation(rest))
		{
			if (check_digits(rest))
			{
				storeInt(variable, stoi(rest));
			}
			else if (check_char(rest))
			{
				string temp2 = remove_commas(rest);
				int temp = temp2.at(0);
				storeInt(variable, temp);
			}
			else
			{
				storeInt(variable, getInt(rest));
			}

		}
		else
		{
			int value = calculate(calculation(rest));
			storeInt(variable, value);
		}
	}


	bool condition(Calculation cal)
	{
		int val1 = getInt(cal.value);
		int val2 = 0;
		if (cal.value2[0] >= '0' && cal.value2[0] <= '9')
		{
			val2 = stoi(cal.value2);
		}
		else
		{
			val2 = getInt(cal.value2);
		}
		if (cal.op.compare(">") == 0)
			return val1 > val2;
		else if (cal.op.compare("<") == 0)
			return val1 < val2;
		else if (cal.op.compare(">=") == 0)
			return val1 >= val2;
		else if (cal.op.compare("<=") == 0)
			return val1 <= val2;
		else if (cal.op.compare("==") == 0)
			return val1 == val2;
		else
			return val1 != val2;
	}

	//Get string till index s
	string truncate_back(string str, int s)
	{
		string temp = "";
		if (str != "")
		{
			for (int i = 0; i < s; i++)
			{
				temp += str[i];
			}
		}
		return temp;
	}

	int IF(string str)
	{
		int exp = str.find("GOTO") - 1;
		string expression = truncate_back(str, exp);        //condition a<c
		int line = str.find("GOTO") + 5;
		string gotoline = truncate(str, line);              //GOTO line number

		bool flag = condition(calculation(expression));

		if (flag)
		{
			return stoi(gotoline);                           //Returning line number if true
		}
		return currentLine + 1;                             //Returning line number if false;
	}

	void execute()
	{
		while (currentLine < lines.size())
		{
			string line = lines[currentLine];
			if (line == "")
			{
				currentLine = lines.size() + 3;
				break;
			}
			int space = line.find(" ");
			string check = "";
			string rest = "";

			if (space != -1)
			{
				check = truncate_back(line, space);
				rest = truncate(line, space + 1);
			}

			if (check.compare("OUT") == 0)
			{
				OUT(rest);
				currentLine++;
			}
			else if (check.compare("IN") == 0)
			{
				IN(rest);
				currentLine++;
			}
			else if (check.compare("GOTO") == 0)
			{
				currentLine = GOTO(rest);
			}
			else if (check.compare("IF") == 0)
			{
				currentLine = IF(rest);
			}
			else
			{
				int find = line.find('=');
				string var = truncate_back(line, find);
				rest = truncate(line, find);
				Assignment(var, rest);
				currentLine++;
			}

		}
	}

};