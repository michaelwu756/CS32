#include<string>
#include<stack>
using namespace std;
bool balanced(std::string expr)
{
	stack<char> bracketStack;
	for(int i=0;i<expr.length();i++)
	{
		char c = expr[i];
		if(c=='(' || c=='[' || c== '{')
			bracketStack.push(c);
		if(c==')' || c==']' || c== '}')
			if(bracketStack.empty())
				return false;
			else
			{
				char top = bracketStack.top();
				bracketStack.pop();
				switch(c)
				{
					case ')':
						if (top!='(')
							return false;
					break;
					case ']':
						if (top!='[')
							return false;
					break;
					case '}':
						if (top!='{')
							return false;
				}
			}
	}
	if(!bracketStack.empty())
		return false;
	return true;
}
/*
int main()
{
	assert(balanced("This class is CS 32 (Summer 2016)"));
	assert(balanced("John von Neumann (December 28, 1903 – February 8, 1957) was a great computer scientist."));
	assert(balanced("(([[]{}]()))"));
	assert(balanced("Midterm I is on July 11th."));
	assert(balanced("()[]"));
	assert(!balanced("This class is important for many reasons.\n1) It is required for graduation for many majors."));
	assert(!balanced(":-)"));
	assert(!balanced("(:"));
	assert(!balanced("([)]"));
}*/