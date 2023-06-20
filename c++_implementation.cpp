#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
// read input graph G and k-uniform fractional matching w
int numVertices, numEdges;
cin >> numVertices >> numEdges;

vector<vector<pair<int, double>>> G(numVertices); // adjacency list representation of the graph
map<pair<int, int>, double> w; // edge weights

double we=0.0;
for (int i = 0; i < numEdges; i++) {
    int u, v;
    double weight;
    cin >> u >> v >> weight;

    G[u].push_back({v, weight});
    G[v].push_back({u, weight});
    w[{u, v}] = weight;
    w[{v, u}] = weight;
    we +=weight;
}
cout<<"size of fractional matching in original graph 'G', size(w)= "<<we<<endl;
    // create list L for each vertex
    
    vector<vector<int> > L(G.size());
    for (int v = 0; v < G.size(); v++) {
        for (auto [u, _] : G[v]) {
            L[v].push_back(u);
        }
        sort(L[v].begin(), L[v].end(), [&](int u, int v) {
            return G[u].size() > G[v].size();
        });
    }

    // initialize output graph G' and its weight w'
    vector<vector<pair<int, double>>> Gprime(G.size());
    double wprime = 0.0;

    // iterate over each vertex v and its list L-v
    for (int v = 0; v < G.size(); v++) {
        int k = ceil(G[v].size() / 2.0);

        // skip already relaxed vertices
        if (Gprime[v].size() >= k) {
            continue;
        }

        vector<int> S;
        for (auto u : L[v]) {
            if (u < v && any_of(Gprime[v].begin(), Gprime[v].end(), [&](auto edge) {
                return edge.first == u;
            })) {
                // edge {u, v} already in G', reduce k
                //k--;
                if (Gprime[v].size() >= k) {
                    break;
                }
                continue;
            }

            if (u >= v && S.size() <= k) {
                S.push_back(u);
                if (S.size() == k) {
                    break;
                }
            }
        }

        // add edges {v, u} to G' and double the weight in G
        for (auto u : S) 
		{
            //Gprime[v].push_back({u, 2.0 / G[v].size()});
            //Gprime[u].push_back({v, 2.0 / G[u].size()});
            Gprime[v].push_back({u, 2.0 * w[{v, u}]});
            Gprime[u].push_back({v, 2.0 * w[{u,v}]});
            wprime += 2.0 * w[{v, u}];
        }
    }

    //print output graph G' and its weight w'
    cout << "sparsifified graph G' = {";
    for (int v = 0; v < Gprime.size(); v++) 
	{
        cout << v << ":{";
        for (auto [u, w] : Gprime[v]) 
		{
            cout << u << "," << w << ",";
        }
        cout << "},";
    }
    cout << "}" << endl;
    cout<<"size of fractional matching in sparsified graph G', size(w')= "<<wprime<<endl;
        return 0;
}

