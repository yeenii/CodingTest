#include<iostream>
#include<vector>

using namespace std;

const int n_max = 105;
vector<int> lst;

void insertSort()
{
	//key와 그의 앞 원소 인덱스 설정
	for (int i = 1; i < lst.size(); i++)
	{
		int key = lst[i]; //key
		int preidx = i - 1; //key 앞 원소

		while (preidx >= 0 && lst[preidx] > key) //앞원소>=0 && 앞 원소가 key 값보다 큰 경우, 교환
		{
			lst[preidx + 1] = lst[preidx]; //key 있는 위치에 앞 원소로 교체
			preidx--;
		}

		//key 값 최종 위치 저장
		lst[preidx + 1] = key;
	}
}

int main()
{

	int n;
	cin >> n;

	for (int i = 0; i < n; i++) //원소 삽입
	{
		int l;
		cin >> l;
		lst.push_back(l);
	}

	//삽입 정렬
	insertSort();

	//결과 출력
	for (int i = 0; i < lst.size(); i++)
		cout << lst[i] << " ";
	cout << endl;


	return 0;
}