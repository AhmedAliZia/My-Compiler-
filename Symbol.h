#pragma once
#include <iostream>
#include <map>
#include <string>
using namespace std;


class Symbol
{
public:
	string type;
	int address;

	Symbol()
	{
		type = "";
		address = 0;
	}

	Symbol(string t, int adrs)
	{
		type = t;
		address = adrs;
	}
};
