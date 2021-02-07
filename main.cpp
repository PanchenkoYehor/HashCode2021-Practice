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

int n, t1, t2, t3;
vector < vector < string > > p;
map<string,int> ingredients;
map<string,double> uniquenessOfIngredients;
vector < double > uniquenessOfPizzas;
vector < vector < int > > answer;
vector < int > nonused;

void Read(const string& in) {
    ifstream FileIn("C:\\Users\\panen\\CLionProjects\\HashCodeTest1\\" + in);
    //freopen("C:\\Users\\panen\\CLionProjects\\HashCodeTest1\\" + in, "r", stdin);
    FileIn>>n>>t1>>t2>>t3;
    p.resize(n);
    //vector < int > unic(n);
    for (int i = 0; i < n; i++) {
        int k;
        FileIn>>k;
        p[i].resize(k);
        for (int j = 0; j < k; j++) {
            FileIn>>p[i][j];
            ingredients[p[i][j]]++;
        }
    }
    FileIn.close();
}

int sqr(int x) {
    return x * x;
}

long long sqr(long long x) {
    return x * x;
}

long long CalculateAnswer() {
    long long res = 0;
    for (auto &t : answer) {
        set < string > curr;
        for (auto &piz : t) {
            for (auto &ing : p[piz]) {
                curr.insert(ing);
            }
        }
        res += sqr((long long)curr.size());
    }
    return res;
}

void PrintAnswer(const string& out) {
    cout<<CalculateAnswer()<<"\n";
    ofstream FileOut("C:\\Users\\panen\\CLionProjects\\HashCodeTest1\\" + out);
    FileOut<<answer.size()<<"\n";
    for (auto &t : answer) {
        FileOut<<t.size()<<" ";
        for (auto &piz : t) {
            FileOut<<piz<<" ";
        }
        FileOut<<"\n";
    }
    FileOut.close();
}

void PropagateAnswer() {
    if (answer.size() == 1)
        return;
    srand(time(0));
    int team1 = rand() % answer.size();
    int team2;
    do {
        team2 = rand() % answer.size();
    } while (team1 == team2);
    int pizza1 = rand() % answer[team1].size();
    int pizza2 = rand() % answer[team2].size();
    set < string > curr1, curr2, after1, after2;

    for (auto &piz : answer[team1]) {
        for (auto &ing : p[piz]) {
            curr1.insert(ing);
        }
    }

    for (auto &piz : answer[team2]) {
        for (auto &ing : p[piz]) {
            curr2.insert(ing);
        }
    }

    for (auto &piz : answer[team1]) {
        if (piz == answer[team1][pizza1])
            continue;
        for (auto &ing : p[piz]) {
            after1.insert(ing);
        }
    }
    for (auto &ing : p[answer[team2][pizza2]]) {
        after1.insert(ing);
    }

    for (auto &piz : answer[team2]) {
        if (piz == answer[team2][pizza2])
            continue;
        for (auto &ing : p[piz]) {
            after2.insert(ing);
        }
    }
    for (auto &ing : p[answer[team1][pizza1]]) {
        after2.insert(ing);
    }

    if (sqr((long long)after1.size()) + sqr((long long)after2.size()) >
    sqr((long long)curr1.size()) + sqr((long long)curr2.size())) {
        swap(answer[team1][pizza1], answer[team2][pizza2]);
    }
}

void PropagateAnswerWithRest() {
    if (answer.size() == 1)
        return;
    srand(time(0));
    int team1 = rand() % answer.size();
    //team 2 - is rest of pizzas

    int pizza1 = rand() % answer[team1].size();
    int pizza2 = rand() % nonused.size();
    set < string > curr1, after1;

    for (auto &piz : answer[team1]) {
        for (auto &ing : p[piz]) {
            curr1.insert(ing);
        }
        if (piz != answer[team1][pizza1]) {
            for (auto &ing : p[piz]) {
                after1.insert(ing);
            }
        }
    }
    for (auto &ing : p[nonused[pizza2]])
        after1.insert(ing);

    if (after1.size() > curr1.size()) {
        swap(answer[team1][pizza1], nonused[pizza2]);
    }
}

