#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <cstdio>
using namespace std;

//Enum for tokens
enum Tokens
{
	INT,
	CHAR,
	SEPERATOR,
	INTOUT,
	PARANTHESIS,
	ASSIGNMENT,
	RELATIONAL,
	OTHER

};

enum Rel
{
	GREATER,
	GTHAN,
	LESS,
	LESSTHAN,
	EQUAL,
	NOTHING
};

enum Decisions
{
	IF,
	ELSE,
	ELSEIF,
	NOT
};

enum Operations
{
	ADD,
	SUBTRACT,
	DIVIDE,
	MULTIPLY,
	INCREMENT,
	EZ
};

enum Assign
{
	EQUALTO,
	ARROW,
	NONE
};


enum Loops
{
	WHILE,
	FOR,
	NEITHER
};


//Compares vector and char arrays
bool compare(vector<char> temp, const char* arr)
{
	bool flag = true;
	if (temp.size() == 0)
	{
		return false;
	}
	if (temp.size() != strlen(arr) )
	{
		return false;
	}
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] != arr[i])
		{
			flag = false;
		}
	}
	return flag;
}


//checks for Arithemetic Operations
Operations checkArithmetic(vector<char> arr)
{
	if (arr.front() == '-')
	{
		return SUBTRACT;
	}
	else if (arr.front() == '/')
	{
		return DIVIDE;
	}
	else if (arr.front() == '*')
	{
		return MULTIPLY;
	}
	else if (arr.front() == '+')
	{
		if (arr[1] == '+')
		{
			return INCREMENT;
		}
		return ADD;
	}

	return EZ;
}


//checks for Relational Operators
Rel checkOperator(vector<char> arr)
{
	if (arr.front() == '>')
	{
		if (arr[1] == '=')
		{
			return GTHAN;
		}
		return GREATER;
	}

	if (arr.front() == '<')
	{
		if (arr[1] == '=')
		{
			return LESSTHAN;
		}
		return LESS;
	}

	if (arr.front() == '=')
	{
		if (arr[1] == '=')
		{
			return EQUAL;
		}
		return NOTHING;
	}

	return NOTHING;
}


//Checks for -> Assignment operator
Assign checkArrow(vector<char> arr)
{
	if (arr.front() == '-')
	{
		if (arr[1] == '>')
		{
			return ARROW;
		}
		return NONE;
	}
	return EQUALTO;
}


//Checks for Loops
Loops checkLoops(vector<char> arr)
{
	if (compare(arr, "while"))
	{
		return WHILE;
	}
	else if (compare(arr, "for"))
	{
		return FOR;
	}
	return NEITHER;
}


//Checks for if else
Decisions checkDecision(vector<char> arr)
{
	if (compare(arr, "if"))
	{
		return IF;
	}
	else if (compare(arr, "elif"))
	{
		return ELSEIF;
	}
	else if (compare(arr, "else"))
	{
		return ELSE;
	}
	return NOT;
}

//checks whether == or =
Tokens checkAssignment(vector<char> arr)
{
	if (arr.front() == '=' || arr.front() == '-')
	{
		if (arr[1] == '=')
		{
			return RELATIONAL;
		}
		return ASSIGNMENT;
	}
	return OTHER;
}


//checks for Paranthesis
Tokens checkParanthesis(char a)
{
	if (a == '(' || a == ')' || a == '{' || a == '}' || a == '[' || a == ']')
	{
		return PARANTHESIS;
	}
	return OTHER;
}


//checks for I/O statements
Tokens checkIO(vector<char> buffer)
{
	if (compare(buffer, "print") || compare(buffer, "println") || compare(buffer, "input"))
	{
		return INTOUT;
	}

	return OTHER;
}


//checks for Separators
Tokens checkSeperator(char a)
{
	if ((a == ',' || a == ';' || a == ':'))
	{
		return SEPERATOR;
	}

	return OTHER;
}


