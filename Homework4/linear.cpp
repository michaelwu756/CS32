 #include<cctype>
 bool anyUppercase(const char a[], int n)
{
	if(n<=0 || a[0]=='\0')
		return false;
	if(isupper(a[0]))
		return true;
	return anyUppercase(a+1,n-1);
}
int countUppercase(const char a[], int n)
{
	if(n<=0 || a[0]=='\0')
		return 0;
	int upperInRestOfString = countUppercase(a+1,n-1);
	if(isupper(a[0]))
		upperInRestOfString++;
	return upperInRestOfString;
}
int firstUppercase(const char a[], int n)
{
	if(n<=0 || a[0]=='\0')
		return -1;
	if(isupper(a[0]))
		return 0;
	int firstUpperIndex = firstUppercase(a+1,n-1);
	if(firstUpperIndex==-1)
		return -1;
	firstUpperIndex++;
	return firstUpperIndex;
}
int indexOfLeastRecursive(const char a[], int n, int index, int lowestIndex)
{
	if(index>=n)
		return lowestIndex;
	if(a[index]<a[lowestIndex])
		lowestIndex=index;
	return indexOfLeastRecursive(a,n,index+1,lowestIndex);
	
}
int indexOfLeast(const char a[], int n)
{
	if(n<=0)
		return -1;
	return indexOfLeastRecursive(a,n,0,0);
}