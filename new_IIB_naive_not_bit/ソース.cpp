//���_����64�ȏ�̏ꍇ, �I�[�o�[�t���[����
#include <iostream>
#include <vector>
#include <intrin.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>  

using namespace std;

//�萔�J�n
vector<vector<long long int>> G;//���͂����O���t
long long int n;//���_��
long long int m;//�}��
vector<long long int> T;//�֐�
vector<long long int> B;//�����W����\���z��//B[i]=1�̎�, ���_i��Y�ɑ�����
bool exist;//�����ɊY������Y�������true,�Ȃ����false.
long long int checker = 0;//��,���ڂ̕����W��������Ă��邩��\��
clock_t start;
//�萔�I��

//�ŏI�I�Ɋ������钸�_�����߂�֐�(bit��p���Ȃ�)
vector<bool> who_is_influenced_not_bit(vector<long long int> Y) {
	vector<bool> Influenced;//���_i���������Ă�����influenced[i]=true,�����łȂ����influenced[i]=false�ł���.
	Influenced.resize(n);
	for (long long int i = 0; i < n; i++) {
		Influenced[i] = false;
	}
	//1��ڂ̊g�U�ߒ��̎����J�n.
	for (long long int i = 0; i < n; i++) {
		if (Y[i] == 0 && T[i] == 0)Influenced[i] = true;
	}
	//1��ڂ̊g�U�ߒ��̎����I��.
	//t��ڂ̊g�U�ߒ��̎����J�n.
	bool changed = false;
	do {
		changed = false;
		for (long long int i = 0; i < n; i++) {
			long long int count = 0;//���_u�̗אړ_�Ŋ������Ă��钸�_�̐��𐔂���.
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
	//t��ڂ̊g�U�ߒ��̎����I��.
	return Influenced;
}

//�����W����񋓂�, ���̒��ŐF�X�ȏ������s��
void making_subgraph(long long int a, long long int k, long long int l) {//a�Ԗڂ����߂�,a-1�Ԗڂ܂ł͌��܂��Ă���
	clock_t now2 = clock();
	if ((double)(now2 - start) / CLOCKS_PER_SEC >= 3600) {
		cout << (double)(now2 - start) / CLOCKS_PER_SEC << "sec.\n";
		cout << "Timeout!" << endl;
		return;
	}
	if (exist) return;
	if (a == n) {
		//�����ŐF�X�ȏ������s��
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
//���C���֐�

int main() {
	//���͊J�n
	ifstream ifs1("moreno_zebra.txt");

	if (!ifs1) {
		std::cout << "Error!";
		return 1;
	}

	string s;
	long long int count = 0;
	long long int x = 0;
	long long int y = 0;

	while (getline(ifs1, s, ' ')) {     // �X�y�[�X�i' '�j�ŋ�؂��āC�i�[
		if (count == 0) {//���_��
			n = stoll(s);
			G.resize(n);//�O���t�̑傫���m��
			count++;
		}
		else if (count == 1) {//�}��
			m = stoll(s);
			count++;
		}
		else if (count > 1 && count < 2 + 2 * m && count % 2 == 0) {//�O���t�̎}�̒[�_
			x = stoll(s);
			x--;
			count++;
		}
		else if (count > 1 && count < 2 + 2 * m && count % 2 == 1) {//�O���t�̎}�̂�����̒[�_
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

	while (getline(ifs2, s, ' ')) {//���_��臒l
		x = stoll(s);
		T[count] = x;
		count++;
	}

	ifs2.close();
	//���͏I��


	//cout << (static_cast<long long>(1) << n) << endl;


	/*
	cout << "���_��:" << n << endl;
	cout << "�}��:" << m << endl;
	cout << "k:" << k << endl;
	cout << "l:" << l << endl;
	long long int edge_number = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < G[i].size(); j++) {
			cout << "�}" << edge_number + 1 << "�{��:" << i << " " << G[i][j] << endl;
			edge_number++;
		}
	}
	for (int i = 0; i < n; i++) {
		cout << "i: " << i << endl;
		cout << T[i] << endl;
	}

	cout << endl;
	*/
	start = clock();    //���ԑ���J�n

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