//clears all '\0' from front of a vector array otherwise vector.empty() does not work
void clearVector(vector<char>& arr)
{
	if (!arr.empty())
	{
		if (arr.front() == '\0')
		{
			arr.clear();
		}
	}
}


//Pops front element of the vector along with spaces at front
void vectorPop(vector<char>& buffer)
{
	//list for poping
	list<char> baka;

	for (int i = 0; i < buffer.size(); i++)
	{
		baka.push_back(buffer[i]);
	}

	//poping all spaces
	baka.pop_front();
	while (baka.front() == ' ')
	{
		baka.pop_front();
	}

	buffer.clear();

	//Repopulating buffer vector without spaces and poped front 
	if (baka.size() != 0)
	{
		int size = baka.size();
		while (baka.size() != 0)
		{
			buffer.push_back(baka.front());
			baka.pop_front();
		}
	}
}

//Removes arr from the vector
void vectorSubraction(vector<char>& buffer, const char* arr)
{
	int count = 0;
	vector<char> temp = buffer;
	bool flag = true;
	int a = 0;

	//iterating count till arr
	while(flag)
	{
		if (temp[a] == arr[a] && temp[a] != '\0')
		{
			count++;
			a++;
		}
		else
		{
			flag = false;
		}
	}


	//Filling list with elements other than arr
	list<char> baka;

	for (int i = count; i < buffer.size(); i++)
	{
		baka.push_back(temp[i]);
	}

	for (int i = 0; i < baka.size(); i++)
	{
		char check = baka.front();
		if (check == ' ')
		{
			baka.pop_front();
		}
	}

	buffer.clear();

	//Repopulating buffer vector
	int size = baka.size();
	for (int i = 0; i < size; i++)
	{
		if (!baka.empty())
		{
			buffer.push_back(baka.front());
			baka.pop_front();
		}
	}


}


//Copies element of arr into vector
void vectorCopy(vector<char>& buffer, const char* arr)
{
	for (int i = 0; i < strlen(arr) + 1; i++)
	{
		buffer.push_back(arr[i]);
	}

}


bool compareStrings(const char* arr1, const char* arr2)
{
	bool flag = true;
	for (int i = 0; i < strlen(arr1) + 1; i++)
	{
		if (arr1[i] != arr2[i])
		{
			flag = false;
		}
	}
	return flag;
}

void display(vector<char> buffer, ofstream& out)
{
	for (int i = 0; i < buffer.size(); i++)
	{
		cout << buffer[i];
		out << buffer[i];
	}
}


bool isLetter(char a)
{
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
	{
		return true;
	}
	return false;
}

bool isDigit(char a)
{
	if ((a >= '0' && a <= '9'))
	{
		return true;
	}
	return false;
}


void Datatype(vector<char>& arr, ofstream& out)
{
	if (arr.size() != 0)
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (isLetter(arr[i]))
				{
					buffer.push_back(arr[i]);
				}
				else
				{
					state = 2;
					break;
				}
				break;

			case 2:
				if (compare(buffer, "int") || compare(buffer, "char"))
				{
					if (compare(buffer, "int"))
					{
						cout << "(DATATYPE , int)" << endl;
						out << "(DATATYPE , int)" << endl;
						vectorSubraction(arr, "int");
						flag = false;

					}
					else if(compare(buffer , "char"))
					{
						cout << "(DATATYPE , char)" << endl;
						out << "(DATATYPE , char)" << endl;
						vectorSubraction(arr, "char");
						flag = false;
					}
					break;
				}
				else
				{
					flag = false;
					break;
				}
				break;
			}
			i++;
		}
	}
}



