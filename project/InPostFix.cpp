#include "InPostFix.h"
#include <iostream>
#include <stack>
#include <string>
#include <istream>
#include <sstream>
#include <fstream>

using namespace std;

InPostFix::InPostFix(string line, char function)
{
	iserrormsg = false;

	switch (function)
	{
	case '1' :
		IntoPostFix(line);
		break;
	case '2':
		PosttoInFix(line);
		break;
	}
}

void InPostFix::IntoPostFix(string line)
{
	ostringstream stream;
	bool prevSymb;
	bool prevOprnd;
	prevSymb = false;
	prevOprnd = false;
	iserrormsg = false;
	string temp;

	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == ' ') continue;
		else if(line[i] == '=')
		{
			while (!theStack.empty())
			{
				temp = theStack.top();
				theStack.pop();
				expression.append(temp);
			}
			if(i!=line.size()-1)
			{
				expression.append(" =\n");
			}
		}

		else if (isOperand(line[i]))
		{
			/*prevSymb = false;
			
			if (prevOprnd)
			{
				stream << "Infix to Postfix Conversion ERROR: Missing mathmatical symbol at: ["
					<< row_count << ", " << col_count << "]";
				errormsg = stream.str();
				Output(errormsg);

				expression.clear();
				line.clear();

				while (!theStack.empty())
				{
					theStack.pop();
				}

				iserrormsg = true;
				break;
			}*/

			expression.append(line, i, 1);
			//prevOprnd = true;
		}
		else if (line[i] == '(')
		{
			/*if (line[i + 1] != NULL && !isOperand(line[i + 1]))
			{
				stream << "Infix to Postfix Conversion ERROR: Missing operand at: ["
					<< row_count << ", " << col_count << "]";
				errormsg = stream.str();
				Output(errormsg);
				expression.clear();
				line.clear();
				while (!theStack.empty())
				{
					theStack.pop();
				}
				iserrormsg = true;

				break;
			}
			if ((i - 1) >= 0 && isOperand(line[i - 1]))
			{
				stream << "Infix to Postfix Conversion ERROR: Missing mathematical symbol at: ["
					<< row_count << ", " << col_count << "]";
				errormsg = stream.str();
				Output(errormsg);
				expression.clear();
				line.clear();
				while (!theStack.empty())
				{
					theStack.pop();
				}
				iserrormsg = true;

				break;
			}*/
			theStack.push(line[i]);
		}
		else if (line[i] == ')')
		{
			/*if ((i - 1) >= 0 && !isOperand(line[i - 1]))
			{
				stream << "Infix to Postfix Conversion ERROR: Missing operand at: ["
					<< row_count << ", " << col_count << "]";
				errormsg = stream.str();
				Output(errormsg);
				expression.clear();
				line.clear();
				while (!theStack.empty())
				{
					theStack.pop();
				}
				iserrormsg = true;

				break;
			}
			if (line[i + 1] != NULL && isOperand(line[i + 1]))
			{
				stream << "Infix to Postfix Conversion ERROR: Missing mathematical symbol at: ["
					<< row_count << ", " << col_count << "]";
				errormsg = stream.str();
				Output(errormsg);
				expression.clear();
				line.clear();
				while (!theStack.empty())
				{
					theStack.pop();
				}
				iserrormsg = true;

				break;
			}*/
			while (!theStack.empty() && theStack.top() != '(')
			{
				temp = theStack.top();
				theStack.pop();
				expression.append(temp);
			}
			theStack.pop();
		}
		else
		{
			/*prevOprnd = false;
			if (prevSymb)
			{
				stream << "Infix to Postfix Conversion ERROR: Missing operand at: ["
					<< row_count << ", " << col_count << "]1";
				errormsg = stream.str();
				Output(errormsg);
				expression.clear();
				line.clear();
				while (!theStack.empty())
				{
					theStack.pop();
				}
				iserrormsg = true;

				break;
			}*/
			while (!theStack.empty() && Prec(line[i]) <= Prec(theStack.top()))
			{
				temp = theStack.top();
				theStack.pop();
				expression.append(temp);
			}
			theStack.push(line[i]);
			prevSymb = true;
		}
	}

	while (!theStack.empty())
	{
		temp = theStack.top();
		theStack.pop();
		expression.append(temp);
	}
	expression.append(" =");
}

void InPostFix::PosttoInFix(string line)
{
	string temp;
	string tempstr1;
	string tempstr2;
	string tempstr3;
	char tempchar;
	int i;

	for (i = 0; i < line.size(); i++)
	{
		if (line[i] == ' ') continue;
		else if(line[i] == '=')
		{
			expression.append(strStack.top());
			strStack.pop();
			if(i!=line.size()-1)
			{
				expression.append(" =\n");
			}
		}
		else if (isOperand(line[i]))
		{
			tempstr1 = line[i];
			strStack.push(tempstr1);
		}
		else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '^')
		{
			if (!strStack.empty())
			{
				tempstr1 = strStack.top();
				strStack.pop();

				if (strStack.empty())
				{
					//invalid expression statement
				}

				tempstr2 = strStack.top();
				strStack.pop();
				tempstr3 = line[i];
				if (tempstr2.size() > 1 && tempstr1.size() > 1)
				{
					tempstr2.insert(0, 1, '(');
					tempstr2.insert(tempstr2.end(), 1, ')');
					tempstr1.insert(0, 1, '(');
					tempstr1.insert(tempstr1.end(), 1, ')');
				}
				tempstr2.append(tempstr3);
				tempstr2.append(tempstr1);
				strStack.push(tempstr2);
			}
			else
			{
				//invalid expression statement
			}
		}
		else
		{
			//invalid expression statement
		}
	}

	if(!strStack.empty())
	{
		expression.append(strStack.top());
		strStack.pop();
	}
	expression.append(" =");
	return;
}

int InPostFix::isOperand(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int InPostFix::Prec(char ch)
{
	switch (ch)
	{
	case '+':
	case '-':
		return 1;

	case '*':
	case '/':
		return 2;

	case '^':
		return 3;
	}
	return -1;
}

void InPostFix::Output(string message)
{
	if (!quiet)
	{
		cout << message << endl;
	}
	return;
}

string InPostFix::GetExpression()
{
	return expression;
}
