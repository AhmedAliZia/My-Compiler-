#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "Symbol.h"
#include "VM.h"
using namespace std;


string token;
string lexeme;
string prev_token;
string prev_lexeme;
int tabs;                         //Number of tabs to print
ifstream reader;
string datatype;                 
map<string, string> table;       
string expression;
bool matched_before = false;
bool checked = false;
pair<string, string> look_ahead;  //Look ahead for Arithmetic Operations
bool T_flag;
map<string, Symbol> SymbolTable;  //The Symbol Table
int address = 0;
vector<string> lines;             //Stores the Machine Code before writing it to file
int line_count = 0;
int temp_count = 0;
ofstream writer;


//Helper struct for Arithmetic Function as it returns 2 values
struct bring
{
	bool flag = false;
	string temp = "";
};

//Updates the Symbol Table 
void updateSymbolTable(string name, string type, int& loc)
{
	Symbol temp(type, loc);
	SymbolTable.insert({ name,temp });
	loc++;
}


//Require recursion
bring Arithmetic();
bool T(string& str);
bool INITIALIZATION();
bool R2(string& str);
bool R1(string& str);
bool DATATYPE();
bool EXPRESSION(stack<string>&, stack<string>&);
bool TERM2(stack<string>&, stack<string>&);
bool STATEMENT();

//Print Error and terminate Program
void PrintError(string str)
{
	cout << str << endl;
	abort();
}

//Print tabs
void print(string str)
{
	for (int i = 0; i < tabs; i++)
	{
		cout << '\t';
	}
	cout << str << endl;
}

//Reads Tokens from Output.txt File
void ReadFile(ifstream& fin)
{
	fin.open("output.txt");
	if (!fin.is_open())
	{
		cout << "File not Open" << endl;
		exit(0);
	}
	//opening file to write machine code to
	writer.open("machinecode.txt");

}

void display(const char* str)
{
	if (str != nullptr)
	{
		for (int i = 0; str[i] != '\0'; i++)
		{
			cout << str[i];
		}
	}
	cout << endl;
}

//Gets next line from the Output.txt file
char* nextLine(ifstream& fin)
{
	char* line = NULL;
	if (!fin.eof())
	{
		line = new char[100];
		fin.getline(line, 100);
	}
	return line;
}

//Seperates and returns Tokens and Lexeme from the line
bool getToken(const char* line)
{
	if (line != nullptr)
	{
		string token_temp = "";
		string lexeme_temp = "";
		int i = 1;

		//Reading Token (till the comma)
		for (i; line[i] != ','; i++)
		{
			token_temp.push_back(line[i]);
		}

		//Removing unnecessary spaces
		if (token_temp.back() == ' ')
		{
			token_temp.pop_back();
		}
		i++;

		//Not removing spaces for Constant Parameters of print and println
		if (token_temp.compare("CONSTANT") != 0)
		{
			for (i; line[i] != '\0'; i++)
			{
				if (line[i] != ' ')
				{
					lexeme_temp.push_back(line[i]);
				}
			}
		}
		else
		{
			if (line[i] == ' ')
			{
				i++;
			}
			for (i; line[i] != '\0'; i++)
			{
				lexeme_temp.push_back(line[i]);
			}
		}
		lexeme_temp.pop_back();
		token = token_temp;
		lexeme = lexeme_temp;
		return true;
	}
	else
	{
		return false;
	}
}