void IO(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (isLetter(arr[i]))
				{
					buffer.push_back(arr[i]);
				}
				else
				{
					state = 2;
					break;
				}
				break;

			case 2:
				if (checkIO(buffer) == INTOUT)
				{
					if (compare(buffer, "print"))
					{
						cout << "(IO , print)" << endl;
						out << "(IO , print)" << endl;
						vectorSubraction(arr, "print");
						flag = false;

					}
					else if (compare(buffer, "println"))
					{
						cout << "(IO , println)" << endl;
						out << "(IO , println)" << endl;
						vectorSubraction(arr, "println");
						flag = false;
					}
					else
					{
						cout << "(IO , input)" << endl;
						out << "(IO , input)" << endl;
						vectorSubraction(arr, "input");
						flag = false;
					}
					break;
				}
				else
				{
					flag = false;
					break;
				}
				break;
			}
			i++;
		}
	}
}

void Paranthesis(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		bool flag = true;
		int i = 0;

		switch (state)
		{
		case 1:
			if (checkParanthesis(arr[i]) == PARANTHESIS)
			{
				cout << "(PARANTHESIS, " << arr[i] << ")" << endl;
				out << "(PARANTHESIS, " << arr[i] << ")" << endl;
				vectorPop(arr);
				state = 0;
			}
			break;
		}
	}
}

void Relational(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		Rel state = NOTHING;
		bool flag = true;
		int i = 0;

		switch (checkOperator(arr))
		{
		case GREATER:

			cout << "(RELATIONAL, " << arr[i] << ")" << endl;
			out << "(RELATIONAL, " << arr[i] << ")" << endl;
			vectorPop(arr);
			state = NOTHING;
			break;
		case GTHAN:
			cout << "(RELATIONAL, >=)" << endl;
			out << "(RELATIONAL, >=)" << endl;
			vectorSubraction(arr, ">=");
			state = NOTHING;
			break;
		case LESS:
			cout << "(RELATIONAL, " << arr[i] << ")" << endl;
			out << "(RELATIONAL, " << arr[i] << ")" << endl;
			vectorPop(arr);
			state = NOTHING;
			break;
		case LESSTHAN:
			cout << "(RELATIONAL, <=)" << endl;
			out << "(RELATIONAL, <=)" << endl;
			vectorSubraction(arr, "<=");
			state = NOTHING;
			break;

		case EQUAL:
			cout << "(RELATIONAL, ==)" << endl;
			out << "(RELATIONAL, ==)" << endl;
			vectorSubraction(arr, "==");
			state = NOTHING;
			break;
		}
	}
}

void Assignment(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		if (checkAssignment(arr) == ASSIGNMENT)
		{
			int state = 1;
			bool flag = true;
			int i = 0;

			switch (state)
			{
			case 1:
				if (checkArrow(arr) == ARROW)
				{
					cout << "(ASSIGNMENT, ->)" << endl;
					out << "(ASSIGNMENT, ->)" << endl;
					vectorSubraction(arr, "->");
					state = 0;
				}
				else if (checkArrow(arr) == EQUALTO)
				{
					cout << "(ASSIGNMENT, " << arr[i] << ")" << endl;
					out << "(ASSIGNMENT, " << arr[i] << ")" << endl;
					vectorPop(arr);
					state = 0;
				}
				break;
			}
		}
		else
		{
			Relational(arr, out);
		}
	}
}


void Arithmetic(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		Operations state = EZ;
		bool flag = true;
		int i = 0;

		switch (checkArithmetic(arr))
		{
		case ADD:
			cout << "(ARITHMETIC, " << arr[i] << ")" << endl;
			out << "(ARITHMETIC, " << arr[i] << ")" << endl;
			vectorPop(arr);
			state = EZ;
			break;
		case SUBTRACT:
			cout << "(ARITHMETIC, " << arr[i] << ")" << endl;
			out << "(ARITHMETIC, " << arr[i] << ")" << endl;
			vectorPop(arr);
			state = EZ;
			break;
		case DIVIDE:
			cout << "(ARITHMETIC, " << arr[i] << ")" << endl;
			out << "(ARITHMETIC, " << arr[i] << ")" << endl;
			vectorPop(arr);
			state = EZ;
			break;
		case MULTIPLY:
			cout << "(ARITHMETIC, " << arr[i] << ")" << endl;
			out << "(ARITHMETIC, " << arr[i] << ")" << endl;
			vectorPop(arr);
			state = EZ;
			break;

		case INCREMENT:
			cout << "(INCREMENT, " << "++" << ")" << endl;
			out << "(INCREMENT, " << "++" << ")" << endl;
			vectorSubraction(arr, "++");
			state = EZ;
			break;
		}
	}
}

