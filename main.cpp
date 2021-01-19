#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <cmath>
#include <ctime>
#include <random>

using namespace std;

int main() {
    freopen("a.in", "r", stdin);
    int n, t1, t2, t3;
    cin>>n>>t1>>t2>>t3;
    vector < vector < string > > p(n);
    map<string,int> ingr;
    //vector < int > unic(n);
    for (int i = 0; i < n; i++) {
        int k;
        cin>>k;
        p[i].resize(k);
        for (int j = 0; j < k; j++) {
            cin>>p[i][j];
            ingr[p[i][j]]++;
        }
    }
    cout<<ingr.size()<<"\n";
}