#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "sellers.hpp"
using namespace std;

int phi(int a, int b) {
    return (int) a != b;
}

vector<int> sellers(string txt, string pat, int r) {
     long n = txt.length();
     long m = pat.length();
     int** D = twoD(m+1);
     int last = 1, prev = 0;
     vector<int> occ;
     for (int j = 0; j < n; j++) {
	 D[last][0] = 0;
	 for (int i = 1; i <= m; i++) {
	     D[last][i] = min(min(D[prev][i-1]+phi(pat[i-1],txt[j]), D[prev][i]+1), D[last][i-1]+1);
	 }
	 if (D[last][m] <= r) {
	     occ.push_back(j);
	 }
	 last = (last+1) % 2;
	 prev = (prev+1) % 2;
     }
     return occ;
}

int** twoD(int n) {
    int** arr = new int*[2];
    arr[0] = new int[n];
    arr[1] = new int[n];
    for (int i = 0; i < n; i++) {
	arr[0][i] = i;
	arr[1][i] = i;
    }
    return arr;
}
/*
int main() {
    string txt = "abadac";
    string pat = "cada";
    int r = 2;
    vector<int> occ = sellers(txt, pat, r);
    for (int i = 0; i < occ.size(); i++) {
	cout << occ[i] << endl;
    }
}
*/
