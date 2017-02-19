#include "newMultiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	/*
    Multiset ms1;
	assert(ms1.empty() && ms1.size()==0 && ms1.uniqueSize()==0);
	ms1.insert(123);
	assert(!ms1.empty()&& ms1.size()==1 && ms1.uniqueSize()==1);
	ms1.insert(123);ms1.insert(456);
	assert(ms1.size()==3 && ms1.uniqueSize()==2);
	ms1.erase(123);
	assert(ms1.size()==2 && ms1.uniqueSize()==2 && ms1.contains(456));
	ms1.insert(651321); ms1.insert(412); ms1.insert(8484); ms1.insert(123); ms1.insert(456);
	ms1.eraseAll(456);
	assert(!ms1.contains(456)&&ms1.count(123)==2);
	unsigned long x;
	assert(!ms1.getLeastFrequentValue(x)&&ms1.getSmallestValue(x)==true&&x==123);
	assert(ms1.getSecondSmallestValue(x)==true && x==412);
	assert(ms1.replace(123,456)==true && ms1.count(456)==2);
	assert(ms1.countIf('<',1000)==3);
	for(int i=0;i<ms1.uniqueSize();i++)
	{
		unsigned long val;
		int num=ms1.get(i,val);
		cout<<val<<" occurs "<<num<<" times"<<endl;
	}
	Multiset ms2;
	ms2.insert(1);ms2.insert(1);ms2.insert(2);
	ms1.swap(ms2);
	assert(ms1.size()==3 && ms2.size()==5);
	ms2.copyIntoOtherMultiset(ms1);
	assert(ms1.size()==8 && ms2.size()==5);
	cout<<endl;
	for(int i=0;i<ms1.uniqueSize();i++)
	{
		unsigned long val;
		int num=ms1.get(i,val);
		cout<<val<<" occurs "<<num<<" times"<<endl;
	}
	ms1=ms2;
	cout<<endl;
	for(int i=0;i<ms1.uniqueSize();i++)
	{
		unsigned long val;
		int num=ms1.get(i,val);
		cout<<val<<" occurs "<<num<<" times"<<endl;
	}
	 Multiset a(1000);   // a can hold at most 1000 distinct items
     Multiset b(5);      // b can hold at most 5 distinct items
     Multiset c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
     ItemType v[6] = {1,2,3,4,5,6};
       // No failures inserting 5 distinct items twice each into b
     for (int k = 0; k < 5; k++)
     {
         assert(b.insert(v[k]));
         assert(b.insert(v[k]));
     }
     assert(b.size() == 10  &&  b.uniqueSize() == 5  &&  b.count(v[0]) == 2);
       // Failure if we try to insert a sixth distinct item into b
	 
     assert(!b.insert(v[5]));

       // When two Multisets' contents are swapped, their capacities are swapped
       // as well:
     a.swap(b);
     assert(!a.insert(v[5])  &&  b.insert(v[5]));
    cout << "Passed all tests" << endl;*/
}