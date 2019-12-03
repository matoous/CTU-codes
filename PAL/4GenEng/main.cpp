//
// Created by Matous Dzivjak on 22/11/2019.
//
#include <iostream>
#include <map>
#include <vector>
#include <climits>

using namespace std;

int main() {
    int N, max_deletes;
    string target_string;

    cin >> target_string >> N >> max_deletes;

    vector<string> atoms(N);
    vector<int> costs(N);
    int d_table[100][100]; // table for levenshtein distance calculation

    for (int i = 0; i < N; ++i) cin >> costs[i] >> atoms[i]; // read all building block

    vector<pair<int, int> > cost(target_string.size() + 1, pair<int, int>(INT_MAX, INT_MAX));
    cost[0] = make_pair(0, 0);

    for (int i = 0; i < target_string.size(); i++) { // go through the whole target string
        const int curr_cost = cost[i].first, curr_distance = cost[i].second;
        if (curr_cost == INT_MAX) continue; // skip unreachable state

        for (int j = 0; j < atoms.size(); ++j) { // go through each atom
            const string atom = atoms[j];
            const int atom_size = atom.size();

            // clear the distances d_table
            const string ts = target_string.substr(i, atom_size);
            for (int l = 0; l <= atom_size; l++) {
                // it is enough to clear till l+1 since operations to the right from diagonal are inserts
                for (int q = 1; q <= l + 1; q++) d_table[l][q] = INT_MAX;
                d_table[l][0] = l;
            }

            // get levenshtein distances with deletes only
            for (int l = 1; l <= atom_size; l++)
                for (int q = 1; q <= l; q++)
                    d_table[l][q] = min(
                            d_table[l - 1][q] != INT_MAX ? d_table[l - 1][q] + 1 : INT_MAX, // delete
                            atom[l - 1] == ts[q - 1] ? d_table[l - 1][q - 1] : INT_MAX //move further
                    );

            for (int k = 1; k <= ts.size(); k++) { // for each available number of deletions in the building block
                const int num_of_deletes = d_table[atom_size][k];
                if (num_of_deletes > max_deletes) continue;
                const int offset = i + atom_size - num_of_deletes; // the the total offset in target string
                if (offset > target_string.size()) continue;
                const int new_cost = curr_cost + num_of_deletes + costs[j];
                const int new_distance = curr_distance + 1;
                if (
                        new_cost < cost[offset].first || // better cost
                        (new_cost == cost[offset].first &&
                         new_distance < cost[offset].second))  // same cost better distance
                {
                    cost[offset] = make_pair(new_cost, new_distance);
                }
            }
        }
    }
    cout << cost[target_string.size()].first << " " << cost[target_string.size()].second << endl;
}
