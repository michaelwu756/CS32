#include"Multiset.h"
Multiset::Multiset():m_numItems(0), m_numUniqueItems(0)
{
	for (int i=0;i<DEFAULT_MAX_ITEMS;i++)
		m_itemArr[i].count=0;
}
bool Multiset::empty() const
{
	if(m_numItems==0)
		return true;
	return false;
}
int Multiset::size() const
{
	return m_numItems;
}
int Multiset::uniqueSize() const
{
	return m_numUniqueItems;
}
bool Multiset::insert(const ItemType& value)
{
	if (m_numUniqueItems<=DEFAULT_MAX_ITEMS)
	{
		bool unique=true;
		for(int i=0;i<m_numUniqueItems;i++)
			if(m_itemArr[i].value==value)
			{
				m_itemArr[i].count++;
				unique=false;
				break;
			}
		if(unique)
		{
			if (m_numUniqueItems==DEFAULT_MAX_ITEMS)
				return false;
			m_itemArr[m_numUniqueItems].value=value;
			m_itemArr[m_numUniqueItems].count=1;
			m_numUniqueItems++;
		}
		m_numItems++;
		return true;
	}
	return false;
}
int Multiset::erase(const ItemType& value)
{
	for (int i=0;i<m_numUniqueItems;i++)
		if(m_itemArr[i].value==value)
		{
			if(m_itemArr[i].count>1)
				m_itemArr[i].count--;
			else
			{
				for(int j=i;j<m_numUniqueItems-1;j++)
					m_itemArr[j]=m_itemArr[j+1];
				m_numUniqueItems--;
			}
			m_numItems--;
			return 1;
		}
	return 0;
}
int Multiset::eraseAll(const ItemType& value)
{
	for(int i=0;i<m_numUniqueItems;i++)
		if(m_itemArr[i].value==value)
		{
			int numErased = m_itemArr[i].count;
			for(int j=i;j<m_numUniqueItems-1;j++)
				m_itemArr[j]=m_itemArr[j+1];
			m_numUniqueItems--;
			m_numItems-=numErased;
			return numErased;
		}
	return 0;
}
bool Multiset::contains(const ItemType& value) const
{
	for (int i=0;i<m_numUniqueItems;i++)
		if(m_itemArr[i].value==value)
			return true;
	return false;
}
int Multiset::count(const ItemType& value) const
{
	for (int i=0;i<m_numUniqueItems;i++)
		if(m_itemArr[i].value==value)
			return m_itemArr[i].count;
	return 0;
}
int Multiset::get(int i, ItemType& value) const
{
	if(i<m_numUniqueItems)
	{
		value=m_itemArr[i].value;
		return m_itemArr[i].count;
	}
	return 0;
}
bool Multiset::getLeastFrequentValue(ItemType &value) const
{
	if(m_numUniqueItems>0)
	{
		int lowest=m_itemArr[0].count;
		for(int i=0;i<m_numUniqueItems;i++)
			if(m_itemArr[i].count<lowest)
				lowest=m_itemArr[i].count;
		int lowestCount=0;
		for(int i=0;i<m_numUniqueItems;i++)
			if(m_itemArr[i].count==lowest)
				lowestCount++;
		if(lowestCount==1)
			for(int i=0;i<m_numUniqueItems;i++)
				if(m_itemArr[i].count==lowest)
				{
					value=m_itemArr[i].value;
					return true;
				}
	}
	return false;
}
bool Multiset::getSmallestValue(ItemType &value) const
{
	if(m_numUniqueItems>0)
	{
		ItemType smallest=m_itemArr[0].value;
		for(int i=0;i<m_numUniqueItems;i++)
			if(m_itemArr[i].value<smallest)
				smallest=m_itemArr[i].value;
		value=smallest;
		return true;
	}
	return false;
}
bool Multiset::getSecondSmallestValue(ItemType &value) const
{
	if(m_numUniqueItems>1)
	{
		ItemType smallest;
		if (!getSmallestValue(smallest))
			return false;
		ItemType secondSmallest;
		if(smallest==m_itemArr[0].value)
			secondSmallest=m_itemArr[1].value;
		else
			secondSmallest=m_itemArr[0].value;
		for(int i=0;i<m_numUniqueItems;i++)
			if(m_itemArr[i].value<secondSmallest && m_itemArr[i].value!=smallest)
				secondSmallest=m_itemArr[i].value;
		value=secondSmallest;
		return true;
	}
	return false;
}
bool Multiset::replace(ItemType original, ItemType new_value)
{
	if (contains(original))
	{
		if (contains(new_value))
		{
			int i,j;
			for (i=0;i<m_numUniqueItems;i++)
				if(m_itemArr[i].value==original)
					break;
			for (j=0;j<m_numUniqueItems;j++)
				if(m_itemArr[j].value==new_value)
					break;
			m_itemArr[j].count+=m_itemArr[i].count;
			for(int k=i;k<m_numUniqueItems-1;k++)
				m_itemArr[k]=m_itemArr[k+1];
			m_numUniqueItems--;
		}
		else
			for (int i=0;i<m_numUniqueItems;i++)
				if(m_itemArr[i].value==original)
					m_itemArr[i].value=new_value;
		return true;
	}
	return false;
}
int Multiset::countIf(char op, ItemType value) const
{
	int toAdd=0;
	int total=0;
	switch(op)
	{
		case '=':
			return count(value);
		case '>':
			for (int i=0;i<m_numUniqueItems;i++)
			{
				toAdd=m_itemArr[i].count;
				if(m_itemArr[i].value>value)
					total+=toAdd;
			}
			return total;
		case '<':
			for (int i=0;i<m_numUniqueItems;i++)
			{
				toAdd=m_itemArr[i].count;
				if(m_itemArr[i].value<value)
					total+=toAdd;
			}
			return total;
	}
	return -1;
}
void Multiset::swap(Multiset& other)
{
	Multiset tempSet;
	tempSet.m_numUniqueItems=m_numUniqueItems;//copy multiset into temp set
	tempSet.m_numItems=m_numItems;
	for(int i=0;i<m_numUniqueItems;i++)
		tempSet.m_itemArr[i]=m_itemArr[i];
	m_numUniqueItems=other.m_numUniqueItems;//copy other into multiset
	m_numItems=other.m_numItems;
	for(int i=0;i<other.m_numUniqueItems;i++)
		m_itemArr[i]=other.m_itemArr[i];
	other.m_numUniqueItems=tempSet.m_numUniqueItems;
	other.m_numItems=tempSet.m_numItems;
	for(int i=0;i<tempSet.m_numUniqueItems;i++)//copy temp set into other
		other.m_itemArr[i]=tempSet.m_itemArr[i];
}
void Multiset::copyIntoOtherMultiset(Multiset &other) const
{
	for(int i=0;i<m_numUniqueItems;i++)//copy multiset into other
		for(int j=0;j<m_itemArr[i].count;j++)
			other.insert(m_itemArr[i].value);
}