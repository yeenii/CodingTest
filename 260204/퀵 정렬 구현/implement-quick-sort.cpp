#include<iostream>
#include<algorithm> //swap()

using namespace std;

const int n_max = 100005;
const int arr_max = 100005;
int arr[n_max] = {};

int partition(int low, int high)
{
	int pivot = arr[high]; //pivot 초기화 (맨 끝 원소 값)
	int blueArrow = low-1; //파란색 화살표 초기화(첫 원소 -1)

	for (int i = low; i < high; i++) //빨간색 화살표
	{
		if (arr[i] < pivot)
		{
			blueArrow++; //파란색 화살표 한 칸 이동
			swap(arr[i], arr[blueArrow]); //파란색 화살표 원소와 빨간색 화살표 원소 교환
		}
	}

	//다 돌고나면, pivot과 파란색 화살표 그 다음 원소와 교환
	swap(arr[high], arr[blueArrow + 1]);

	return blueArrow + 1; //교체된 pivot 위치
}

void quickSort(int low,int high)
{
	if (low < high)
	{
		int pivot = partition(low, high); //1. pivot 위치 구하기

		//2. pivot 기준 왼쪽/오른쪽 정렬(재귀)
		quickSort(low, pivot-1); 
		quickSort(pivot+1, high);
	}
}

int main()
{
	int n;
	cin >> n; //n개의 원소

	for (int i = 0; i < n; i++) //배열에 원소 채우기
	{
		cin >> arr[i];
	}
		

	//퀵 정렬을 이용해 n개의 숫자 오름차순 정렬
	quickSort(0, n-1); //0~n-1까지

	for (int i = 0; i < n; i++) //배열 출력
		cout << arr[i] <<" ";
	cout << endl;

	return 0;
}