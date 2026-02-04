#include<iostream>
#include<vector>

using namespace std;

const int n_max = 100005;

vector<int> arr; //원소 배열
vector<int> meArr; //병합 정렬 결과 저장

void mergeNode(int low, int mid, int high)
{
	//두 배열 원소 가리키는 변수
	int i = low;
	int j = mid + 1;

	int k = low; //병합 배열 첫 원소

	
	while (i <= mid && j <= high) //두 배열에 원소가 남아있는 경우
	{
		//(1) 두 배열이 가리키는 값 중 작은 값을 병합 배열에 추가
		if (arr[i] <= arr[j])
		{
			meArr[k] = arr[i]; //**병합 배열에 추가
			k++; //인덱스 증가
			i++;
		}
		else {
			meArr[k] = arr[j];
			k++;
			j++;
		}
	}
	
	while (i <= mid) //(2) 왼쪽 배열에 남아있는 원소 병합 배열에 저장
	{
		meArr[k] = arr[i];
		k++;
		i++;
	}

	while (j <= high) //(3)오른쪽 배열에 남아있는 원소 병합 배열에 저장
	{
		meArr[k] = arr[j];
		k++;
		j++;
	}

	for (int t = low; t <= high; t++) //**(4) 병합 정렬한 결과 원래 배열에 복사
		arr[t] = meArr[t];

}

void mergeSort(int low, int high) //1. 쪼개기
{
	if (low < high) //원소가 2개 이상이라면
	{
		int mid = (low + high) / 2; //중간 값 구하기
		mergeSort(low, mid); //왼쪽 배열
		mergeSort(mid + 1, high); //오른쪽 배열
		mergeNode(low, mid, high); //2. 병합
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

	//**병합 배열 크기 미리 설정
	meArr.resize(n);

	//1. 쪼개기
	mergeSort(0, n-1);


	//병합 배열 결과 출력
	for (int i = 0; i < meArr.size(); i++)
		cout << meArr[i] << " ";
	cout << endl;


	return 0;
}