#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>

using namespace std;

const int n_max = 1000;
int n, m;
int v1, v2;
unordered_map<int, vector<int>> map_list;
unordered_set<int> visited;
int result;

void dfs(int v)
{
	//방문처리 
	visited.insert(v);
	
	//이웃 방문 확인
	for (auto neighbor : map_list[v])
	{
		if (visited.find(neighbor) == visited.end()) //방문하지 않았다면
		{
			dfs(neighbor);
			result++;
		}
	}
}
int main(int argc, char** argv)
{
	//int test_case;
	//int T;
	
	//freopen("input.txt", "r", stdin);
	//cin >> T;
	
	//for (test_case = 1; test_case <= T; ++test_case)
	//{
		cin >> n >> m;

		for (int i = 0; i < m; i++)
		{
			cin >> v1 >> v2;
			//양방향 간선
			map_list[v1].push_back(v2);
			map_list[v2].push_back(v1);
		}

		//연결리스트 정렬
		for (auto map : map_list)
		{
			sort(map.second.begin(), map.second.end());
		}

		dfs(1); //dfs로 계산

		cout << result << endl;

		//결과 초기화 (테스트케이스 여러개)
		result = 0; 
		map_list.clear();
		visited.clear();
	//}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}