//
// Created by musta on 20.12.2022.
//

#include "Graph.h"
#include "Flight.h"

Graph::Graph() {
    string temp;

    unordered_map<string, string> city2country;


    ifstream in("airlines.csv");
    getline(in, temp);

    while (getline(in, temp)) {
        Airline a(temp);
        string code = temp.substr(0, 3);
        airlines[code] = a;
        t.insert(a.getCode());
    }


    in.close(); in.open("flights.csv");
    getline(in, temp);
    while (getline(in, temp)) {
        n_of_flights++;
        Flight f = Flight(temp);
        g[f.getSource()].push_back(f.getTarget());
        airlines[f.getTarget().getAirline()].addAirport(f.getTarget().getAirport());
    }


    in.close(); in.open("airports.csv");
    getline(in, temp);

    while (getline(in, temp)) {
        Airport a(temp);
        string code = temp.substr(0, 3);
        if (city2country.find(a.getCity()) != city2country.end()) {
            if (city2country[a.getCity()] != a.getCountry()) {
                strangeCities.insert(a.getCity());
            }
        }
        else {
            city2country[a.getCity()] = a.getCountry();
        }
    }


    in.close(); in.open("airports.csv");
    getline(in, temp);

    while (getline(in, temp)) {
        Airport a(temp);
        string code = temp.substr(0, 3);

        if (strangeCities.find(a.getCity()) != strangeCities.end()) {
            airports[code] = a;
            dataForStrangeCities[a.getCountry()][a.getCity()].insert(code);
        }
        else {
            airports[code] = a;
            cities[a.getCity()].insert(code);
            countries[a.getCountry()].insert(a.getCity());
        }
    }

}


bool Graph::isCity(const string& name) {
    auto p = cities.find(name);
    auto p2 = strangeCities.find(name);
    if (p != cities.end() || p2 != strangeCities.end()) {
        return true;
    }
    return false;
}

bool Graph::isCountry(const string& name) {
    auto p = countries.find(name);
    if (p == countries.end()) {
        return false;
    }
    return true;
}

int Graph::get_global_n_flight() { return n_of_flights; }

unordered_map<string, Airline> Graph::getAirlines() { return airlines; }

unordered_map<string, vector<Target>> Graph::getG() { return g; }

unordered_map<string, Airport> Graph::getAirports() { return airports; }

unordered_map<string, unordered_set<string>> Graph::getCities() { return cities; }

unordered_map<string, unordered_set<string>> Graph::getCountries() { return countries; }

ss Graph::getUltimatePath(string from, string to, set <string> Comp, vector < vector<pss> >* others) {
    Comp = Comp.empty() ? t : Comp;
    vector <string> fromV;
    vector <string> toV;
    bool isCountryF = isCountry(from);
    bool isCountryT = isCountry(to);
    bool isCityF = isCity(from);
    bool isCityT = isCity(to);

    if (isCountryF) {
        for (const auto& city : countries[from]) {

            if (isStarageCiti(city)) {
                for (const auto& airport : dataForStrangeCities[from][city]) {
                    fromV.push_back(airport);
                }
            }
            else {
                for (const auto& airport : cities[city]) {
                    fromV.push_back(airport);
                }
            }
        }
    }

    if (isCountryT) {
        for (const auto& city : countries[to]) {
            if (isStarageCiti(city)) {
                for (const auto& airport : dataForStrangeCities[from][city]) {
                    toV.push_back(airport);
                }
            }
            else {
                for (const auto& airport : cities[city]) {
                    toV.push_back(airport);
                }
            }

        }
    }

    if (isCityF) {
        if (isStarageCiti(from)) {
            string countryF1;
            string countryF;
            cout << "The Arrive city: " << from << " is ambigous in country\nPlease enter the Country:";
            cin >> countryF1;
            getline(cin, countryF);
            countryF1 += countryF;
            for (auto i : dataForStrangeCities[countryF1][from]) fromV.push_back(i);
        }
        else {
            for (const auto& i : cities[from]) fromV.push_back(i);
        }
    }

    if (isCityT) {
        if (isStarageCiti(to)) {
            string countryT1;
            string countryT;
            cout << "The Destination city: " << to << " is ambigous in country\nPlease enter the Country:";
            cin >> countryT1;
            getline(cin, countryT);
            countryT1 += countryT;
            for (auto i : dataForStrangeCities[countryT1][to]) toV.push_back(i);
        }
        else {
            for (const auto& i : cities[to]) toV.push_back(i);
        }
    }

    if (!isCityF && !isCountryF) {
        fromV.push_back(from);
    }
    if (!isCityT && !isCountryT) {
        toV.push_back(to);
    }

    ss ans = getPathByVectors(fromV, toV, Comp, others);

    return ans;

}