void PropagateAnswerWithTwoTeams() {
    if (answer.size() == 1)
        return;
    srand(time(0));
    int team1 = rand() % answer.size();
    int team2;
    do {
        team2 = rand() % answer.size();
    } while (team1 == team2);

    set < int > allpizzas;
    vector < int > pizzas;
    for (auto &piz : answer[team1])
        pizzas.push_back(piz);
    for (auto &piz : answer[team2])
        pizzas.push_back(piz);
    for (auto &p : pizzas) {
        allpizzas.insert(p);
    }

    vector < int > pos;
    for (int mask = 0; mask < (1<<pizzas.size()); mask++) {
        if (__builtin_popcount(mask) == answer[team1].size()) {
            pos.push_back(mask);
        }
    }

    vector < int > bestforteam1;
    for (auto &piz : answer[team1])
        bestforteam1.push_back(piz);
    long long count = 0;

    for (auto mask : pos) {
        set < string > curr1, curr2;
        vector < int > ones;

        for (int ind = 0; ind < pizzas.size(); mask /= 2, ind++) {
            if (mask % 2 == 1) {
                ones.push_back(ind);
                for (auto &ing : p[pizzas[ind]]) {
                    curr1.insert(ing);
                }
            } else {
                for (auto &ing : p[pizzas[ind]]) {
                    curr2.insert(ing);
                }
            }
        }

        long long val = sqr((long long)curr1.size()) + sqr((long long)curr2.size());
        if (val > count) {
            count = val;
            for (int ind = 0; ind < ones.size(); ind++) {
                bestforteam1[ind] = pizzas[ones[ind]];
            }
        }

    }

    for (int ind = 0; ind < answer[team1].size(); ind++) {
        answer[team1][ind] = bestforteam1[ind];
        allpizzas.erase(answer[team1][ind]);
    }

    answer[team2].clear();
    for (auto &p : allpizzas) {
        answer[team2].push_back(p);
    }

}

void PropagateAnswerWithAllTwoTeams() {
    if (answer.size() == 1)
        return;
    srand(time(0));
    /*srand(time(0));
    int team1 = rand() % answer.size();
    int team2;
    do {
        team2 = rand() % answer.size();
    } while (team1 == team2);*/
    for (int team1 = 0; team1 < answer.size();
    team1 += 1*rand() % 10/*(int)sqrt(answer.size())*/) {

        if (team1 % 1 == 0) {
            cout << "team: " << team1 << "/" << answer.size() << "\n";
        }

        for (int team2 = team1 + 1; team2 < answer.size();
        team2 += 1*rand() % /*(int)sqrt(answer.size())*/ 10) {

            /*if (team2 % 1 == 0) {
                cout << "team2: " << team2 << "/" << answer.size() << "\n";
            }*/

            set<int> allpizzas;
            vector<int> pizzas;
            for (auto &piz : answer[team1])
                pizzas.push_back(piz);
            for (auto &piz : answer[team2])
                pizzas.push_back(piz);
            for (auto &pizza : pizzas) {
                allpizzas.insert(pizza);
            }

            vector<int> pos;
            for (int mask = 0; mask < (1 << pizzas.size()); ++mask) {
                if (__builtin_popcount(mask) == answer[team1].size()) {
                    pos.push_back(mask);
                }
            }

            vector<int> bestforteam1;
            for (auto piz : answer[team1])
                bestforteam1.push_back(piz);
            long long count = 0;

            for (auto mask : pos) {
                set<string> curr1, curr2;
                vector<int> ones;

                for (int ind = 0; ind < pizzas.size(); mask >>= 1, ++ind) {
                    if (mask & 1) {
                        ones.push_back(ind);
                        for (auto &ing : p[pizzas[ind]]) {
                            curr1.insert(ing);
                        }
                    } else {
                        for (auto &ing : p[pizzas[ind]]) {
                            curr2.insert(ing);
                        }
                    }
                }

                long long val = sqr((long long) curr1.size()) + sqr((long long) curr2.size());
                if (val > count) {
                    count = val;
                    for (int ind = 0; ind < ones.size(); ++ind) {
                        bestforteam1[ind] = pizzas[ones[ind]];
                    }
                }

            }

            for (int ind = 0; ind < answer[team1].size(); ++ind) {
                answer[team1][ind] = bestforteam1[ind];
                allpizzas.erase(answer[team1][ind]);
            }

            answer[team2].clear();
            for (auto pp : allpizzas) {
                answer[team2].push_back(pp);
            }
        }
    }

}

