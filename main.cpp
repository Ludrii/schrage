#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <functional>
#include <algorithm>

#define WEJ "schrage.data.txt"
#define WYJ "schrage.wynik.txt"

using namespace std;

struct Zadanie
{
	int id;
	int r;
	int p;
	int q;
};

struct greaterR
{
	bool operator()(const Zadanie &a, const Zadanie &b) const
	{
		return a.r > b.r;
	}
};

struct lesserQ
{
	bool operator()(const Zadanie &a, const Zadanie &b) const
	{
		return a.q < b.q;
	}
};

struct Kopiec
{
	vector<Zadanie> Zadania;

	Zadanie getRoot();
	void insertMax(Zadanie &Zad);
	void insertMin(Zadanie &Zad);
	void _delete(int i);
	void maxHeapify(int i);
	void minHeapify(int i);
	int parent(int i);
	int left(int i);
	int right(int i);
	void linearSearchR(Zadanie Zad);
	void linearSearchQ(Zadanie Zad);
};

void swap(Zadanie &a, Zadanie &b);

int schrage(int n, Zadanie *T, int *X);

int schrage(int n, vector<Zadanie> R);

int schrage(int n, Kopiec R);

int schrageZP(int n, Zadanie *T);

int schrageZP(int n, vector<Zadanie> R);

int schrageZP(int n, Kopiec R);

int main()
{
	int X[100], n, z;
	Zadanie T[100], Z[100];
	vector<Zadanie> H;
	Kopiec K;
	string s = "data.00", s1, s2;
	ifstream in(WEJ);
	ofstream out(WYJ);

	for (int i = 0; i < 9; i++)
	{
		s1 = s + to_string(i) + ":";
		while (s2 != s1)
		{
			in >> s2;
		}
		in >> n;
		for (int j = 0; j < n; j++)
		{
			T[j].id = j;
			in >> T[j].r >> T[j].p >> T[j].q;
			H.push_back(T[j]);
			Z[j] = T[j];
			K.insertMin(T[j]);
		}

		make_heap(H.begin(), H.end(), greaterR());
		K.minHeapify(0);

		out << "Zbior danych nr " << i << endl;
		z = schrage(n, T, X);
		out << "Schrage bez podziału na tablicy: " << z << endl;
		z = schrageZP(n, T);
		out << "Schrage z podziałem na tablicy: " << z << endl;
		z = schrage(n, H);
		out << "Schrage bez podziału na kopcu STL: " << z << endl;
		z = schrageZP(n, H);
		out << "Schrage z podziałem na kopcu STL: " << z << endl;
		z = schrage(n, K);
		out << "Schrage bez podziału na własnym kopcu: " << z << endl;
		z = schrageZP(n, K);
		out << "Schrage z podziałem na własnym kopcu: " << z << endl;
		H.clear();
		K.Zadania.clear();
	}
	cout << "Plik wynikowy - " << WYJ << endl;
}

void swap(Zadanie &a, Zadanie &b)
{
	swap(a.id, b.id);
	swap(a.p, b.p);
	swap(a.q, b.q);
	swap(a.r, b.r);
}

int schrage(int n, Zadanie *T, int *X)
{
	int R[100], Q[100];
	int r = n, q = 0, w = 0;
	int t = 0, cmax = 0;
	for (int i = 0; i < n; i++)
	{
		R[i] = i;
	}
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (T[R[j]].r < T[R[j + 1]].r)
			{
				swap(R[j], R[j + 1]);
			}
		}
	}
	while (w != n)
	{
		if (r != 0)
		{
			if (T[R[r - 1]].r <= t)
			{
				Q[q] = R[r - 1];
				q++;
				r--;
				for (int k = q - 1; k > 0; k--)
				{
					if (T[Q[k]].q < T[Q[k - 1]].q)
					{
						swap(Q[k], Q[k - 1]);
					}
				}
				continue;
			}
		}
		if (q != 0)
		{
			X[w] = Q[q - 1];
			t += T[X[w]].p;
			cmax = max(cmax, t + T[X[w]].q);
			q--;
			w++;
			continue;
		}
		if (q == 0 && T[R[r - 1]].r > t)
		{
			t = T[R[r - 1]].r;
		}
	}
	return cmax;
}