vector <vector <pss> > fill(const string& from, const string& to, unordered_map <string, vector <string> >& p, unordered_map <string, vector <string> >& pAir, int d) {

    vector <vector <string> > ans = { {to} }, ansAir = { {"No airport"} };

    for (int i = 0; i < d; i++) {
        vector <vector <string> > temp, temp2;
        int pos = 0;
        for (auto elem : ans) {
            int j = 0;
            for (auto path : p[elem.back()]) {
                vector <string> tt = elem, tt2 = ansAir[pos];
                tt.push_back(path);
                tt2.push_back(pAir[elem.back()][j]);
                temp.push_back(tt);
                temp2.push_back(tt2);
                j++;
            }
            pos++;
        }
        ans = temp;
        ansAir = temp2;
    }

    vector < vector <pss> > result(ans.size());

    for (int i = 0; i < ans.size(); i++) {
        result[i].resize(ans[i].size());
        for (int j = 0; j < ans[i].size(); j++)
            result[i][j] = { ans[i][j], ansAir[i][j] };
        reverse(result[i].begin(), result[i].end());
    }
    return result;
}

ss Graph::getPathAirports(const string& from, const string& to, set <string> Comp, vector < vector<pss> >* others) {

    Comp = Comp.empty() ? t : Comp;

    unordered_map <string, bool> used;
    unordered_map <string, vector <string > > p, pAirlane;
    unordered_map <string, int> d;
    queue <string> q;
    q.push(from);
    d[q.front()] = 0;
    used[q.front()] = true;

    while (!q.empty()) {

        string cur = q.front();
        q.pop();

        for (auto i = 0; i < g[cur].size(); i++) {
            string target = g[cur][i].getAirport(), air = g[cur][i].getAirline();
            if ((!used[target] || d[cur] + 1 == d[target]) && Comp.find(air) != Comp.end()) {
                if (!used[target])
                    q.push(target);
                d[target] = d[cur] + 1;
                p[target].push_back(cur);
                pAirlane[target].push_back(air);
                used[target] = true;
            }
        }
    }

    if (!used[to])
        return { {"No path exist"}, {} };

    string pass = to;
    vector <string> ans, ansAir;
    while (pass != from) {
        ans.push_back(pass);
        ans.back() = airports[ans.back()].getName();
        ansAir.push_back(pAirlane[pass][0]);
        pass = p[pass][0];
    }

    ans.push_back(airports[from].getName());

    reverse(ans.begin(), ans.end());
    reverse(ansAir.begin(), ansAir.end());
    ansAir.push_back("No airline");

    if (others != nullptr)
        *others = fill(from, to, p, pAirlane, d[to]);
    return { ans, ansAir };
}

vector<string> Graph::targetAirports(const string& from, int num, set <string> Comp) {

    Comp = Comp.empty() ? t : Comp;

    unordered_set <string> t;
    unordered_map <string, bool> used;
    queue <pair <string, int> > q;
    q.push({ from, 0 });

    while (!q.empty()) {

        pair <string, int> cur = q.front();
        q.pop();

        for (auto i = 0; i < g[cur.first].size(); i++) {
            string target = g[cur.first][i].getAirport(), air = g[cur.first][i].getAirline();
            if (!used[target] && cur.second < num && Comp.find(air) != Comp.end()) {
                t.insert(target);
                q.push({ target, cur.second + 1 });
                used[target] = true;
            }
        }
    }

    vector <string> ans;
    for (const auto& i : t)
        ans.push_back(i);
    return ans;
}

ss Graph::getPathByVectors(vector <string> from, vector <string> to, set <string> Comp, vector < vector<pss> >* others) {

    int best = -1;
    ss ans;
    ss temp;
    vector < vector<pss> > tempOthers;

    for (int i = 0; i < from.size(); i++) {
        for (int j = 0; j < to.size(); j++) {

            string fromA = from[i];
            string toA = to[j];

            temp = getPathAirports(fromA, toA, Comp, &tempOthers);

            if (temp.first.size() != 1 && (best == -1 || best > temp.first.size())) {
                best = temp.first.size();
                *others = tempOthers;
                ans = temp;
            }
            if (best == temp.first.size()) {
                for (const auto& item : tempOthers) {
                    others->push_back(item);
                }
                ans = temp;
            }
        }
    }

    return ans;
}

vector<string> Graph::getPathByPoint(string from, double lat, double lon, double dist) /* FALTA ACABAR AQUIIIIIIIIIIIIIIIIIIIIIIII */ {

    vector <string> toV;
    for (const auto& e : airports) {
        if (e.second.getDistanceTo(lat, lon) < dist) {
            toV.push_back(e.first);
        }
    }
    return {};
}

unsigned int Graph::getNumberOfFlights(const std::string& Airport) {
    return g[Airport].size();
}

unordered_set<string> Graph::getAirlinesFromAirport(const std::string& Airport) {
    unordered_set<string> res;
    vector<Target> targets = g[Airport];
    for (auto t : targets) {
        res.insert(t.getAirline());
    }
    return res;
}

list<string> Graph::getArticulationPoints(set <string> Comp) {
    Comp = Comp.empty() ? t : Comp;

    unordered_map <string, bool> used;
    unordered_map <string, int> num, low;
    int index = 1;
    list<string> res;
    for (const auto& a : g) {
        if (!used[a.first]) {
            dfsArticulationP(a.first, num, low, index, used, res, Comp);
        }
    }
    return res;
}

