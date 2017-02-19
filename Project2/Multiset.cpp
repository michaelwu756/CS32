#include "Multiset.h"
using namespace std;
void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	Multiset ms;//create a new multiset, copy values from ms1 and ms2, then use assignment operator to set result equal to the new multiset
	ms1.copyIntoOtherMultiset(ms);
	ms2.copyIntoOtherMultiset(ms);
	result=ms;
}
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	Multiset ms;
	ms1.copyIntoOtherMultiset(ms);//copy values from ms1 into new multiset
	int num;
	ItemType item;
	for(int i=0;i<ms2.uniqueSize();i++)//call erase for every value in ms2
	{
		num=ms2.get(i,item);
		for(int j=0;j<num;j++)
			ms.erase(item);
	}
	result = ms;//use assignment operator to set result equal to new multiset
}
Multiset::Multiset():m_numItems(0), m_numUniqueItems(0),head(nullptr),tail(nullptr){}
Multiset::Multiset(const Multiset& src):m_numItems(src.m_numItems), m_numUniqueItems(src.m_numUniqueItems)
{
	if (src.head==nullptr)//if source has 0 elements, initialize head and tail to null
	{
		head=nullptr;
		tail=nullptr;
	}
	else
	{
		head=new DataCounterNode(src.head->value, src.head->count);//else create node for head to point at
		DataCounterNode* n=src.head;
		DataCounterNode* p=head;
		while(n->next!=nullptr)//traverse through source, creating and linking a copy of each node in the new list
		{
			n=n->next;
			DataCounterNode* copy=new DataCounterNode(n->value,n->count);
			p->next=copy;
			copy->prev=p;
			p=copy;
		}
		tail=p;//set tail to last element of doubly linked list
	}
}
Multiset::~Multiset()
{
	DataCounterNode* p=head;
	while(p!=nullptr)//traverse the list, deleting each element after moving to the next node
	{
		DataCounterNode* n=p;
		p=p->next;
		delete n;
	}
}
Multiset& Multiset::operator=(const Multiset& src)
{
	if(this!=&src)
	{
		DataCounterNode* p=head;//same as destructor, to free all memory
		while(p!=nullptr)
		{
			DataCounterNode* n=p;
			p=p->next;
			delete n;
		}
		m_numItems=src.m_numItems;//set scalar member variables equal
		m_numUniqueItems=src.m_numUniqueItems;
		
		if (src.head==nullptr)//same as copy constructor, to allocate new memory and copy values from source to target
		{
			head=nullptr;
			tail=nullptr;
		}
		else
		{
			head=new DataCounterNode(src.head->value, src.head->count);
			DataCounterNode* n=src.head;
			DataCounterNode* ptr=head;
			while(n->next!=nullptr)
			{
				n=n->next;
				DataCounterNode* copy=new DataCounterNode(n->value,n->count);
				ptr->next=copy;
				copy->prev=ptr;
				ptr=copy;
			}
			tail=ptr;
		}
	}
	return *this;
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
	if(head==nullptr)//if no elements, create the first node and make head and tail point to it
	{
		head=new DataCounterNode(value);
		tail=head;
		m_numUniqueItems++;//update size counters
		m_numItems++;
		return true;
	}
	bool unique=true;//assume value is not in list
	DataCounterNode* p=head;
	while(p!=nullptr)//if value is already in list, add 1 to its count
	{
		if (p->value==value)
		{
			p->count++;
			unique=false;//mark that it is in list
			break;
		}
		p=p->next;
	}
	if(unique)//if value is not in list, create a new node for it at the tail
	{
		DataCounterNode* newNode= new DataCounterNode(value);
		newNode->prev=tail;//link tail, update it and update size counters
		tail->next=newNode;
		tail=newNode;
		m_numUniqueItems++;
	}
	m_numItems++;
	return true;
}
int Multiset::erase(const ItemType& value)
{
	DataCounterNode* p=head;
	while(p!=nullptr)//traverse list starting from head looking for value
	{
		if(p->value==value)
		{
			if(p->count>1)//if value is found and count is greater than 1, lower count
				p->count--;
			else if(p==head)//else count is equal to 1, so a node must be deleted
			{
				head=head->next;//if value is in the first node, move head up 1
				if (head!=nullptr)//if head points to a valid node
					head->prev=nullptr;//set previous to null
				else//otherwise there are no more elements in the doubly linked list so set tail to null as well
					tail=nullptr;
				delete p;//delete node with value
				m_numUniqueItems--;//update size counter
			}
			else //else the node is in the middle or at the tail
			{
				DataCounterNode* n=p->prev;//create a pointer to the node before the one to delete
				n->next=p->next;//link the node before to the node after
				delete p;//delete node with value
				if (n->next!=nullptr)//if the next node exists
				{
					p=n->next;//link the node after to the node before
					p->prev=n;
				}
				else//else the node before is the last element, so make tail point to it
					tail=n;
				m_numUniqueItems--;//update size counter
			}
			m_numItems--;//update size counter
			return 1;
		}
		p=p->next;
	}
	return 0;
}
int Multiset::eraseAll(const ItemType& value)
{
	DataCounterNode* p=head;
	while(p!=nullptr)//traverse list looking for value
	{
		if(p->value==value)
		{
			int numErased=p->count;//value is found so note how many values will be deleted
			if(p==head)//if the value is at the head, delete the head node
			{
				head=head->next;
				if (head!=nullptr)//if there are still elements in the list, update head pointer
					head->prev=nullptr;
				else//otherwise no elements, so set tail to null
					tail=nullptr;
				delete p;//delete node with value in it
			}
			else//else value is in middle or tail
			{
				DataCounterNode* n=p->prev;//get node before the one with the value
				n->next=p->next;//link the node before to the node after
				delete p;//delete node with value in it
				if (n->next!=nullptr)//if the node after is a valid node link it to the node before
				{
					p=n->next;
					p->prev=n;
				}
				else//else the node before is the last node, so update tail to point to it
					tail=n;
			}
			m_numUniqueItems--;//update size counters and return the number erased
			m_numItems-=numErased;
			return numErased;
		}
		p=p->next;
	}
	return 0;
}
bool Multiset::contains(const ItemType& value) const
{
	DataCounterNode* p=head;//traverse list, return true if value is found
	while(p!=nullptr)
	{
		if(p->value==value)
			return true;
		p=p->next;
	}
	return false;
}
int Multiset::count(const ItemType& value) const
{
	DataCounterNode* p=head;//traverse list, return count of value if it is found
	while(p!=nullptr)
	{
		if(p->value==value)
			return p->count;
		p=p->next;
	}
	return false;
}
int Multiset::get(int i, ItemType& value) const
{
	DataCounterNode* p=head;
	if(i<m_numUniqueItems && i>=0)//if i is a valid index
	{
		for(int j=0;j<i;j++)//move forward in list i times
			p=p->next;
		value=p->value;//set value equal to value in node
		return p->count;//return count in node
	}
	return 0;
}
bool Multiset::getLeastFrequentValue(ItemType &value) const
{
	DataCounterNode* p=head;
	if(m_numUniqueItems>0)//make sure there are elements in the list
	{
		int lowest=head->count;//assume lowest is the count in the head node
		while(p!=nullptr)//traverse list, setting lowest to any number lower than its current number
		{
			if(p->count<lowest)
				lowest=p->count;
			p=p->next;
		}
		p=head;
		int lowestCount=0;
		while(p!=nullptr)//traverse list again, counting how many times the lowest count occurs
		{
			if(p->count==lowest)
				lowestCount++;
			p=p->next;
		}
		p=head;
		if(lowestCount==1)//if the lowest count is unique
			while(p!=nullptr)//traverse list to find where the lowest count is
			{
				if(p->count==lowest)
				{
					value=p->value;//set value equal to the value in node and return true
					return true;
				}
				p=p->next;
			}
	}
	return false;
}
bool Multiset::getSmallestValue(ItemType &value) const
{
	if(m_numUniqueItems>0)//make sure there are elements in the list
	{
		ItemType smallest=head->value;//assume smallest value is at head
		DataCounterNode* p=head;
		while(p!=nullptr)//traverse list, updating smallest to any smaller value
		{
			if(p->value<smallest)
				smallest=p->value;
			p=p->next;
		}
		value=smallest;//set value to the smallest and return true
		return true;
	}
	return false;
}
bool Multiset::getSecondSmallestValue(ItemType &value) const
{
	if(m_numUniqueItems>1)//make sure there are at least two elements in list
	{
		ItemType smallest;
		if (!getSmallestValue(smallest))//make sure getting the smallest value works
			return false;
		ItemType secondSmallest;
		if(smallest==head->value)//if head is smallest value, assume second smallest value is at next node
			secondSmallest=head->next->value;
		else
			secondSmallest=head->value;//else assume second smallest value is at head
		DataCounterNode* p=head;
		while(p!=nullptr)//traverse list
		{
			if(p->value<secondSmallest && p->value!=smallest)//update second smallest value to any value smaller, but not the smallest
				secondSmallest=p->value;
			p=p->next;
		}	
		value=secondSmallest;//set value to second smallest and return true
		return true;
	}
	return false;
}
bool Multiset::replace(ItemType original, ItemType new_value)
{
	if (contains(original))//make sure original is in list
	{
		if (new_value==original)//ensure it works when both parameters are the same
			return true;
		if (contains(new_value))//if new value is already in list
		{
			DataCounterNode* p=head;
			DataCounterNode* q=head;
			while(p!=nullptr)//traverse list and make p point to node with original
			{
				if(p->value==original)
					break;
				p=p->next;
			}
			while(q!=nullptr)//traverse list and make q point to node with original
			{
				if(q->value==new_value)
					break;
				q=q->next;
			}
			q->count+=p->count;//add number of old values to number of new values
			if(p==head)//use previous code to delete node holding original value
			{
				head=head->next;//if original is at head, update head
				if (head!=nullptr)//if head still points to valid node, make its previous pointer null
					head->prev=nullptr;
				else//else there are no elements in list, so set tail to null
					tail=nullptr;
				delete p;
			}
			else//else node is not at head
			{
				DataCounterNode* n=p->prev;//get previous node and link it to the next node
				n->next=p->next;
				delete p;
				if (n->next!=nullptr)//if next node is not null, link it to previous node
				{
					p=n->next;
					p->prev=n;
				}
				else//if next node is null then it is the tail, so update tail
					tail=n;
			}
			m_numUniqueItems--;//update size counter
		}
		else
		{
			DataCounterNode* p=head;
			while(p!=nullptr)//if the new value is not in the list, find the original node
			{
				if(p->value==original)
					p->value=new_value;//change original value to new value
				p=p->next;
			}
		}
		return true;
	}
	return false;
}
int Multiset::countIf(char op, ItemType value) const
{
	int toAdd=0;
	int total=0;
	DataCounterNode* p=head;
	switch(op)
	{
		case '=':
			return count(value);//if equals, function behaves same as count(value)
		case '>':
			while(p!=nullptr)//if greater than, loop through the list
			{
				toAdd=p->count;
				if(p->value>value)//for every node with value higher than the parameter, add the count to the total
					total+=toAdd;
				p=p->next;
			}
			return total;
		case '<':
			while(p!=nullptr)//if less than, loop through the list
			{
				toAdd=p->count;
				if(p->value<value)//for every node with value lower than the parameter, add the count to the total
					total+=toAdd;
				p=p->next;
			}
			return total;
	}
	return -1;
}
void Multiset::swap(Multiset& other)
{
	DataCounterNode* tempHead=other.head;//create temp variables for all member variables and swap them
	DataCounterNode* tempTail=other.tail;
	int tempUniqueItems=other.m_numUniqueItems;
	int tempItems=other.m_numItems;
	other.head=head;
	other.tail=tail;
	other.m_numUniqueItems=m_numUniqueItems;
	other.m_numItems=m_numItems;
	head=tempHead;
	tail=tempTail;
	m_numUniqueItems=tempUniqueItems;
	m_numItems=tempItems;
}
void Multiset::copyIntoOtherMultiset(Multiset &other) const
{
	Multiset ms=other;//create a copy of other to account for aliasing
	DataCounterNode* p=head;
	while(p!=nullptr)//traverse the list
	{
		for(int i=0;i<p->count;i++)//call insert for each value into the copy
				ms.insert(p->value);
		p=p->next;
	}
	other=ms;//set other equal to the copy
}
