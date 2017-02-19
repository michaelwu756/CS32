#include<iostream>
using namespace std;

int merge(int data[], int n1, int n2)
{
	int i=0, j=0, k=0;
	int *temp = new int[n1+n2];
	int *sechalf = data+n1;
	int inversions = 0;

	while (i < n1 || j < n2)
	{
		if (i == n1)
			temp[k++] = sechalf[j++];
		else if (j == n2)
			temp[k++] = data[i++];
		else if (data[i] < sechalf[j])
			temp[k++] = data[i++];
		else
		{
			temp[k++] = sechalf[j++];
			inversions+=n1-i;
		}
	}
	for (i=0;i<n1+n2;i++)
		data[i] = temp[i];
	delete [] temp;
	return inversions;
}
int mergeCountInversions(int* arr, int size)
{
	if (size <=1)
		return 0;
	int mid=size/2;
	int inversions=0;
	inversions+=mergeCountInversions(arr,mid);
	inversions+=mergeCountInversions(arr+mid,size-mid);
	inversions+=merge(arr,mid,size-mid);
	return inversions;
}
int getInversions(int const * arr, int size)
{
	int * temp = new int[size];
	for(int i=0;i<size;i++)
		temp[i]=arr[i];
	int inv= mergeCountInversions(temp,size);
	delete []temp;
	return inv;
}
int main()
{
	int arr[]={2,1,5,4,3,};
	int size = 5;
	cout<<getInversions(arr,size)<<endl;
	for(int i=0;i<size;i++)
		cout<<arr[i]<<endl;
}