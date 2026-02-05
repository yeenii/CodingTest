#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

const int n_max = 100005;

vector<int> arr;

int setPivot(int low, int high)
{
	int pivot = arr[high];
	int blueArrow = low - 1;

	for (int i = low; i < high; i++) //빨간색 화살표
	{
		if (pivot > arr[i])
		{
			blueArrow++;
			swap(arr[i], arr[blueArrow]); //파란색 - 빨간색 교환
		}
	}

	//빨간색 다 돌고 나서, 파란색 화살표 그다음 위치에 pivot 교환
	swap(arr[blueArrow + 1], arr[high]);

	return blueArrow + 1; //pivot 최종 위치 리턴

}

void QuickSort(int low, int high)
{
	if (low < high)
	{
		//1. pivot 구하기
		int pivot = setPivot(low, high);

		//2. pivot 기준 정렬 수행
		QuickSort(low, pivot - 1);
		QuickSort(pivot + 1, high);
	}
	

}

int main()
{
	int n;
	cin >> n;
	arr.resize(n);
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
	}

	//퀵정렬
	QuickSort(0, n-1);

	//퀵 정렬 출력
	for (auto a : arr)
		cout << a << " ";
	cout << endl;

	return 0;
}