void Identifiers(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		char* temp = nullptr;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (isLetter(arr[i]))
				{
					buffer.push_back(arr[i]);
					state = 2;
					break;
				}
				else
				{
					state = 4;
					break;
				}
				break;

			case 2:
				if (isLetter(arr[i]) || isDigit(arr[i]) || arr[i] == '_')
				{
					buffer.push_back(arr[i]);
					break;
				}
				else
				{
					state = 3;
					break;
				}
			case 3:
				//In case if the string is not an identifier 
				if (checkLoops(buffer) == NEITHER && checkIO(buffer) == OTHER && checkDecision(buffer) == NOT)
				{
					cout << "(IDENTIFIER, ";
					out << "(IDENTIFIER, ";
					display(buffer, out);
					cout << ")" << endl;
					out << ")" << endl;
					temp = new char[buffer.size() + 1];
					for (int i = 0; i < buffer.size(); i++)
					{
						temp[i] = buffer[i];
					}
					vectorSubraction(arr, temp);
					state = 4;
					break;
				}
				else
				{
					state = 4;
					break;
				}
			case 4:
				flag = false;
				break;
			}
			i++;
		}
	}
}

void ConstantStatement(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		char* temp = nullptr;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (arr[i] == '"' || arr[i] == '\'')
				{
					buffer.push_back(arr[i]);
					state = 2;
					break;
				}
				else if (isDigit(arr[i]))
				{
					buffer.push_back(arr[i]);
					state = 5;
				}
				else
				{
					state = 4;
				}
				break;
			case 2:
				if (arr[i] != '"' && arr[i] != '\'')
				{
					buffer.push_back(arr[i]);
					break;
				}
				else
				{
					i--;
					state = 3;
					break;
				}
			case 3:
				if (arr[i] != '\0')
				{
					buffer.push_back(arr[i]);
				}
				else
				{
					buffer.push_back(' ');
				}
				cout << "(CONSTANT, ";
				out << "(CONSTANT, ";
				display(buffer, out);
				cout << ")" << endl;
				out << ")" << endl;
				temp = new char[buffer.size() + 1];
				for (int i = 0; i < buffer.size(); i++)
				{
					temp[i] = buffer[i];
				}
				vectorSubraction(arr, temp);
				state = 4;
				break;
			case 4:
				flag = false;
				break;
			case 5:
				if (isDigit(arr[i]))
				{
					buffer.push_back(arr[i]);

				}
				else
				{
					state = 3;
				}
				break;
			}
			i++;
		}
	}
}

void Seperator(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		bool flag = true;
		char seperator;
		int i = 0;

		switch (state)
		{
		case 1:
			if (checkSeperator(arr[i]) == SEPERATOR)
			{
				cout << "(SEPERATOR, " << arr[i] << ")" << endl;
				out << "(SEPERATOR, " << arr[i] << ")" << endl;
				vectorPop(arr);

				state = 0;
			}
			break;
		}
	}
}

void LoopingSt(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (isLetter(arr[i]))
				{
					buffer.push_back(arr[i]);
				}
				else
				{
					state = 2;
					break;
				}
				break;

			case 2:
				if (compare(buffer, "while") || compare(buffer, "for"))
				{
					if (compare(buffer, "while"))
					{
						cout << "(LOOPING STATEMENT , while)" << endl;
						out << "(LOOPING STATEMENT , while)" << endl;
						vectorSubraction(arr, "while");
						flag = false;

					}
					else
					{
						cout << "(LOOPING STATEMENT , for)" << endl;
						out << "(LOOPING STATEMENT , for)" << endl;
						vectorSubraction(arr, "for");
						flag = false;
					}
					break;
				}
				else
				{
					flag = false;
					break;
				}
				break;
			}
			i++;
		}
	}

}