int schrageZP(int n, Zadanie *T)
{
	int R[100], Q[100], pom[100];
	int r = n, q = 0;
	int t = 0, cmax = 0;
	int poz = 100;
	int ile_zr = 0;
	for (int i = 0; i < n; i++)
	{
		pom[i] = T[i].p;
	}
	for (int i = 0; i < n; i++)
	{
		R[i] = i;
	}
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (T[R[j]].r < T[R[j + 1]].r)
			{
				swap(R[j], R[j + 1]);
			}
		}
	}
	while (r != 0 || q != 0)
	{
		if (r != 0)
		{
			if (T[R[r - 1]].r <= t)
			{
				Q[q] = R[r - 1];
				q++;
				r--;
				for (int k = q - 1; k > 0; k--)
				{
					if (T[Q[k]].q < T[Q[k - 1]].q)
					{
						swap(Q[k], Q[k - 1]);
					}
				}
				if (poz != 100)
				{
					if (T[Q[q - 1]].q > T[poz].q)
					{
						Q[q] = poz;
						swap(Q[q], Q[q - 1]);
						q++;
						poz = 100;
					}
				}
				continue;
			}
		}
		if (q != 0)
		{
			if (poz == 100)
			{
				poz = Q[q - 1];
				q--;
			}
			if (r != 0)
			{
				ile_zr = min(pom[poz], T[R[r - 1]].r - t);
			}
			else
			{
				ile_zr = pom[poz];
			}
			t += ile_zr;
			pom[poz] -= ile_zr;
			if (pom[poz] == 0)
			{
				cmax = max(cmax, t + T[poz].q);
				poz = 100;
			}
			continue;
		}
		if (q == 0 && r != 0)
		{
			if (T[R[r - 1]].r > t)
			{
				t = T[R[r - 1]].r;
			}
		}
	}
	return cmax;
}

int schrage(int n, vector<Zadanie> R)
{
	vector<Zadanie> Q;
	Zadanie Temp;
	int t = 0, cmax = 0;

	make_heap(Q.begin(), Q.end(), lesserQ());

	do
	{
		if (!R.empty() && R.front().r > t && Q.empty())
		{
			t = R.front().r;
		}
		while (!R.empty() && t >= R.front().r)
		{
			pop_heap(R.begin(), R.end(), greaterR());
			Temp = R.back();
			R.pop_back();
			Q.push_back(Temp);
			push_heap(Q.begin(), Q.end(), lesserQ());
		}
		pop_heap(Q.begin(), Q.end(), lesserQ());
		Temp = Q.back();
		Q.pop_back();
		t += Temp.p;
		cmax = max(cmax, t + Temp.q);
	} while (!Q.empty() || !R.empty());

	return cmax;
}

int schrageZP(int n, vector<Zadanie> R)
{
	vector<Zadanie> Q;
	Zadanie Temp;
	int t = 0, cmax = 0, p0 = 0, p1 = 0;

	make_heap(Q.begin(), Q.end(), lesserQ());

	do
	{
		if (!R.empty() && R.front().r > t && Q.empty())
		{
			t = R.front().r;
		}
		while (!R.empty() && t >= R.front().r)
		{
			pop_heap(R.begin(), R.end(), greaterR());
			Temp = R.back();
			R.pop_back();
			Q.push_back(Temp);
			push_heap(Q.begin(), Q.end(), lesserQ());
		}
		pop_heap(Q.begin(), Q.end(), lesserQ());
		Temp = Q.back();
		Q.pop_back();
		if (t + Temp.p <= R.front().r || R.empty())
		{
			t += Temp.p;
			cmax = max(cmax, t + Temp.q);
		}
		else
		{
			p0 = t;
			p1 = R.front().r;
			Temp.p -= p1 - p0;
			t = p1;
			Q.push_back(Temp);
			push_heap(Q.begin(), Q.end(), lesserQ());
		}
	} while (!Q.empty() || !R.empty());

	return cmax;
}

void Kopiec::linearSearchQ(Zadanie Zad)
{
	for (auto i : Zadania)
	{
		if (i.q == Zad.q)
			return;
	}
}

