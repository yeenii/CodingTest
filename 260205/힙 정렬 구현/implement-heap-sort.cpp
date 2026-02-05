// 원소 개수 : 6
// 배열: 5 2 6 1 3 8
//Heap Sort를 사용해 오름차순으로 정렬

#include<iostream>
#include<algorithm>

using namespace std;

const int n_max = 100005;
int n;
int arr[n_max];

void heapify(int n, int idx) 
{
	//(1) largest, 왼쪽, 오른쪽 자식 노드 인덱스
	int largest = idx;
	int l = idx * 2;
	int r = idx * 2 + 1;

	//(2) '자식노드 <=n && arr[자식노드] > arr[현재 노드] -> largest는 자식 노드'
	if (l <= n && arr[l] > arr[largest])
		largest = l;

	if (r <= n && arr[r] > arr[largest])
		largest = r;

	//(3) 최대 노드가 자식 노드일 경우
	if (largest != idx)
	{
		//1. 자식노드와 현재 노드 교환
		swap(arr[largest], arr[idx]);

		//2. 밑에 내려간 위치에서 다시 heapify 수행
		heapify(n, largest);
	}

}

void heapSort()
{
	//1. heapify
	for(int i=n/2; i>=1; i--) //(1) n/2 ...1 번째 노드까지 heapify 수행 
		heapify(n, i); //현재 노드 넘겨줌

	//2. 정렬
	for (int i = n; i > 1; i--)
	{
		//(1) 1번째 노드와 마지막 노드 교환
		swap(arr[1], arr[i]);

		//(2) n을 1 감소 시키고, 1번째 노드에 대한 heapify 수행
		heapify(i - 1, 1);
	}
}

int main()
{
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		cin >> arr[i]; //원소 배열에 저장
	}

	//1. 힙정렬
	heapSort();

	//정렬 결과 출력
	for (int i = 1; i <= n; i++)
		cout << arr[i] << " ";
	cout << endl;

	return 0;
}