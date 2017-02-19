#include <iostream>
#include "StudentMultiset.h"
using namespace std;
StudentMultiset::StudentMultiset()
{
}

bool StudentMultiset::add(unsigned long id)
{
	return m_ms.insert(id);
}

int StudentMultiset::size() const
{
	return m_ms.size();
}

void StudentMultiset::print() const
{
	for(int i=0;i<m_ms.uniqueSize();i++)
	{
		unsigned long id;
		int timesSubmitted=m_ms.get(i,id);
		for(int j=0;j<timesSubmitted;j++)
			cout<<id<<endl;
	}
}