void Kopiec::linearSearchR(Zadanie Zad)
{
	for (auto i : Zadania)
	{
		if (i.r == Zad.r)
			return;
	}
}

int Kopiec::parent(int i)
{
	return (i - 1) / 2;
}

int Kopiec::left(int i)
{
	return (2 * i + 1);
}

int Kopiec::right(int i)
{
	return (2 * i + 2);
}

Zadanie Kopiec::getRoot()
{
	Zadanie Temp;

	swap(*Zadania.begin(), *(Zadania.end() - 1));
	Temp = *(Zadania.end() - 1);
	Zadania.pop_back();
	return Temp;
}

void Kopiec::minHeapify(int i)
{
	int l = left(i);
	int r = right(i);
	int smallest = i;
	if (l < Zadania.size() && Zadania.at(l).r < Zadania.at(i).r)
	{
		smallest = l;
	}
	if (r < Zadania.size() && Zadania.at(r).r < Zadania.at(smallest).r)
	{
		smallest = r;
	}
	if (smallest != i)
	{
		swap(Zadania.at(i), Zadania.at(smallest));
		minHeapify(smallest);
	}
}

void Kopiec::maxHeapify(int i)
{
	int l = left(i);
	int r = right(i);
	int biggest = i;
	if (l < Zadania.size() && Zadania.at(l).q > Zadania.at(i).q)
	{
		biggest = l;
	}
	if (r < Zadania.size() && Zadania.at(r).q > Zadania.at(biggest).q)
	{
		biggest = r;
	}
	if (biggest != i)
	{
		swap(Zadania.at(i), Zadania.at(biggest));
		maxHeapify(biggest);
	}
}

int schrage(int n, Kopiec R)
{
	Kopiec Q;
	Zadanie Temp;
	int t = 0, cmax = 0;

	Q.maxHeapify(0);

	do
	{
		if (!R.Zadania.empty() && R.Zadania.front().r > t && Q.Zadania.empty())
		{
			t = R.Zadania.front().r;
		}
		while (!R.Zadania.empty() && t >= R.Zadania.front().r)
		{
			Temp = R.getRoot();
			R.minHeapify(0);
			Q.insertMax(Temp);
		}
		Temp = Q.getRoot();
		Q.maxHeapify(0);
		t += Temp.p;
		cmax = max(cmax, t + Temp.q);
	} while (!Q.Zadania.empty() || !R.Zadania.empty());

	return cmax;
}

int schrageZP(int n, Kopiec R)
{
	Kopiec Q;
	Zadanie Temp;
	int t = 0, cmax = 0, p0 = 0, p1 = 0;

	Q.maxHeapify(0);

	do
	{
		if (!R.Zadania.empty() && R.Zadania.front().r > t && Q.Zadania.empty())
		{
			t = R.Zadania.front().r;
		}
		while (!R.Zadania.empty() && t >= R.Zadania.front().r)
		{
			Temp = R.getRoot();
			R.minHeapify(0);
			Q.insertMax(Temp);
		}
		Temp = Q.getRoot();
		Q.maxHeapify(0);
		if (t + Temp.p <= R.Zadania.front().r || R.Zadania.empty())
		{
			t += Temp.p;
			cmax = max(cmax, t + Temp.q);
		}
		else
		{
			p0 = t;
			p1 = R.Zadania.front().r;
			Temp.p -= p1 - p0;
			t = p1;
			Q.insertMax(Temp);
		}
	} while (!Q.Zadania.empty() || !R.Zadania.empty());

	return cmax;
}

void Kopiec::insertMax(Zadanie &Zad)
{
	int i = Zadania.size();

	Zadania.push_back(Zad);
	while (i > 0 && Zadania.at(parent(i)).q < Zadania.at(i).q)
	{
		swap(Zadania.at(i), Zadania.at(parent(i)));
		i = parent(i);
	}
}

void Kopiec::insertMin(Zadanie &Zad)
{
	int i = Zadania.size();

	Zadania.push_back(Zad);
	while (i > 0 && Zadania.at(parent(i)).r > Zadania.at(i).r)
	{
		swap(Zadania.at(i), Zadania.at(parent(i)));
		i = parent(i);
	}
}