void Graph::dfsArticulationP(const string& airport, unordered_map <string, int>& num, unordered_map <string, int>& low, int& index, unordered_map <string, bool>& used, list<string>& res, const set <string>& Comp) {

    bool a = false;
    int children = 0;
    num[airport] = low[airport] = index++; used[airport] = true;

    for (auto e : g[airport]) {
        string w = e.getAirport(), air = e.getAirline();
        if (!used[w] && Comp.find(air) != Comp.end()) {
            children++;
            dfsArticulationP(w, num, low, index, used, res, Comp);
            low[airport] = min(low[airport], low[w]);
            if (low[w] >= num[airport] && num[airport] > 1) {
                a = true;
            }
        }
        else {
            low[airport] = min(low[airport], num[w]);
        }
    }
    if ((num[airport] == 1 && children > 1) || (num[airport] > 1 && a)) {
        res.push_back(airport);
    }
}

int Graph::diameterBFS(string airport, set<string> Comp) {
    int max = -1;
    unordered_map <string, bool> used;
    unordered_map <string, int> d;
    queue <string> q;
    q.push(airport);
    d[airport] = 0;

    while (!q.empty()) {

        string cur = q.front(); q.pop();

        for (auto i = 0; i < g[cur].size(); i++) {
            string target = g[cur][i].getAirport(), air = g[cur][i].getAirline();
            if (!used[target] && Comp.find(air) != Comp.end()) {
                q.push(target);
                d[target] = d[cur] + 1;
                if (d[target] > max) {
                    max = d[target];
                }
                used[target] = true;
            }
        }
    }
    return max; // temos que verificar se for -1???????????????????
}

// for(auto i : airports){ ou nao?
int Graph::getDiameter(set<string> Comp) {

    Comp = Comp.empty() ? t : Comp;

    int max = -1;
    for (auto i : airports) {
        int temp = diameterBFS(i.first, Comp);
        if (temp > max) {
            max = temp;
        }
    }
    return max;
}

unordered_set<string> Graph::getStranfeSities() {
    return strangeCities;
}

bool Graph::isStarageCiti(string name) {
    return not (strangeCities.find(name) == strangeCities.end());
}

vector <pss> Merge(vector <pss> p1, vector <pss> p2) {
    vector <pss> ans = p1;
    for (int i = 0; i < p2.size(); i++)
        ans.push_back(p2[i]);
    return ans;
}

vector <vector <pss> > Graph::Combine(vector <vector <pss> > p1, vector <vector <pss> > p2) {
    vector < vector <pss> > ans;
    if (p2.empty()) return p1;
    for (int i = 0; i < p1.size(); i++)
        for (int j = 0; j < p2.size(); j++)
            ans.push_back(Merge(p1[i], p2[j]));
    return ans;
}

void Graph::Update(string v, um <string, int>& d, string& curComp, int dist, string start, queue <string>& q, um <string, vector <string> >& p, um <string, vector <string> >& pAir, string par, string parAir) {

    used[v] = curComp;

    if (start != v) {
        if (d.find(v) == d.end())
            q.push(v);
        parent[v].push_back(start);
        air[v].push_back(curComp);
        d[v] = dist;
        p[v].push_back(par);
        pAir[v].push_back(parAir);
    }

    for (auto i : g[v]) {
        string step = i.getAirport();
        if (curComp == i.getAirline() && (d.find(step) == d.end() || d[step] == dist) && used[step] != curComp) {
            Update(step, d, curComp, dist, start, q, p, pAir, v, i.getAirline());
        }
    }

}

void Graph::getPath(string v, string to, vector <vector <pss> > cur, vector <vector <pss> >& ans) {
    vector <vector <pss> > tempAns;

    if (v == to) {
        for (auto i : cur)
            ans.push_back(i);
        return;
    }

    for (int i = 0; i < parent[v].size(); i++) {
        set <string> tempComp; tempComp.insert(air[v][i]);
        vector <vector <pss> > tempOthers;

        string zxc = parent[v][i];

        getPathAirports(parent[v][i], v, tempComp, &tempOthers);

        if (cur.size())
            for (int j = 0; j < tempOthers.size(); j++)
                tempOthers[j].pop_back();

        tempAns = Combine(tempOthers, cur);

        getPath(parent[v][i], to, tempAns, ans);

    }

}


vector <vector <pss> > Graph::getPathByAirportsAirlines(string from, string to, set <string> Comp) {

    if (Comp.empty())
        Comp = t;

    vector <vector <pss> > ans;
    um <string, vector <string> > p, pAir;
    um <string, int> d;

    d[from] = 0;
    queue <string> q;
    q.push(from);

    while (q.size()) {

        string cur = q.front();
        q.pop();

        if (d.find(to) != d.end() && d[cur] + 1 > d[to]) break;

        for (auto i : Comp)
            Update(cur, d, i, d[cur] + 1, cur, q, p, pAir, "", "");

    }

    getPath(to, from, {}, ans);

    parent.clear();
    used.clear();
    air.clear();
    d.clear();


    return ans;

}