void Decision(vector<char>& arr, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		char* temp = nullptr;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (isLetter(arr[i]))
				{
					buffer.push_back(arr[i]);
				}
				else
				{
					state = 2;
					break;
				}
				break;

			case 2:
				if (checkDecision(buffer) != NOT)
				{
					cout << "(DECISION, ";
					out << "(DECISION, ";
					display(buffer, out);
					cout << ")" << endl;
					out << ")" << endl;
					temp = new char[buffer.size() + 1];
					for (int i = 0; i < buffer.size(); i++)
					{
						temp[i] = buffer[i];
					}
					vectorSubraction(arr, temp);
					flag = false;
					state = NOT;
					break;

				}
				else
				{
					flag = false;
					break;
				}
				break;
			}
			i++;
		}
	}
}

void Comment(vector<char>& arr, ifstream& fin, ofstream& out)
{
	if (!arr.empty())
	{
		int state = 1;
		vector<char> buffer;
		int a = 0;
		int i = 0;
		bool flag = true;
		char* temp = nullptr;
		while (flag)
		{
			switch (state)
			{
			case 1:
				if (arr[i] == '/')
				{
					state = 2;
					break;
				}
				else
				{
					state = 7;
					break;
				}
				break;

			case 2:
				if (arr[i] == '/') //for // type comments
				{
					state = 3;
					break;
				}
				else if (arr[i] == '*') //for /* type comments
				{
					state = 5;
					break;
				}
				else
				{
					state = 7;
					break;
				}
			case 3:
				if (arr[i] != '\n' && arr[i] != '\0') //for // read till end of line
				{
					buffer.push_back(arr[i]);
				}
				else
				{
					state = 6;
				}
				break;
			case 4:
				state = 5;
				break;
			case 5:
				if (i != arr.size())                          //in case if the /* is on multiple lines
				{
					if (arr[i] != '\0')
					{

						if (arr[i] != '*')
						{
							buffer.push_back(arr[i]);        //store current line to buffer
						}
						else
						{
							buffer.push_back(arr[i]);       //for reading the ending */
							buffer.push_back(arr[i + 1]);
							state = 6;
						}
					}
				}
				else
				{
					arr.clear();
					temp = new char[100];
					fin.getline(temp, 100);                   //read the file for further instructions
					vectorCopy(arr, temp);
					i = -1;
				}
				break;
			case 6:
				cout << "(COMMENT, ";
				out << "(COMMENT, ";
				display(buffer, out);
				cout << ")" << endl;
				out << ")" << endl;
				arr.clear();
				flag = false;
				state = 7;
				break;
			case 7:
				flag = false;
				break;
			}
			i++;
		}
	}
}




void wrapper(vector<char>& arr, ifstream& baka, ofstream& out)
{
	Datatype(arr, out);
	IO(arr, out);
	Seperator(arr, out);
	Assignment(arr, out);
	Paranthesis(arr, out);
	ConstantStatement(arr, out);
	Identifiers(arr, out);
	Comment(arr, baka, out);
	Relational(arr, out);
	Arithmetic(arr, out);
	LoopingSt(arr, out);
	Decision(arr, out);
}

void driver()
{
	
	char* buffer = new char[100];
	vector<char> arr;
	ifstream baka;
	ofstream out;
	baka.open("input.txt");
	out.open("output.txt");
	if (baka.is_open())
	{

		while (!baka.eof())
		{
			baka.getline(buffer, 100);
			vectorCopy(arr, buffer);

			while (!arr.empty())
			{
				wrapper(arr, baka, out);
				clearVector(arr);
			}


		}
	}
	baka.close();
	out.close();
}

int main()
{
	driver();
	return 0;
}