//retunrs New Tokens
bool nextToken()
{
	if (getToken(nextLine(reader)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//For maintaining count and making Temporary variables for Machine Code
string newTemp()
{
	string newtemp = "T" + to_string(temp_count);
	temp_count++;
	return newtemp;
}

//returns equation in form of : temp = a opcode b
string FormEquation(string temp, string a, string b, string opcode)
{
	string ez = temp + "=" + a + opcode + b;
	return ez;
}

//Writing Machine Code
void quad(string str)
{
	string line = to_string(line_count) + ": " + str; //Adding Line number in front of every command
	lines.push_back(line);
	line_count++;
}

//Writes to machinecode.txt
void writeToFile()
{
	for (int i = 0; i < lines.size(); i++)
	{
		writer << lines[i] << endl;
	}
}

//Matches Token and gets a new one
bool Match(string str)
{
	if (str == "" || token.compare(str) != 0)
		return false;
	prev_token = token;
	prev_lexeme = lexeme;
	nextToken();
	return true;
}

//Matches Look ahead token
bool Match3(string str)
{
	if (str == "" || look_ahead.first.compare(str) != 0)
		return false;

	return true;

}


//Part of Arithmetic CFG : checks for ID| const | (E)
bool ENTITY(stack<string>& variables, stack<string>& opcodes)
{
	print("ENTITY");
	tabs++;
	bool flag;

	if (Match("IDENTIFIER"))
	{
		variables.push(prev_lexeme);        //Pushing id into stack
		print(prev_lexeme);
		tabs--;
		return true;
	}

	else if (Match("CONSTANT"))
	{
		variables.push(prev_lexeme);        //pushing const into stack
		print(prev_lexeme);
		tabs--;
		return true;
	}

	//Checking for another Arithmetic Function if Brackets (E)
	else if (Match("PARANTHESIS"))
	{
		print(prev_lexeme);
		bring value;              //returns bool and a string
		value = Arithmetic();     //Arithmetic function inside Paranthesis
		if (!value.temp.empty())
		{
			variables.push(value.temp);   //storing answer of Arithmetic Function inside paranthesis in stack
		}
		flag = value.flag;
		if (flag)
		{
			if (Match("PARANTHESIS"))
			{
				print(prev_lexeme);
			}
			else
			{
				PrintError("Expected a )");
			}
		}
		else
		{
			PrintError("Expected an expression");
		}
		tabs--;
		return flag;
	}

	else
	{
		PrintError("Bad Token");
		return false;
	}

	return flag;

}

//Helper Funtion for Entity used in Indirect calls 
bool ENTITY2(stack<string>& variables, stack<string>& opcodes)
{
	bool flag = false;
	if (Match3("PARANTHESIS"))
	{
		print(look_ahead.second);
		bring value;
		value = Arithmetic();
		if (!value.temp.empty())
		{
			variables.push(value.temp);
		}
		flag = value.flag;
		if (flag)
		{
			if (Match("PARANTHESIS"))
			{
				print(prev_lexeme);
			}
			else
			{
				PrintError("Expected a )");
			}
		}
		else
		{
			PrintError("Expected an expression");
		}
		tabs--;

	}
	return flag;
}

//Part of Arithmetic CFG TERM -> ENTITY TERM2
bool TERM(stack<string>& variables, stack<string>& opcodes)
{
	print("TERM");
	tabs++;
	bool flag = ENTITY(variables, opcodes);
	bool flag2 = TERM2(variables, opcodes);
	tabs--;
	return flag || flag2;
}

//Checks if Token is Identifier or Not
bool IDENTIFIER2()
{
	bool flag = false;
	if (Match("IDENTIFIER"))
	{
		if (table.find(prev_lexeme) == table.end())
		{
			PrintError("Variable is not Declared");
		}
		print(prev_lexeme);
		flag = true;
	}
	else
	{
		flag = false;
	}
	return flag;
}

//Part of PRINTING CFG checks if Token matches print
bool PRINT()
{
	bool flag = false;
	print("PRINT");
	++tabs;
	if (Match("IO"))
	{
		print(prev_lexeme);
		flag = true;
	}
	--tabs;
	return flag;
}

//Checks if Token is constant
bool CONSTANT()
{
	bool flag = false;
	/*print("CONSTANT");*/
	if (Match("CONSTANT"))
	{
		print(prev_lexeme);
		flag = true;
	}
	else
	{
		flag = false;
	}
	return flag;
}


//Check Parameters whether ID or CONST
bool PARAMETER()
{
	bool flag1 = IDENTIFIER2();
	bool flag2 = CONSTANT();
	return flag1 || flag2;
}

//For Making Parse tree of Printing 
bool PRINTING()
{

	string str = "";
	bool check_endl = false;
	print("PRINTING");
	++tabs;
	//Match Printing Token
	if (!PRINT())
	{
		return false;
	}
	if (prev_lexeme.compare("println") == 0)
	{
		check_endl = true;            //Checking for println to add endl line to Machine Code
	}
	str = "OUT ";
	if (Match("PARANTHESIS"))
	{
		print(prev_lexeme);

		if (!PARAMETER())
		{
			PrintError("Expected a value for Printing");
		}
		str += prev_lexeme;

		if (Match("PARANTHESIS"))
		{
			print(prev_lexeme);

		}
		else
		{
			PrintError("Expected a Paranthesis");
		}

		if (Match("SEPERATOR"))
		{
			print(prev_lexeme);
		}
		else
		{
			PrintError("Expected a ;");
		}
		--tabs;
		quad(str);
		str = "";
		if (check_endl)       //Checking whether endl should be printed or not
		{
			str = "OUT \\N";
			quad(str);
		}
		return true;

	}
	else
	{
		PrintError("Expected: missing a Paranthesis");
		return false;
	}

}

//For Parse Tree of Inputs
bool INPUT()
{
	string str = "";
	bool flag = false;
	print("INPUT");
	++tabs;
	str = "IN ";
	if (Match("IO"))
	{
		print(prev_lexeme);
		if (Match("ASSIGNMENT"))
		{
			print(prev_lexeme);
		}
		else
		{
			PrintError("Expected a ->");
		}

		if (!IDENTIFIER2())
		{
			PrintError("Expected a Identifier");
		}

		str += prev_lexeme;
		if (Match("SEPERATOR"))
		{
			print(prev_lexeme);
		}
		else
		{
			PrintError("Expected a ;");
		}

		quad(str);
		flag = true;
	}
	else
	{
		PrintError("Expected input");
		flag = false;
	}
	--tabs;
	return flag;
}

//Match Token but Donot get Next Token
bool Match2(string str)
{
	if (str == "" || token.compare(str) != 0)
		return false;

	return true;
}


//Helper Function for Arithmetic 
string Arithmetic2(stack<string>& variables, stack<string>& opcodes)
{
	string baka = "";
	EXPRESSION(variables, opcodes);
	if (!variables.empty())
	{
		baka = variables.top();
	}
	variables.pop();
	return baka;
}

//Checks for Arithmetic Operations
bool CheckIfArithmetic()
{

	look_ahead = { token, lexeme };
	nextToken();
	if (Match2("ARITHMETIC"))
	{
		return true;
	}
	return false;

}

//Parse tree for Assignment 
bool ASSIGNMENT()
{
	print("ASSIGNMENT");
	++tabs;

	string str;
	stack<string> variables;  //stores variables for Arithmetic function
	stack<string> opcodes;    //stores operation signs for Arithmetic Functions

	if (!Match3("IDENTIFIER"))
	{
		return false;
	}
	print(prev_lexeme);
	str += prev_lexeme;
	if (Match("ASSIGNMENT"))
	{
		print(prev_lexeme);
		str += prev_lexeme;
		if (!CheckIfArithmetic())
		{
			if (Match3("IDENTIFIER") || Match3("CONSTANT"))
			{
				
				if (!Match3("CONSTANT"))
				{
					str += look_ahead.second;
					print(look_ahead.second);
					tabs--;
					quad(str);
					str = "";
					return true;
				}
				else
				{
					//Converting numeric literals to temporary variables
					string temp = newTemp();
					str += temp;
					updateSymbolTable(temp, "int", address);
					quad(temp + "=" + look_ahead.second);
					print(look_ahead.second);
					tabs--;
					quad(str);
					str = "";
					return true;
				}
			}
			else
			{
				print("ARITHMETIC");
				tabs++;
				print("TERM");
				tabs++;
				ENTITY2(variables, opcodes);
				TERM2(variables, opcodes);
				tabs--;
				string temp = Arithmetic2(variables, opcodes);
				str += temp;
				if (Match("SEPERATOR"))
				{
					quad(str);
					str = "";
					print(prev_lexeme);
					--tabs;
					return true;
				}
				else
				{
					PrintError("Expected a ;");
					return false;
				}
			}
		}
		else
		{
			print("ARITHMETIC");
			tabs++;
			print("TERM");
			tabs++;
			if (look_ahead.first.compare("PARATHESIS") != 0)
			{
				print("ENTITY");
				tabs++;
				print(look_ahead.second);
				variables.push(look_ahead.second);
				tabs--;
			}
			else
			{
				ENTITY(variables, opcodes);
			}
			TERM2(variables, opcodes);
			tabs--;
			string temp = Arithmetic2(variables, opcodes);
			str += temp;
			if (Match("SEPERATOR"))
			{
				quad(str);
				str = "";
				print(prev_lexeme);
				--tabs;
				return true;
			}
			else
			{
				PrintError("Expected a ;");
				return false;
			}
			return true;
		}
	}
	else
	{
		return false;
	}
}


//Parse tree for Increments
bool INCREMENT()
{
	string a = "";
	string b = "1";
	string temp = "";
	string op = "+";

	if (Match2("IDENTIFIER"))
	{
		look_ahead = { token, lexeme };
		nextToken();
		prev_token = look_ahead.first;
		prev_lexeme = look_ahead.second;
		if (Match("INCREMENT"))
		{
			print(look_ahead.second);
			temp = newTemp();
			updateSymbolTable(temp, "int", address);
			a += look_ahead.second;
			print("INCREMENT");
			print(prev_lexeme);
			if (Match("SEPERATOR"))
			{
				//Seperating increment into Three Address Code
				/*
				* c++;
				* T0 = c +1
				* c = T0
				*/
				string forconstant = newTemp();
				updateSymbolTable(forconstant, "int", address);
				quad(forconstant + "=" + b);
				print(prev_lexeme);
				quad(FormEquation(temp, a, forconstant, op));
				a += "=" + temp;
				quad(a);
				a = "";

			}
			else
			{
				PrintError("Expected a ;");
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//Parse Tree For Relations
bool RELATIONAL(string& str)
{
	print("RELATIONAL");
	++tabs;

	print(prev_lexeme);

	if (Match("RELATIONAL"))
	{
		print(prev_lexeme);
		str += prev_lexeme;
		if (!PARAMETER())
		{
			PrintError("Expected a value");
		}
		str += prev_lexeme;
		return true;
	}
	else
	{
		return false;
	}
}

//Parse Tree for Comments
bool COMMENT()
{
	//Ignores everything just returns New Token
	if (Match("COMMENT"))
	{
		return true;
	}
	return false;
}

//Parse tree for Conditions of loops and flow statements
bool CONDITION(string& str)
{
	if (Match("IDENTIFIER"))
	{
		str += prev_lexeme;
		if (!RELATIONAL(str))
		{
			PrintError("Expected a condition");
		}
		return true;
	}
	return false;
}

//Helper Function for Block
bool MULTISTATEMENT()
{
	print("MULTISTATEMENTS");
	++tabs;

	bool flag = STATEMENT();

	if (flag)
	{
		--tabs;
		MULTISTATEMENT();
	}
	--tabs;
	return flag;
}


//Parse Tree for Statements of Loop and Flow Blocks
void BLOCK()
{
	print("BLOCK");
	++tabs;

	if (Match("PARANTHESIS"))
	{
		print(prev_lexeme);
		++tabs;
		if (!MULTISTATEMENT())
			PrintError("Expected a statement");
		--tabs;

		if (!Match("PARANTHESIS"))
			PrintError("Expected a )");
		print(prev_lexeme);
	}
	--tabs;
}

//Part of Decision CFG checks for conditions
int P()
{
	print("P");
	++tabs;
	string str;

	int condition;  //stores the line number of the Condition line
	int backpatch;  //stores number for Backpatching
	int nextLine;   //number for next line after block

	str = "IF ";
	if (CONDITION(str))
	{
		if (Match("SEPERATOR"))
		{
			condition = line_count;
			str += " GOTO " + to_string(line_count + 2);   //Skipping GOTO for false statements 
			quad(str);
			backpatch = line_count;
			quad("GOTO ");                                 //GOTO for False conditions
			print(prev_lexeme);
			BLOCK();
			nextLine = line_count;
			quad("GOTO ");                                //GOTO for moving to next else or elif
			string replace = lines.at(backpatch);
			replace += to_string(line_count);
			lines.at(backpatch) = replace;   
			--tabs;
			return nextLine;
		}
		else
		{
			PrintError("Expected a : ");
		}

	}
	else
	{
		PrintError("Expected a condition");
	}
	return 0;
}

//Part of DECISION CFG checks for if statements
int L()
{
	print("L");
	int nextLine;
	++tabs;
	if (Match("DECISION"))
	{
		if (prev_lexeme.compare("if") != 0)
		{
			PrintError("Expected an if");
			return 0;
		}
		print(prev_lexeme);
		nextLine = P();                        //Line immediately after if block
		--tabs;
		return nextLine;
	}
	return 0;
}

//Part of DECISION CFG checks for elif conditions
bool E(vector<int> &lineno)       //lineno for storing line numbers if multiple elifs
{
	print("E");
	++tabs;

	if (lexeme.compare("elif") != 0)
	{
		print("^");
		--tabs;
		return false;
	}
	if (Match("DECISION"))
	{
		print(prev_lexeme);
		lineno.push_back(P());
		E(lineno);
		--tabs;
		return true;
	}
	return false;
}


//Part of DECISION CFG checks block of the conditions
int T()
{
	print("T");
	++tabs;
	int endLine;
	if (lexeme.compare("else") != 0)
	{
		print("^");
		--tabs;
		endLine = line_count;
		return endLine;
	}
	if (Match("DECISION"))
	{
		print(prev_lexeme);
		BLOCK();
		endLine = line_count;
		--tabs;
		return endLine;
	}
	return 0;
}

//Parse Tree for FLOW STATEMENTS
bool DECISIONS()
{
	print("DECISION");
	++tabs;
	int if_return_line;
	vector<int> numbers;
	int endLine;
	if_return_line = L();                    //Line no after if statement
	E(numbers);                              //Line nos after all elifs
	endLine = T();                           //Line no after else

	//replacing line nos according to number of elifs and else statements
	string replace =to_string(endLine);
	lines[if_return_line] += replace;
	for (int i = 0; i < numbers.size(); i++)
	{
		lines[numbers[i]] += replace;
	}
	return true;
}


//Parse Tree for Looping Statements
bool LOOP()
{
	print("LOOP");
	++tabs;

	string str = "";

	int condition = 0;            //line no of looping condition
	int backpatch = 0;            //line no of escaping loop

	if (Match("LOOPING STATEMENT"))
	{
		print(prev_lexeme);
		str = "IF ";
		if (CONDITION(str))
		{
			if (Match("SEPERATOR"))
			{
				condition = line_count;
				str += " GOTO " + to_string(line_count + 2);
				quad(str);
				backpatch = line_count;
				quad("GOTO ");
				print(prev_lexeme);
				BLOCK();
				quad("GOTO " + to_string(condition));
				string replace = lines.at(backpatch);
				replace += to_string(line_count);
				lines.at(backpatch) = replace;
				--tabs;
				return true;
			}
			else
			{
				PrintError("Expected a : ");
			}

		}
		else
		{
			PrintError("Expected a condition");
		}
	}
	--tabs;
	return false;
}


//Generates Machine code by recieving variables and opcodes
string GenerateArithmeticMachineCode(stack<string>& variables, stack<string>& opcodes)
{
	string a;
	string b;
	string temp;
	string op;
	while (!opcodes.empty())
	{
		//If stack contains 2 or more variables create temporary variable and store operation in it
		a = variables.top();
		variables.pop();
		if (!variables.empty())
		{
			b = variables.top();
			variables.pop();

			temp = newTemp();
			updateSymbolTable(temp, "int", address);

			op = opcodes.top();
			opcodes.pop();

			quad(FormEquation(temp, b, a, op));

			variables.push(temp);
		}

	}

	return "";
}


//For testing
void Check_Statement()
{
	ReadFile(reader);
	nextToken();
	{
		if (!INITIALIZATION())
		{
			PrintError("Ma name a jeff");
		}
	}

}

/*
* -----------------------------------------------------------------------------------------------------------------------------------------------
*													MAIN
* -----------------------------------------------------------------------------------------------------------------------------------------------
*/



int main()
{
	/*Lexer lex;
	lex.driver();*/
	cout << "PARSE TREES";
	ReadFile(reader);    //Reading Tokens from Lexical Analyzer
	nextToken();         //Getting the first Token
	while (!reader.eof())
	{
		if (!STATEMENT())
		{

		}
	}
	cout << endl << endl << endl;
	cout << "<---------------------------------------------------------------------------------------------------------->" << endl;
	cout << endl;
	cout << "EXECUTING VIRTUAL MACHINE : " << endl << endl << endl;
	writeToFile();       //Writing Three Address Code to Machine.txt
	VM vm(SymbolTable);  //Initializing VM with symboltable
	vm.readMachineCode();//Readign Machine Code
	vm.execute();        //Executing Machine Code
	return 0;
}


bool STATEMENT()
{
	print("STATEMENT");
	bool flag = false;
	if (token == "")
	{
		print("^");
		return false;
	}
	else if (token.compare("DATATYPE") == 0)
	{
		++tabs;
		flag = INITIALIZATION();
		--tabs;
	}
	else if (token.compare(("IDENTIFIER")) == 0)
	{
		++tabs;
		flag = INCREMENT();
		if (!flag)
		{
			flag = ASSIGNMENT();
		}
		--tabs;
	}
	else if (token.compare("COMMENT") == 0)
	{
		flag = COMMENT();
	}
	else if (lexeme.compare("input") == 0)
	{
		++tabs;
		flag = INPUT();
		--tabs;
	}
	else if (lexeme.compare("print") == 0 || lexeme.compare("println") == 0)
	{
		++tabs;
		flag = PRINTING();
		--tabs;
	}
	else if (token.compare("LOOPING STATEMENT") == 0)
	{
		++tabs;
		flag = LOOP();
		--tabs;
	}
	else if (token.compare("DECISION") == 0)
	{
		++tabs;
		flag = DECISIONS();
		--tabs;
	}
	else
	{
		flag = Match("SEPERATOR");
		print("^");
	}
	return flag;
}

//Part of Arithmetic CFG
bool TERM2(stack<string>& variables, stack<string>& opcodes)
{
	print("TERM2");
	tabs++;

	bool flag = false;
	bool flag2 = false;


	if (Match("ARITHMETIC"))
	{
		matched_before = true;
	}

	if (prev_lexeme.compare("*") == 0)
	{
		checked = true;
		opcodes.push(prev_lexeme);
		print(prev_lexeme);
		flag = ENTITY(variables, opcodes);
		flag2 = TERM2(variables, opcodes);
		tabs--;
		return flag || flag2;
	}
	else if (prev_lexeme.compare("/") == 0)
	{
		checked = true;
		opcodes.push(prev_lexeme);
		print(prev_lexeme);
		flag = ENTITY(variables, opcodes);
		flag2 = TERM2(variables, opcodes);
		tabs--;
		return flag || flag2;
	}
	else
	{
		print("^");
		tabs--;
		return false;
	}

}

//Part of Arithmetic CFG
bool EXPRESSION(stack<string>& variables, stack<string>& opcodes)
{
	print("EXPRESSION");
	tabs++;
	bool flag = false;
	bool flag2 = false;


	if (matched_before && !checked)
	{
		if (prev_token.compare("ARITHMETIC") != 0)
		{
			PrintError("Expected an Operator");
			return false;
		}
		else
		{
			matched_before = false;
			if (prev_lexeme.compare("+") == 0)
			{
				opcodes.push(prev_lexeme);
				print(prev_lexeme);
				flag = TERM(variables, opcodes);
				flag2 = EXPRESSION(variables, opcodes);
				tabs--;
				return flag || flag2;
			}

			else if (prev_lexeme.compare("-") == 0)
			{
				opcodes.push(prev_lexeme);
				print(prev_lexeme);
				flag = TERM(variables, opcodes);
				flag2 = EXPRESSION(variables, opcodes);
				tabs--;
				return flag || flag2;
			}


			else
			{
				string temp = GenerateArithmeticMachineCode(variables, opcodes);
				print("^");
				tabs--;
				return false;
			}
		}
	}
	else
	{
		if (checked)
		{
			if (prev_token.compare("ARITHMETIC") == 0)
			{
				if (prev_lexeme.compare("+") == 0)
				{
					opcodes.push(prev_lexeme);
					print(prev_lexeme);
					flag = TERM(variables, opcodes);
					flag2 = EXPRESSION(variables, opcodes);
					tabs--;
					return flag || flag2;
				}

				else if (prev_lexeme.compare("-") == 0)
				{
					opcodes.push(prev_lexeme);
					print(prev_lexeme);
					flag = TERM(variables, opcodes);
					flag2 = EXPRESSION(variables, opcodes);
					tabs--;
					return flag || flag2;
				}


				else
				{
					string temp = GenerateArithmeticMachineCode(variables, opcodes);
					print("^");
					tabs--;
					return true;
				}
			}
			else
			{
				string temp = GenerateArithmeticMachineCode(variables, opcodes);
				print("^");
				tabs--;
				return true;
			}
		}
		else
		{
			if (Match("ARITHMETIC"))
			{
				if (prev_lexeme.compare("+") == 0)
				{
					opcodes.push(prev_lexeme);
					print(prev_lexeme);
					flag = TERM(variables, opcodes);
					flag2 = EXPRESSION(variables, opcodes);
					tabs--;
					return flag || flag2;
				}

				else if (prev_lexeme.compare("-") == 0)
				{
					opcodes.push(prev_lexeme);
					print(prev_lexeme);
					flag = TERM(variables, opcodes);
					flag2 = EXPRESSION(variables, opcodes);
					tabs--;
					return flag || flag2;
				}


				else
				{
					string temp = GenerateArithmeticMachineCode(variables, opcodes);
					print("^");
					tabs--;
					return false;
				}
			}
			string temp = GenerateArithmeticMachineCode(variables, opcodes);
			print("^");
			tabs--;
			return false;
		}
	}
}


//Parse Tree for Arithmetic Operations
bring Arithmetic()
{
	print("ARITHMETIC");
	stack<string> variables;
	stack<string> opcodes;
	tabs++;
	expression = "";
	bool flag = TERM(variables, opcodes);
	bool flag2 = EXPRESSION(variables, opcodes);
	tabs--;
	bring value;
	value.flag = flag || flag2;
	value.temp = variables.top();
	variables.pop();
	return value;
}

bool T(string& str)
{
	print("T");
	tabs++;

	if (Match("SEPERATOR"))
	{
		if (prev_lexeme.compare(",") == 0)
		{
			print(prev_lexeme);

			R1(str);
			R2(str);
			T_flag = T(str);
			tabs--;
			return T_flag;
		}
		else
		{
			print("^");
			tabs--;
			return false;
		}
	}
	else
	{
		print("^");
		tabs--;
		return false;
	}
}

bool DATATYPE()
{
	print("DATATYPE");
	tabs++;

	if (Match("DATATYPE"))
	{
		datatype = prev_lexeme;
		print(prev_lexeme);
		tabs--;
		return true;
	}
	else
	{
		tabs--;
		return false;
	}
}


bool R1(string& str)
{
	print("R1");
	tabs++;

	if (Match("IDENTIFIER"))
	{
		print(prev_lexeme);
		if (table.find(prev_lexeme) != table.end())
		{
			PrintError("Variable Already Defined");

		}

		if (prev_lexeme.compare("int") == 0 || prev_lexeme.compare("char") == 0)
		{
			PrintError(" Cannot use keywords to declare variable names ");
		}

		str += prev_lexeme;
		table.insert({ prev_lexeme, datatype });
		updateSymbolTable(prev_lexeme, datatype, address);
		tabs--;
		return true;
	}
	else
	{
		PrintError("Expected an id");
		tabs--;
		return false;
	}
}


bool R2(string& str)
{
	print("R2");
	tabs++;

	stack<string> variables;
	stack<string> opcodes;


	if (Match("ASSIGNMENT"))
	{
		print(prev_lexeme);
		str += prev_lexeme;
		if (!CheckIfArithmetic())
		{
			if (!Match3("PARANTHESIS"))
			{
				if (Match3("IDENTIFIER") || Match3("CONSTANT"))
				{
					if (!Match3("CONSTANT"))
					{
						str += look_ahead.second;
						print(look_ahead.second);
						tabs--;
						quad(str);
						str = "";
						return true;
					}
					else
					{
						string temp = newTemp();
						str += temp;
						updateSymbolTable(temp, "int", address);
						quad(temp + "=" + look_ahead.second);
						print(look_ahead.second);
						tabs--;
						quad(str);
						str = "";
						return true;
					}
				}
				else
				{
					PrintError("Expected a value");
					tabs--;
					return false;
				}
			}
			else
			{
				print("ARITHMETIC");
				tabs++;
				print("TERM");
				tabs++;
				ENTITY2(variables, opcodes);
				TERM2(variables, opcodes);
				tabs--;
				string temp;
				temp = Arithmetic2(variables, opcodes);
				str += temp;
				quad(str);
				str = "";
				return true;
			}
		}
		else
		{
			print("ARITHMETIC");
			tabs++;
			print("TERM");
			tabs++;
			if (look_ahead.first.compare("PARATHESIS") != 0)
			{
				print("ENTITY");
				tabs++;
				print(look_ahead.second);
				variables.push(look_ahead.second);
				tabs--;
			}
			else
			{
				ENTITY(variables, opcodes);
			}
			TERM2(variables, opcodes);
			tabs--;
			string temp = Arithmetic2(variables, opcodes);
			str += temp;
			quad(str);
			str = "";
			return true;
		}
	}
	else
	{
		print("^");
		--tabs;
		return false;
	}
}




bool INITIALIZATION()
{
	print("INITIALIZATION");
	tabs++;

	string value = "";

	bool flag = DATATYPE();
	if (Match("SEPERATOR"))
	{
		print(prev_lexeme);
	}
	else
	{
		PrintError("Expected a :");
	}
	bool flag2 = R1(value);
	bool flag3 = R2(value);
	bool flag4 = T(value);
	if (flag4)
	{
		if (!Match("SEPERATOR"))
		{
			PrintError("Expected a ;");
		}
	}
	print(prev_lexeme);
	--tabs;
	return true;
}