int pow3(int x) {
    return x * x * x;
}

void PropagateAnswerWithAllThreeTeams() {
    if (answer.size() < 3)
        return;
    srand(time(0));

    for (int team1 = 0; team1 < answer.size(); ++team1) {
        if (team1 % 1 == 0) {
            cout << "team1: " << team1 << "/" << answer.size() << "\n";
        }
        for (int team2 = team1 + 1; team2 < answer.size(); team2 += rand() % 50) {
            /*if (team2 % 1 == 0) {
                cout<<"\tteam2: "<<team2<<"\n";
            }*/
            for (int team3 = team2 + 1; team3 < answer.size(); team3 += rand() % 50) {
                /*if (team3 % 200 == 0) {
                    cout<<"\t\tteam2: "<<team3<<"\n";
                }*/
                set<int> allpizzas;
                vector<int> pizzas;
                for (auto &piz : answer[team1])
                    pizzas.push_back(piz);
                for (auto &piz : answer[team2])
                    pizzas.push_back(piz);
                for (auto &piz : answer[team3])
                    pizzas.push_back(piz);
                for (auto &p : pizzas) {
                    allpizzas.insert(p);
                }
                //cout<<"work\n";
                vector<int> pos;
                auto poww3 = pow3(pizzas.size());
                for (int mask = 0; mask < poww3; ++mask) {
                    int mmask = mask;
                    int cnt1 = 0, cnt2 = 0;
                    while (mmask) {
                        if (mmask % 3 == 1) {
                            ++cnt1;
                            if (cnt1 > answer[team1].size())
                                break;
                        }
                        else if (mmask % 3 == 2) {
                            ++cnt2;
                            if (cnt2 > answer[team2].size())
                                break;
                        }
                        mmask /= 3;
                    }
                    if (cnt1 == answer[team1].size() && cnt2 == answer[team2].size()) {
                        pos.push_back(mask);
                    }
                }

                vector<int> bestforteam1;
                vector<int> bestforteam2;
                for (auto &piz : answer[team1])
                    bestforteam1.push_back(piz);
                for (auto &piz : answer[team2])
                    bestforteam2.push_back(piz);
                long long count = 0;
                {
                    set<string> steam1;
                    for (auto &piz : answer[team1])
                        for (auto &ing : p[piz]) {
                            steam1.insert(ing);
                        }
                    set<string> steam2;
                    for (auto &piz : answer[team2])
                        for (auto &ing : p[piz]) {
                            steam2.insert(ing);
                        }
                    set<string> steam3;
                    for (auto &piz : answer[team3])
                        for (auto &ing : p[piz]) {
                            steam3.insert(ing);
                        }
                    count += sqr((long long) steam1.size());
                    count += sqr((long long) steam2.size());
                    count += sqr((long long) steam3.size());
                }
                long long diff = -count;

                for (auto mask : pos) {
                    set<string> curr1, curr2, curr3;
                    vector<int> ones;
                    vector<int> twos;

                    for (int ind = 0; ind < pizzas.size(); mask /= 3, ++ind) {
                        if (mask % 3 == 1) {
                            ones.push_back(ind);
                            for (auto &ing : p[pizzas[ind]]) {
                                curr1.insert(ing);
                            }
                        } else if (mask % 3 == 2) {
                            twos.push_back(ind);
                            for (auto &ing : p[pizzas[ind]]) {
                                curr2.insert(ing);
                            }
                        } else {
                            for (auto &ing : p[pizzas[ind]]) {
                                curr3.insert(ing);
                            }
                        }
                    }

                    long long val = sqr((long long) curr1.size()) + sqr((long long) curr2.size()) +
                                    sqr((long long) curr3.size());
                    if (val > count) {
                        count = val;
                        for (int ind = 0; ind < ones.size(); ++ind) {
                            bestforteam1[ind] = pizzas[ones[ind]];
                        }
                        for (int ind = 0; ind < twos.size(); ++ind) {
                            bestforteam2[ind] = pizzas[twos[ind]];
                        }
                    }

                }

                diff += count;
                //cout<<"diff = "<<diff<<"\n";

                for (int ind = 0; ind < answer[team1].size(); ++ind) {
                    answer[team1][ind] = bestforteam1[ind];
                    allpizzas.erase(answer[team1][ind]);
                }
                for (int ind = 0; ind < answer[team2].size(); ++ind) {
                    answer[team2][ind] = bestforteam2[ind];
                    allpizzas.erase(answer[team2][ind]);
                }

                answer[team3].clear();
                for (auto &p : allpizzas) {
                    answer[team3].push_back(p);
                }
            }
        }
    }

}

