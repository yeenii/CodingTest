#include<iostream>
#include<algorithm> //swap()

using namespace std;

const int n_max = 100005;
const int arr_max = 100005;

int arr[arr_max] = {};
int n; //총 노드 개수

void heapify(int n, int i)
{
	//1. 최대 노드, 왼쪽 자식 노드, 오른쪽 자식 노드 정의
	int largest = i; //현재 노드가 최대 노드로 초기화
	int lft = i * 2; //인덱스 1로 시작할 때
	int rgt = i * 2 + 1;

	//2. 비교
	if (lft <= n && arr[lft] > arr[largest])
		largest = lft;
	
	if (rgt <= n && arr[rgt] > arr[largest])
		largest = rgt;

	//3. 최대 노드가 자식노드인 경우
	if (largest != i)
	{
		swap(arr[i], arr[largest]);
		heapify(n, largest); //내려온 자리에서 다시 heapify()
	}

}

void HeapSort()
{
	//1. max-heap
	for(int i=n/2; i>=1; i--) //n/2 .. 1
		heapify(n, i); //총 노드 개수, n/2 ... 1번째 노드


	//2. 정렬
	for (int j = n; j >= 1; j--) //n ... 1
	{
		swap(arr[1], arr[j]); //1번 노드와 n번 노드 교환
		heapify(j-1, 1); //1번 노드 기준으로 heapify() 다시 진행
	}

}

int main()
{
	
	cin >> n; //n개의 원소

	for (int i = 1; i <= n; i++)
	{
		cin >> arr[i]; //1~n번까지 노드 입력
	}
		
	/*for (int i = 0; i <= n; i++)
		cout << arr[i] << " ";
	cout << endl;*/

	HeapSort(); //힙정렬

	for (int i = 1; i <= n; i++)
		cout << arr[i] << " ";
	cout << endl;


	return 0;
}