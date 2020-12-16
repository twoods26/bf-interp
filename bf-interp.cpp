#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;
void throwError(string);

int main()
{
	// system resources
	string fileName = "src.bf";
	uint32_t mem_size = 1048576; //2^20
	char *mem;
	char *code;
	int ptr;
	int ip;

	// checking the length of the source code
	ifstream file;
	file.open(fileName);
	int codeLength = 0;
	char c = file.get();
	while(c != EOF)
	{
		if(c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == '[' || c == ']')
		{
			codeLength++;
		}
		c = file.get();
	}

	// allocating memory to store the source code
	file.clear();
	file.seekg(0, ios_base::beg);
	code = (char*)malloc(codeLength * sizeof(char));
	
	// reading in the code and stripping non-BF chars
	c = file.get();
	int i = 0;
	while(c != EOF)
	{
		if(c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == '[' || c == ']')
		{
			*(code + i) = c;
			i++;
		}
		c = file.get();
	}

	// allocating and 0'ing the program memory
	mem = (char*)calloc(mem_size, sizeof(char));
	
	ptr = 0; // setting the program pointer to the file element
	ip = 0; // setting the IP to the first char in the code
	
	int bracketCount = 0; // used to count brackets when branching

	// main logic
	while(ip < codeLength)
	{
		switch(code[ip])
		{
			case '<':
				if(ip == 0)
				{
					throwError("mem outofrange (underflow)");
				}
				else
				{
					ptr--;
				}
				ip++;
				break;
			case '>':
				if(ip == 0)
				{
					throwError("mem outofrange (overflow)");
				}
				else
				{
					ptr++;
				}
				ip++;
				break;
			case '+':
				mem[ptr]++;
				ip++;
				break;
			case '-':
				mem[ptr]--;
				ip++;
				break;
			case '.':
				cout << (char)(mem[ptr]);
				ip++;
				break;
			case ',':
				cin.get(mem[ptr]);
				ip++;
				break;
			case '[':
				if(mem[ptr] == 0)
				{
					ip++;

					// finding the matching ']'
					while(code[ip] != ']' || bracketCount != 0)
					{
						if(code[ip] == '[')
						{
							bracketCount++;
						}
						else if(code[ip] == ']')
						{
							bracketCount--;
						}
						ip++;
					}
					ip++; // moving past the bracket
				}
				else
				{
					ip++;
				}
				break;
			case ']':
				if(mem[ptr] != 0)
				{
					ip--;

					// finding the matching '['
					while(code[ip] != '[' || bracketCount != 0)
					{
						if(code[ip] == ']')
						{
							bracketCount++;
						}
						else if(code[ip] == '[')
						{
							bracketCount--;
						}
						ip--;
					}
					ip++; // moving below the bracket
				}
				else
				{
					ip++;
				}
		}
	}

	return 0;
}

void throwError(string msg)
{
	cout << "[error] " << msg << endl;
}