void PropagateAnswerWithThreeTeams() {
    if (answer.size() < 3)
        return;
    srand(time(0));
    int team1 = rand() % answer.size();
    int team2;
    do {
        team2 = rand() % answer.size();
    } while (team1 == team2);
    int team3;
    do {
        team3 = rand() % answer.size();
    } while (team1 == team3 || team2 == team3);


    set < int > allpizzas;
    vector < int > pizzas;
    for (auto &piz : answer[team1])
        pizzas.push_back(piz);
    for (auto &piz : answer[team2])
        pizzas.push_back(piz);
    for (auto &piz : answer[team3])
        pizzas.push_back(piz);
    for (auto &p : pizzas) {
        allpizzas.insert(p);
    }
    //cout<<"work\n";
    vector < int > pos;
    for (int mask = 0; mask < pow3(pizzas.size()); mask++) {
        int mmask = mask;
        int cnt1 = 0, cnt2 = 0;
        while (mmask) {
            if (mmask % 3 == 1)
                cnt1++;
            else if (mmask % 3 == 2)
                cnt2++;
            mmask /= 3;
        }
        if (cnt1 == answer[team1].size() && cnt2 == answer[team2].size()) {
            pos.push_back(mask);
        }
    }

    vector < int > bestforteam1;
    vector < int > bestforteam2;
    for (auto &piz : answer[team1])
        bestforteam1.push_back(piz);
    for (auto &piz : answer[team2])
        bestforteam2.push_back(piz);
    long long count = 0;
    {
        set < string > steam1;
        for (auto &piz : answer[team1])
            for (auto &ing : p[piz]) {
                steam1.insert(ing);
            }
        set < string > steam2;
        for (auto &piz : answer[team2])
            for (auto &ing : p[piz]) {
                steam2.insert(ing);
            }
        set < string > steam3;
        for (auto &piz : answer[team3])
            for (auto &ing : p[piz]) {
                steam3.insert(ing);
            }
        count += sqr((long long)steam1.size());
        count += sqr((long long)steam2.size());
        count += sqr((long long)steam3.size());
    }
    long long diff = -count;

    for (auto mask : pos) {
        set < string > curr1, curr2, curr3;
        vector < int > ones;
        vector < int > twos;

        for (int ind = 0; ind < pizzas.size(); mask /= 3, ind++) {
            if (mask % 3 == 1) {
                ones.push_back(ind);
                for (auto &ing : p[pizzas[ind]]) {
                    curr1.insert(ing);
                }
            } else if (mask % 3 == 2) {
                twos.push_back(ind);
                for (auto &ing : p[pizzas[ind]]) {
                    curr2.insert(ing);
                }
            } else {
                for (auto &ing : p[pizzas[ind]]) {
                    curr3.insert(ing);
                }
            }
        }

        long long val = sqr((long long)curr1.size()) + sqr((long long)curr2.size()) + sqr((long long)curr3.size());
        if (val >= count) {
            count = val;
            for (int ind = 0; ind < ones.size(); ind++) {
                bestforteam1[ind] = pizzas[ones[ind]];
            }
            for (int ind = 0; ind < twos.size(); ind++) {
                bestforteam2[ind] = pizzas[twos[ind]];
            }
        }

    }

    diff += count;
    //cout<<"diff = "<<diff<<"\n";

    for (int ind = 0; ind < answer[team1].size(); ind++) {
        answer[team1][ind] = bestforteam1[ind];
        allpizzas.erase(answer[team1][ind]);
    }
    for (int ind = 0; ind < answer[team2].size(); ind++) {
        answer[team2][ind] = bestforteam2[ind];
        allpizzas.erase(answer[team2][ind]);
    }

    answer[team3].clear();
    for (auto &p : allpizzas) {
        answer[team3].push_back(p);
    }

}

