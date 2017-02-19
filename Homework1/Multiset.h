#ifndef MULTISET_H
#define MULTISET_H
#include <string>
typedef unsigned long ItemType;
const int DEFAULT_MAX_ITEMS = 200;
class Multiset
{
	public:
		Multiset();
		// Create an empty multiset.

		bool empty() const;
		// Return true if the multiset is empty, otherwise false.

		int size() const;
		// Return the number of items in the multiset.  For example, the size
		// of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.

		int uniqueSize() const;
		// Return the number of distinct items in the multiset.  For example,
		// the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
		// "turmeric" is 2.
	 
		bool insert(const ItemType& value);
		// Insert value into the multiset.  Return true if the value was
		// actually inserted.  Return false if the value was not inserted
		// (perhaps because the multiset has a fixed capacity and is full).
	 
		int erase(const ItemType& value);
		// Remove one instance of value from the multiset if present.
		// Return the number of instances removed, which will be 1 or 0.
	 
		int eraseAll(const ItemType& value);
		// Remove all instances of value from the multiset if present.
		// Return the number of instances removed.
	 
		bool contains(const ItemType& value) const;
		// Return true if the value is in the multiset, otherwise false.
	 
		int count(const ItemType& value) const;
		// Return the number of instances of value in the multiset.

		int get(int i, ItemType& value) const;
		// If 0 <= i < uniqueSize(), copy into value an item in the
		// multiset and return the number of instances of that item in
		// the multiset.  Otherwise, leave value unchanged and return 0.
		// (See below for details about this function.)

		bool getLeastFrequentValue(ItemType &value) const;
		// If there exists a single item that has the least number of instances in the multiset, 
		// then copy into value that item in the multiset and return true.
		// However, if there exist more than 1 item that have the least number of instances in the multiset,
		// then do not copy into value any item in the multiset and return false. In other words, value should remain unchanged.
		// If there's no item in the multiset, return false.

		bool getSmallestValue(ItemType &value) const;
		// If there exists a value that is the smallest value among all the values in the multiset, 
		// then copy into value that item in the multiset and return true
		// Otherwise, return false.
		// For both unsigned long and string data type, the lower value can be found by using less than operator (<).
		// For example, 10 is smaller than 20, so 10 < 20 is true.
		// "ABCDE" is smaller than "XYZ", so "ABCDE" < "XYZ" is true.

		bool getSecondSmallestValue(ItemType &value) const;
		// Similar to getSmallestValue(), but this time you need to find the second smallest value.
		// If there exists a value that is the 2nd smallest value among all the values in the multiset,
		// then copy into value that item in the multiset and return true.
		// Otherwise, return false.
		// Please note that you cannot use any sorting algorithm to sort the multiset.

		bool replace(ItemType original, ItemType new_value);
		// Replace the item that has the value equal to original by the new value new_value.
		// For example, replace("ABCDE","XYZ") will search the multiset for the item "ABCDE" and replace all occurrences of "ABCDE" as "XYZ".
		// If the replacement is successful, then return true.
		// If there is no item to be replaced, then return false.

		int countIf(char op, ItemType value) const;
		// Count the number of items that the item is greater than, less than, or equal to value.
		// For example: 
		// countIf('>',100) returns the number of items in multiset in which the item is greater than 100.
		// countIf('=',"ABC") returns the number of items in multiset in which the item is equal to "ABC".
		// countIf('<',50) returns the number of items in multiset in which the item is less than 50.
		// If op is a character other than '>','=', and '<', then return -1.
	
		void swap(Multiset& other);
		// Exchange the contents of this multiset with the other one.
		
		void copyIntoOtherMultiset(Multiset &other) const;
		// Insert all the items into the multiset in other.
	private:
		struct DataCounter
		{
			ItemType value;
			int count;
		};
		DataCounter m_itemArr[DEFAULT_MAX_ITEMS];
		int m_numUniqueItems;
		int m_numItems;
};
#endif