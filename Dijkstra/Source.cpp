#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int MAX = 20;

struct Node {
	int dinhKe;
	int trongSo;
	Node* link;
};

struct MaTranKe {
	int mtk[MAX][MAX];
	int soDinh;
};

struct DanhSachKe {
	Node* dsk[MAX];
	int soDinh;
};

bool docMaTranKe(const char* file, MaTranKe& mt) {
	FILE* f = fopen(file, "rt");
	if (f == NULL) return false;
	fscanf(f, "%d", &mt.soDinh);
	for (int i = 0; i < mt.soDinh; i++) {
		for (int j = 0; j < mt.soDinh; j++) {
			fscanf(f, "%d", &mt.mtk[i][j]);
		}
	}
	fclose(f);
	return true;
}

void xuatMaTranKe(MaTranKe mt) {
	for (int i = 0; i < mt.soDinh; i++)
	{
		cout << endl;
		for (int j = 0; j < mt.soDinh; j++) {
			cout << mt.mtk[i][j] << " ";
		}
	}
}

bool docDanhSach(const char* fileName, DanhSachKe& ds) {
	FILE* f = fopen(fileName, "rt");
	if (f == NULL)
		return false;

	for (int i = 0; i < MAX; i++)
	{
		ds.dsk[i] = NULL;
	}

	fscanf(f, "%d", &ds.soDinh);

	for (int i = 0; i < ds.soDinh; i++)
	{
		int soDinhKe;
		fscanf(f, "%d", &soDinhKe);
		for (int j = 0; j < soDinhKe; j++)
		{
			Node* p = new Node;
			fscanf(f, "%d%d", &p->dinhKe, &p->trongSo);
			p->link = NULL;
			if (ds.dsk[i] == NULL)
				ds.dsk[i] = p;
			else {
				Node* t = ds.dsk[i];
				while (t->link != NULL) {
					t = t->link;
				}
				t->link = p;
			}
		}
	}
	fclose(f);
	return true;
}

void xuatDanhSach(DanhSachKe ds) {
	for (int i = 0; i < ds.soDinh; i++)
	{
		cout << "\n(" << i << "): ";
		Node* p = ds.dsk[i];
		while (p != NULL) {
			cout << "(" << p->dinhKe << "," << p->trongSo << ")" << " ";
			p = p->link;
		}
	}
}

const int INF = 1e9;
void Dijkstra_DS(int dinhBatDau, DanhSachKe ds) {

	vector<bool> visited(ds.soDinh, false);
	vector<int>khoangCach(ds.soDinh, INF);

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> hangDoiUuTien;

	hangDoiUuTien.push({ dinhBatDau,0 });

	khoangCach[dinhBatDau] = 0;

	while (!hangDoiUuTien.empty()) {
		pair<int, int> dinhHienTai = hangDoiUuTien.top();
		hangDoiUuTien.pop();

		if (visited[dinhHienTai.first] == true)
			continue;

		visited[dinhHienTai.first] = true;

		//Duyet DSK cua dinh hien tai
		Node* p = ds.dsk[dinhHienTai.first];
		while (p) {
			if (p->trongSo != INF && khoangCach[p->dinhKe] > dinhHienTai.second + p->trongSo) { //khoangCach[dinhHienTai.first] == dinhHienTai.second
				khoangCach[p->dinhKe] = dinhHienTai.second + p->trongSo;
				hangDoiUuTien.push({ p->dinhKe,khoangCach[p->dinhKe] });
			}
			p = p->link;
		}
	}
	for (int i = 0; i < khoangCach.size(); i++)
	{
		cout << khoangCach[i] << " ";
	}
}


void Dijkstra_MT(int dinhBatDau, MaTranKe mt) {
	vector<bool> visited(mt.soDinh, false);
	vector<int> khoangCach(mt.soDinh, INF);

	khoangCach[dinhBatDau] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> hangDoiUuTien;

	hangDoiUuTien.push({ dinhBatDau,0 });

	while (!hangDoiUuTien.empty()) {
		pair<int, int> dinhHienTai = hangDoiUuTien.top();
		hangDoiUuTien.pop();

		if (visited[dinhHienTai.first] == true)
			continue;
		visited[dinhHienTai.first] = true;

		for (int i = 0; i < mt.soDinh; i++)
		{
			int trongSo = mt.mtk[dinhHienTai.first][i];
			if (trongSo != INF && trongSo != 0 && trongSo > 0 && khoangCach[i] > dinhHienTai.second + trongSo) {
				khoangCach[i] = dinhHienTai.second + trongSo;
				hangDoiUuTien.push({ i, khoangCach[i] });
			}
		}
	}

	for (int i = 0; i < khoangCach.size(); i++) {
		if (khoangCach[i] == INF) {
			cout << "INF ";
		}
		else {
			cout << khoangCach[i] << " ";
		}
	}
}


int main() {
	DanhSachKe DSK_Co_Huong;
	if (docDanhSach("DSK_Co_Huong.txt", DSK_Co_Huong)) {
		cout << "DANH SACH KE CO HUONG:";
		xuatDanhSach(DSK_Co_Huong);


		int dinhBatDau;
		do {
			cout << "\nNhap dinh bat dau: ";
			cin >> dinhBatDau;
		} while (dinhBatDau < 0 || dinhBatDau >= DSK_Co_Huong.soDinh);

		cout << "Duong di ngan nhat thuat toan Dijkstra cua danh sach ke: ";

		Dijkstra_DS(dinhBatDau, DSK_Co_Huong);
	}
	else
		cout << "\nKhong doc duoc danh sach!!!";

	cout << "\n\n--------------------------------------------------------------------------------------------\n\n";

	MaTranKe MTK_Co_Huong;
	if (docMaTranKe("MTK_Co_Huong.txt", MTK_Co_Huong)) {
		cout << "MA TRAN KE CO HUONG: ";
		xuatMaTranKe(MTK_Co_Huong);


		int dinhBatDau;
		do {
			cout << "\nNhap dinh bat dau: ";
			cin >> dinhBatDau;
		} while (dinhBatDau < 0 || dinhBatDau >= MTK_Co_Huong.soDinh);

		cout << "Duong di ngan nhat thuat toan Dijkstra cua ma tran ke: ";
		Dijkstra_MT(dinhBatDau, MTK_Co_Huong);
	}


	cout << "\n\n";
	return 0;
}