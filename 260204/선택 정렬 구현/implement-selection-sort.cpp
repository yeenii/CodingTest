#include<iostream>
#include<vector>

using namespace std;

const int n_max = 105;
vector<int> arr;

void selectionSort()
{
	for (int i = 0; i < arr.size()-1; i++) //0~n-2
	{
		int min_num = i;
		for (int j = i + 1; j < arr.size(); j++) //0~n-1
		{
			if (arr[min_num] > arr[j]) //현재 가장 작은 원소 값 > 뒷 원소 값 -> 교환
			{
				int temp = arr[min_num];
				arr[min_num] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

int main()
{
	int n;
	cin >> n;

	for (int i = 0; i < n; i++) //원소 삽입
	{
		int a;
		cin >> a;
		arr.push_back(a);
	}

	selectionSort();

	for (int i = 0; i < arr.size(); i++)
		cout << arr[i] << " ";
	cout << endl;


	return 0;
}