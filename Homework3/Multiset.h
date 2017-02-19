// Multiset.h
#ifndef MULTISET_INCLUDED
#define MULTISET_INCLUDED
#include <utility>
template<typename T>
class Multiset
{
  public:
    Multiset();          // Create an empty multiset.
    bool empty() const;  // Return true if the multiset is empty, otherwise false.

    int size() const;
      // Return the number of items in the multiset.  For example, the size
      // of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.

    int uniqueSize() const;
      // Return the number of distinct items in the multiset.  For example,
      // the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
      // "turmeric" is 2.

    bool insert(const T& value);
      // Insert value into the multiset.  Return true if the value was
      // actually inserted.  Return false if the value was not inserted
      // (perhaps because the multiset has a fixed capacity and is full).

    int erase(const T& value);
      // Remove one instance of value from the multiset if present.
      // Return the number of instances removed, which will be 1 or 0.

    int eraseAll(const T& value);
      // Remove all instances of value from the multiset if present.
      // Return the number of instances removed.

    bool contains(const T& value) const;
      // Return true if the value is in the multiset, otherwise false.

    int count(const T& value) const;
      // Return the number of instances of value in the multiset.

    int get(int i, T& value) const;
      // If 0 <= i < uniqueSize(), copy into value an item in the multiset
      // and return the number of instances of that item in the multiset.
      // Otherwise, leave value unchanged and return 0.

    bool getLeastFrequentValue(T& value) const;
      // If there exists a single item that has the least number of instances in the multiset, 
      // then copy into value that item in the multiset and return true.
      // However, if there exist more than 1 item that have the least number of instances in the multiset,
      // then do not copy into value any item in the multiset and return false. In other words, value should remain unchanged.
      // If there's no item in the multiset, return false.

    bool getSmallestValue(T& value) const;
      // If there exists a value that is the smallest value among all the values in the multiset, 
      // then copy into value that item in the multiset and return true
      // Otherwise, return false.
      // For both unsigned long and string data type, the lower value can be found by using less than operator (<).
      // For example, 10 is smaller than 20, so 10 < 20 is true.
      // "ABC" is smaller than "XYZ", so "ABC" < "XYZ" is true.

    bool getSecondSmallestValue(T& value) const;
      // Similar to getSmallestValue(), but this time you need to find the second smallest value.
      // If there exists a value that is the 2nd smallest value among all the values in the multiset,
      // then copy into value that item in the multiset and return true.
      // Otherwise, return false.
      // Please note that you cannot use any sorting algorithm to sort the multiset.

    bool replace(T original, T new_value);
      // Replace the item that has the value equal to original by the new value
      // new_value.  For example, replace("ABC","XYZ") will search the multiset
      // for the item "ABC" and replace all occurrences of "ABC" as "XYZ".
      // If the replacement is successful, then return true.  If there is no
      // item to be replaced, then return false.

    int countIf(char op, T value) const;
      // Count the number of items that the item is greater than, less than, or
      // equal to value.  For example:  countIf('>',100) returns the number of
      // items in multiset in which the item is greater than 100.

    void swap(Multiset& other);
      // Exchange the contents of this multiset with the other one.

    void copyIntoOtherMultiset(Multiset& other) const;
      // Insert all the items into the multiset in other.

      // Housekeeping functions
    ~Multiset();
    Multiset(const Multiset& other);
    Multiset& operator=(const Multiset& rhs);

  private:
      // Representation:
      //   a circular doubly-linked list with a dummy node.
      //   m_head points to the dummy node.
      //   m_head->m_prev->m_next == m_head and m_head->m_next->m_prev == m_head
      //   m_uniqueSize == 0 and m_size == 0  if and only if
      //                           m_head->m_next == m_head->m_prev == m_head
      //   In addition to the dummy node, the list has m_uniqueSize nodes.
      //   Nodes are in no particular order.

    struct Node
    {
        T m_value;
        int      m_count;
        Node*    m_next;
        Node*    m_prev;
    };

