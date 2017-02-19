#include <cassert>
int countIncludes(const char a1[], int n1, const char a2[], int n2)
{
	if (n2==0)
		return 1;
	if(n1<0 || n2<0 || n1<n2)
		return 0;
	if(a1[0]==a2[0])
		if(n2==1)
			return countIncludes(a1+1,n1-1,a2,n2)+1;
		else
			return countIncludes(a1+1,n1-1,a2,n2)+countIncludes(a1+1,n1-1,a2+1,n2-1);
	return countIncludes(a1+1,n1-1,a2,n2);
}

int main()
{
	char a[10] = { 'h', 'o', 'D', 'r', 'U', 's', 'C', 'K', 'e', '@' };
	char x[10] = { 'H', 'N', 'J', 'P', 'D', 'F', 'D', 'D', 'S', 'M' };
	char c[8] = { 'a', 'u', 'o', 'e', 'u', 'o', 'i' };
	char d[12] = { 'a', 'e', 'o', 'a', 'o', 'i', 'e', 'a', 'o', 'u', 'o', 'i' };
	assert(countIncludes(c, 7, d, 3) == 1);
	assert(countIncludes(c, 7, d+3, 3) == 2);
	assert(countIncludes(c, 7, d+6, 3) == 0);
	assert(countIncludes(c, 7, d+9, 3) == 3);
	assert(countIncludes(c, 7, d+2, 1) == 2);
	assert(countIncludes(c, 2, d+2, 1) == 0);
	assert(countIncludes(c, 7, d, 0) == 1);
	assert(countIncludes(c, 0, d, 0) == 1);
}
