//頂点数が64以上の場合, オーバーフローする
#include <iostream>
#include <vector>
#include <intrin.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>  

using namespace std;

//定数開始
vector<vector<long long int>> G;//入力されるグラフ
long long int n;//頂点数
long long int m;//枝数
vector<long long int> T;//関数
vector<long long int> B;//部分集合を表す配列//B[i]=1の時, 頂点iはYに属する
bool exist;//条件に該当するYがあればtrue,なければfalse.
long long int checker = 0;//今,何個目の部分集合が作られているかを表す
clock_t start;
//定数終了

//最終的に感染する頂点を求める関数(bitを用いない)
vector<bool> who_is_influenced_not_bit(vector<long long int> Y) {
	vector<bool> Influenced;//頂点iが感染していたらinfluenced[i]=true,そうでなければinfluenced[i]=falseである.
	Influenced.resize(n);
	for (long long int i = 0; i < n; i++) {
		Influenced[i] = false;
	}
	//1回目の拡散過程の実装開始.
	for (long long int i = 0; i < n; i++) {
		if (Y[i] == 0 && T[i] == 0)Influenced[i] = true;
	}
	//1回目の拡散過程の実装終了.
	//t回目の拡散過程の実装開始.
	bool changed = false;
	do {
		changed = false;
		for (long long int i = 0; i < n; i++) {
			long long int count = 0;//頂点uの隣接点で感染している頂点の数を数える.
			if (Y[i]==0 && !Influenced[i]) {
				for (long long int j = 0; j < G[i].size(); j++) {
					if (Influenced[G[i][j]])count++;
				}
				if (count >= T[i]) {
					Influenced[i] = true;
					changed = true;
				}
			}
		}
	} while (changed);
	//t回目の拡散過程の実装終了.
	return Influenced;
}

//部分集合を列挙し, その中で色々な処理を行う
void making_subgraph(long long int a, long long int k, long long int l) {//a番目を決める,a-1番目までは決まっている
	clock_t now2 = clock();
	if ((double)(now2 - start) / CLOCKS_PER_SEC >= 3600) {
		cout << (double)(now2 - start) / CLOCKS_PER_SEC << "sec.\n";
		cout << "Timeout!" << endl;
		return;
	}
	if (exist) return;
	if (a == n) {
		//ここで色々な処理を行う
		//cout << checker << endl;
		//checker++;
		long long int number_of_immunized = 0;
		for (long long int i = 0; i < n; i++) {
			if (B[i])number_of_immunized++;
		}
		if (number_of_immunized > l)return;
		vector<bool> influenced = who_is_influenced_not_bit(B);
		long long int number_of_influenced = 0;
		for (long long int i = 0; i < n; i++) {
			if (influenced[i])number_of_influenced++;
		}
		if (number_of_influenced <= k)exist = true;
		return;
	}
	else {
		B[a] = 0;
		making_subgraph(a + 1, k, l);
		B[a] = 1;
		making_subgraph(a + 1, k, l);
		return;
	}
}
//メイン関数

int main() {
	//入力開始
	ifstream ifs1("moreno_zebra.txt");

	if (!ifs1) {
		std::cout << "Error!";
		return 1;
	}

	string s;
	long long int count = 0;
	long long int x = 0;
	long long int y = 0;

	while (getline(ifs1, s, ' ')) {     // スペース（' '）で区切って，格納
		if (count == 0) {//頂点数
			n = stoll(s);
			G.resize(n);//グラフの大きさ確保
			count++;
		}
		else if (count == 1) {//枝数
			m = stoll(s);
			count++;
		}
		else if (count > 1 && count < 2 + 2 * m && count % 2 == 0) {//グラフの枝の端点
			x = stoll(s);
			x--;
			count++;
		}
		else if (count > 1 && count < 2 + 2 * m && count % 2 == 1) {//グラフの枝のもう一つの端点
			y = stoll(s);
			y--;
			count++;
			G[x].push_back(y);
			G[y].push_back(x);
		}
	}

	ifs1.close();

	ifstream ifs2("moreno_zebra_threshold.txt");

	if (!ifs2) {
		std::cout << "Error!";
		return 1;
	}

	B.resize(n);
	T.resize(n);
	count = 0;

	while (getline(ifs2, s, ' ')) {//頂点の閾値
		x = stoll(s);
		T[count] = x;
		count++;
	}

	ifs2.close();
	//入力終了


	//cout << (static_cast<long long>(1) << n) << endl;


	/*
	cout << "頂点数:" << n << endl;
	cout << "枝数:" << m << endl;
	cout << "k:" << k << endl;
	cout << "l:" << l << endl;
	long long int edge_number = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < G[i].size(); j++) {
			cout << "枝" << edge_number + 1 << "本目:" << i << " " << G[i][j] << endl;
			edge_number++;
		}
	}
	for (int i = 0; i < n; i++) {
		cout << "i: " << i << endl;
		cout << T[i] << endl;
	}

	cout << endl;
	*/
	start = clock();    //時間測定開始

	for (long long int parameter_sum = 0; parameter_sum < n + 1; parameter_sum++) {
		for (long long int k = 0; k < parameter_sum + 1; k++) {
			checker = 0;
			clock_t now = clock();
			if ((double)(now - start) / CLOCKS_PER_SEC >= 3600) {
				cout << (double)(now - start) / CLOCKS_PER_SEC << "sec.\n";
				cout << "Timeout!" << endl;
				return 0;
			}
			long long int l = parameter_sum - k;
			cout << "k: " << k << " l:" << l << endl;
			making_subgraph(0, k, l);
			if (exist) {
				clock_t Yesend = clock();
				cout << "Yes!" << endl;
				cout << (double)(Yesend - start) / CLOCKS_PER_SEC << "sec.\n";
				return 0;
			}
		}
	}
	clock_t Noend = clock();
	cout << (double)(Noend - start) / CLOCKS_PER_SEC << "sec.\n";
	cout << "No!" << endl;
	return 0;
}