void FillNonUsedPizzas() {
    nonused.clear();
    set < int > used;
    for (auto &t : answer) {
        for (auto piz : t) {
            used.insert(piz);
        }
    }
    for (int i = 0; i < p.size(); i++) {
        if (used.count(i) == 0) {
            nonused.push_back(i);
        }
    }
}

void BuildUniquenessOfIngredients() {
    int numberOfIngredients = 0;
    for (auto &e : ingredients) {
        numberOfIngredients += e.second;
    }

    uniquenessOfIngredients.clear();
    for (auto &e : ingredients) {
        uniquenessOfIngredients[e.first] = (double)e.second / numberOfIngredients;
    }
}

void BuildUniquenessOfPizzas() {
    uniquenessOfPizzas.clear();
    for (auto &piz : p) {
        double val = 0;
        for (auto &ing : piz) {
            val += pow(uniquenessOfIngredients[ing], 0.8);
        }
        val /= pow(piz.size(), 2);
        uniquenessOfPizzas.push_back(val);
    }
}

void solve() {

    for (auto &piz : p) {
        for (auto &ing : piz) {
            ingredients[ing]++;
        }
    }

    BuildUniquenessOfIngredients();
    BuildUniquenessOfPizzas();

    vector < int > order(p.size());
    for (int i = 0; i < order.size(); i++) {
        order[i] = i;
    }
    sort(order.begin(), order.end(), [&](int a, int b) {
        return (uniquenessOfPizzas[a] < uniquenessOfPizzas[b]);
    });
    int tt1 = t1, tt2 = t2, tt3 = t3;
    for (int i = 0; i < order.size(); i++) {

        if (tt3 && i + 4 <= order.size()) {
            tt3--;
            vector < int > team;
            for (int j = i; j < i + 4; j++) {
                team.push_back(order[j]);
            }
            answer.push_back(team);
            i += 3;
            continue;
        }

        if (tt2 && i + 3 <= order.size()) {
            tt2--;
            vector < int > team;
            for (int j = i; j < i + 3; j++) {
                team.push_back(order[j]);
            }
            answer.push_back(team);
            i += 2;
            continue;
        }

        if (tt1 && i + 2 <= order.size()) {
            tt1--;
            vector < int > team;
            for (int j = i; j < i + 2; j++) {
                team.push_back(order[j]);
            }
            answer.push_back(team);
            i += 1;
            continue;
        }

        break;
    }

    /*for (int q = 500000; q; q--) {
        if (q % 50000 == 0)
            cout<<CalculateAnswer()<<"\n";
        PropagateAnswer();
    }*/
}

void UpdateAnswer(const string& fin, string fout) {
    Read(fin);
    ifstream FileIn("C:\\Users\\panen\\CLionProjects\\HashCodeTest1\\" + fout);
    int nt;
    FileIn>>nt;
    answer.resize(nt);
    for (int i = 0; i < nt; i++) {
        int k;
        FileIn>>k;
        answer[i].resize(k);
        for (int j = 0; j < k; j++)
            FileIn>>answer[i][j];
    }
    FileIn.clear();
    FileIn.close();
    cout<<"Before improvement: ";
    cout << CalculateAnswer() << "\n";
    if (bool NeedToPropagate = true; NeedToPropagate) {
        FillNonUsedPizzas();
        int tests = 0;
        int freq = 1;
        for (int q = tests; q >= 0; q--) {
            //PropagateAnswerWithRest();
            PropagateAnswerWithAllTwoTeams();
            //cout<<"yes\n";
            //PropagateAnswerWithThreeTeams();
            //PropagateAnswerWithAllThreeTeams();
            //cout<<"yes\n";
            //PropagateAnswer();
            //PropagateAnswerWithTwoTeams();
            if (q % freq == 0) {
                cout << (tests - q) / freq << "/" << tests / freq << ": ";
                cout << CalculateAnswer() << "\n";
            }
        }
        PrintAnswer(fout);
    }
}

int main() {
    /*Read("c.in");
    solve();
    PrintAnswer("c.out");*/
    UpdateAnswer("c.in", "c.out");
    return 0;
}