    Node* m_head;
    int   m_uniqueSize;
    int   m_size;

    Node* find(const T& value) const;
      // Return pointer to Node whose m_value == value if there is one,
      // else m_head

    Node* insertHelper(const T& value, int nToInsert) const;
      // If value is present, increase its count by nToInsert and return nullptr.
      // Otherwise, create a new node with the value and count and return a
      // pointer to it (with m_next and m_prev being uninitialized).

    int doErase(const T& value, bool all);
      // Remove one or all instances of value from the multiset if present,
      // depending on the second parameter.  Return the number of instances
      // removed.
};
// Declarations of non-member functions
template<typename T>
void combine(const Multiset<T>& ms1, const Multiset<T>& ms2, Multiset<T>& result)
{
      // Guard against the case that result is an alias for ms1 or ms2
      // (i.e., that result is a reference to the same multiset that ms1 or ms2
      // refers to) by building the answer in a local variable res.  When
      // done, swap res with result; the old value of result (now in res) will
      // be destroyed when res is destroyed.

    Multiset<T> res(ms1);
    for (int k = 0; k < ms2.uniqueSize(); k++)
    {
        T v;
        for (int n = ms2.get(k, v); n > 0; n--)
            res.insert(v);
    }
    result.swap(res);
}
template<typename T>
void subtract(const Multiset<T>& ms1, const Multiset<T>& ms2, Multiset<T>& result)
{
      // Guard against the case that result is an alias for ms1 or ms2
      // by building the answer in a local variable res.  When done, swap res
      // with result; the old value of result (now in res) will be destroyed
      // when res is destroyed.

    Multiset<T> res;
    for (int k = 0; k != ms1.uniqueSize(); k++)
    {
        T v;
        int n = ms1.get(k, v);
        for (n -= ms2.count(v); n > 0; n--)
            res.insert(v);
    }
    result.swap(res);
}
// Inline implementations
template<typename T>
inline
int Multiset<T>::size() const
{
    return m_size;
}
template<typename T>
inline
int Multiset<T>::uniqueSize() const
{
    return m_uniqueSize;
}
template<typename T>
inline
bool Multiset<T>::empty() const
{
    return size() == 0;
}
template<typename T>
inline
int Multiset<T>::erase(const T& value)
{
    return doErase(value, false);
}
template<typename T>
inline
int Multiset<T>::eraseAll(const T& value)
{
    return doErase(value, true);
}
template<typename T>
inline
bool Multiset<T>::contains(const T& value) const
{
    return find(value) != m_head;
}
template<typename T>
Multiset<T>::Multiset()
 : m_uniqueSize(0), m_size(0)
{
      // create dummy node
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}
template<typename T>
Multiset<T>::~Multiset()
{
      // Delete the m_uniqueSize non-dummy nodes plus the dummy node

    for (Node* p = m_head->m_prev ; m_uniqueSize >= 0; m_uniqueSize--)
    {
        Node* toBeDeleted = p;
        p = p->m_prev;
        delete toBeDeleted;
    }
}
template<typename T>
Multiset<T>::Multiset(const Multiset<T>& other)
 : m_uniqueSize(other.m_uniqueSize), m_size(other.m_size)
{
      // Create dummy node; don't initialize its m_next

    m_head = new Node;
    m_head->m_prev = m_head;

      // Copy each node from the other list; each iteration will set the
      // m_next of the previous node copied

    for (Node* p = other.m_head->m_next ; p != other.m_head; p = p->m_next)
    {
          // Create a copy of the node p points to
        Node* pnew = new Node;
        pnew->m_value = p->m_value;
        pnew->m_count = p->m_count;
        
          // Connect the m_prev pointers
        pnew->m_prev = m_head->m_prev;
        m_head->m_prev = pnew;

          // Connect the previous Node's m_next 
        pnew->m_prev->m_next = pnew;
    }

      // Connect the last Node's m_next 
    m_head->m_prev->m_next = m_head;
}
template<typename T>
Multiset<T>& Multiset<T>::operator=(const Multiset<T>& rhs)
{
    if (this != &rhs)
    {
        Multiset<T> temp(rhs);
        swap(temp);
    }
    return *this;
}
template<typename T>
bool Multiset<T>::insert(const T& value)
{
    Node* p = insertHelper(value, 1);

    if (p != nullptr)
    {
          // Insert new node at tail of list (arbitrary choice of position)
          //     Connect it to tail
        p->m_prev = m_head->m_prev;
        p->m_prev->m_next = p;

          //     Connect it to dummy node
        p->m_next = m_head;
        m_head->m_prev = p;

        m_uniqueSize++;
    }

    m_size++;
    return true;
}
template<typename T>
int Multiset<T>::count(const T& value) const
{
    Node* p = find(value);
    return p == m_head ? 0 : p->m_count;
}
template<typename T>
int Multiset<T>::get(int i, T& value) const
{
    if (i < 0  ||  i >= m_uniqueSize)
        return 0;

      // Get the value at position i.  This is one way of ensuring the required
      // behavior of get:  If the Multiset doesn't change in the interim,
      // * calling get with each i in 0 <= i < size() gets each of the
      //   Multiset elements, and
      // * calling get with the same value of i each time gets the same element.

      // If i is closer to the head of the list, go forward to reach that
      // position; otherwise, start from tail and go backward.

    Node* p;
    if (i < m_uniqueSize / 2)  // closer to head
    {
        p = m_head->m_next;
        for (int k = 0; k != i; k++)
            p = p->m_next;
    }
    else  // closer to tail
    {
        p = m_head->m_prev;
        for (int k = m_uniqueSize-1; k != i; k--)
            p = p->m_prev;
    }

    value = p->m_value;
    return p->m_count;
}
template<typename T>
bool Multiset<T>::getLeastFrequentValue(T& value) const
{
    if (empty())
        return false;

      // Start by assuming first value is the unique least frequent value

    Node* pLeastFrequent = m_head->m_next;
    bool uniqueLeastFrequent = true;

      // See if any other value is the least frequent

    for (Node* p = pLeastFrequent->m_next; p != m_head; p = p->m_next)
    {
        int diff = (p->m_count - pLeastFrequent->m_count);
        if (diff == 0)  // least frequent value, but not unique
            uniqueLeastFrequent = false;
        else if (diff < 0)  // a new unique least frequent value
        {
            pLeastFrequent = p;
            uniqueLeastFrequent = true;
        }
    }

      // Set value and return true only if the least frequent value is unique

    if (uniqueLeastFrequent)
        value = pLeastFrequent->m_value;
    return uniqueLeastFrequent;
}
template<typename T>
bool Multiset<T>::getSmallestValue(T& value) const
{
    if (empty())
        return false;

      // Start by assuming first value is the smallest

    Node* pSmallest = m_head->m_next;

      // See if any other value is smaller

    for (Node* p = pSmallest->m_next; p != m_head; p = p->m_next)
        if (p->m_value < pSmallest->m_value)
            pSmallest = p;
    value = pSmallest->m_value;
    return true;
}
template<typename T>
bool Multiset<T>::getSecondSmallestValue(T& value) const
{
    if (m_uniqueSize < 2)
        return false;

      // pSmallest1 points to smallest seen so far
      // pSmallest2 points to second smallest seen so far
      // Start by assuming they're in first two positions.

    Node* pSmallest1 = m_head->m_next;
    Node* pSmallest2 = pSmallest1->m_next;
    Node* third = pSmallest2->m_next;

    if (pSmallest1->m_value > pSmallest2->m_value)
        std::swap(pSmallest1, pSmallest2);

      // Check frequencies of other values, and adjust

    for (Node* p = third; p != m_head; p = p->m_next)
    {
        const T& v = p->m_value;
        if (v < pSmallest1->m_value)  // smallest value so far?
        {
            pSmallest2 = pSmallest1;  // old smallest now second smallest
            pSmallest1 = p;
        }
        else if (v < pSmallest2->m_value)  // second smallest?
            pSmallest2 = p;
    }

      // Set value to second smallest

    value = pSmallest2->m_value;
    return true;
}
template<typename T>
bool Multiset<T>::replace(T original, T new_value)
{
    Node* pOriginal = find(original);
    if (pOriginal == m_head)  // no occurrence of original
        return false;
    Node* pNew = find(new_value);
    if (pNew == m_head)  // new_value not already present?
        pOriginal->m_value = new_value;
    else if (pNew != pOriginal)  // new_value present and distinct from original?
    {
          // Absorb original count into new_value count and eliminate original

        pNew->m_count += pOriginal->m_count;
        pOriginal->m_prev->m_next = pOriginal->m_next;
        pOriginal->m_next->m_prev = pOriginal->m_prev;
        delete pOriginal;
        m_uniqueSize--;
    }
    return true;
}
template<typename T>
int Multiset<T>::countIf(char op, T value) const
{
    if (op == '=')
        return count(value);
    if (op != '<'  &&  op != '>')
        return -1;
    int total = 0;
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)
    {
        bool meetsCriterion = (op == '<' ? p->m_value < value :
                                           p->m_value > value);
        if (meetsCriterion)
            total += p->m_count;
    }
    return total;
}
template<typename T>
void Multiset<T>::swap(Multiset<T>& other)
{
    std::swap(m_head, other.m_head);
    std::swap(m_uniqueSize, other.m_uniqueSize);
    std::swap(m_size, other.m_size);
}
template<typename T>
void Multiset<T>::copyIntoOtherMultiset(Multiset<T>& other) const
{
      // Works even if this == &other

      // If we add new items to other, we won't insert them into  other's
      // list yet so that the call to find doesn't waste time looking at
      // items it doesn't need to.

    int newUniqueSize = other.m_uniqueSize;
    int newSize = other.m_size;
    Node* oldTail = other.m_head->m_prev;
    Node* newTail = oldTail;

    for (Node* p = m_head->m_next ; p != m_head; p = p->m_next)
    {
        Node* pNew = other.insertHelper(p->m_value, p->m_count);
        if (pNew != nullptr)
        {
            pNew->m_prev = newTail;
            newTail = pNew;
            newUniqueSize++;
        }
        newSize += p->m_count;
    }

      // Commit any new elements

    other.m_uniqueSize = newUniqueSize;
    other.m_size = newSize;

    if (newTail != oldTail)
    {
        other.m_head->m_prev = newTail;
        Node* p = other.m_head;
        do
        {
            p->m_prev->m_next = p;
            p = p->m_prev;
        } while (p != oldTail);
    }
}
template<typename T>
typename Multiset<T>::Node* Multiset<T>::find(const T& value) const
{
      // Do a linear search through the list

    Node* p;
    for (p = m_head->m_next; p != m_head && p->m_value != value; p = p->m_next)
        ;
    return p;
}
template<typename T>
typename Multiset<T>::Node* Multiset<T>::insertHelper(const T& value, int nToInsert) const
{
    Node* p = find(value);

    if (p != m_head)  // found
    {
        p->m_count += nToInsert;
        return nullptr;
    }
    else
    {
          // Create a new node
        p = new Node;
        p->m_value = value;
        p->m_count = nToInsert;
        return p;
    }
}
template<typename T>
int Multiset<T>::doErase(const T& value, bool all)
{
    Node* p = find(value);

    if (p == m_head)  // not found
        return 0;

    int nErased = (all ? p->m_count : 1);  // number to erase

      // If erasing one, and there are more than one, just decrement;
      // otherwise, we're erasing all, or erasing one whose count is 1,
      // so unlink the Node from the list and destroy it

    if (!all  &&  p->m_count > 1)
        p->m_count--;
    else
    {
        p->m_prev->m_next = p->m_next;
        p->m_next->m_prev = p->m_prev;
        delete p;

        m_uniqueSize--;
    }

    m_size -= nErased;
    return nErased;
}
#endif 