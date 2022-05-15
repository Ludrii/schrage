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
	bool operator()(const Zadanie& a, const Zadanie& b) const
	{
		return a.r>b.r;
	}
};

struct lesserQ
{
	bool operator()(const Zadanie& a, const Zadanie& b) const
	{
		return a.q<b.q;
	}
};

void swap(Zadanie& a, Zadanie& b);

int schrage(int n, Zadanie* T, int* X);

int schrage(int n, vector<Zadanie> R, int* X);

int schrageZP(int n, Zadanie* T);

int main()
{
	int X[100], n, z;
	Zadanie T[100], Z[100];
	vector<Zadanie> H;
	Zadanie Temp;
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
			Z[j]=T[j];
		}

		make_heap(H.begin(),H.end(),greaterR());

		out << "Zbior danych nr " << i << endl;
		z = schrage(n, T, X);
		out << "Schrage bez podziału na tablicy: " << z << endl;
		z = schrageZP(n, T);
		out << "Schrage z podziałem na tablicy: " << z << endl;
		z = schrage(n, H, X);
		out << "Schrage bez podziału na kopcu STL: " << z <<endl;
		H.clear();
	}
	cout << "Plik wynikowy - " << WYJ << endl;
}

void swap(Zadanie& a, Zadanie& b)
{
	swap(a.id, b.id);
	swap(a.p, b.p);
	swap(a.q, b.q);
	swap(a.r, b.r);
}

int schrage(int n, Zadanie* T, int* X)
{
	int R[100], Q[100];
	int r = n, q = 0, w = 0; 
	int  t = 0, cmax = 0;
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

int schrageZP(int n, Zadanie* T)
{
	int R[100], Q[100],pom[100];
	int r = n, q = 0;
	int  t = 0, cmax = 0;
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
			if (T[R[r-1]].r <= t)
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
			if (pom[poz]== 0)
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

int schrage(int n,vector<Zadanie> R, int* X)
{
	vector<Zadanie> Q;
	Zadanie Temp;
	int r = n, q = 0, w = 0; 
	int  t = 0, cmax = 0, cj=0;
	
	while(!R.empty())
	{
		pop_heap(R.begin(), R.end(), greaterR());
		Temp=R.back();
		R.pop_back();
		if(t<Temp.r)
		{
			t=Temp.r;
		}
		Q.push_back(Temp);
		t+=Temp.p;
		cmax=max(cmax,t + Temp.q);
	}

	return cmax;
}