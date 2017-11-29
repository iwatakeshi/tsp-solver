#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#define INT_INF numeric_limits<int>::max()

typedef vector<vector<int> > dist_vec;

int n; // Number of cities
size_t nset; // Number of subsets (2^n)

// Prints binary representation of a subset
void printb(size_t s)
{
	for (int i = n - 1; i >= 0; i--)
		printf("%zu", (s >> i) & 1);
}


// Read distances from stdin
void read_dist(dist_vec &dist)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> dist[i][j];
		}
	}
}

// Generates the minimum subdistance for each ending city in each possible
// subset
void build_subsets(dist_vec &subdist, const dist_vec &dist)
{
	// Start with shortest possible tour (starting and ending at the first
	// city)
	subdist[1][0] = 0;

	// Iterate all subsets that include the first city, with size greater
	// than 2 (i.e. every odd bitmask, starting from 3 (dec) or 11 (bin))
	for (size_t s = 3; s < nset; s += 2) { 

		// Iterate ending cities (excluding first city)
		for (int j = 1; j < n; j++) {

			if (!(s & (1 << j)))
				continue;

			// Exclude ending city j from subset: S - {j}
			size_t t = s & ~(1 << j);

			// Find minimum sub-distance for this subtour with j as ending city
			for (int i = 0; i < n; i++) {
				if (s & (1 << i) && i != j && subdist[t][i] < INT_INF){
					auto d = subdist[t][i] + dist[i][j];
					subdist[s][j] = min(subdist[s][j], d);	
				}
			}
		}
	}

	for (int i = 0; i < subdist.size(); i++) {
      for (int j = 0; j < subdist[i].size(); j++) {
        printf("%20d ", subdist[i][j]);
      }
      printf("\n");
    }
    printf("\n");
}

// Searches for shortest Hamiltonian cycle
list<int> min_cycle(dist_vec &subdist, const dist_vec &dist)
{
	list<int> cycle(1, 0);
	vector<bool> visited(n, false);

	// Backtracking start with subset that contains all cities
	size_t s = nset - 1;

	// Mark first city as visited
	visited[0] = true;

	for (int i = 0; i < n - 1; i++) {

		int best_j;
		int min_dist = INT_INF;

		// Find next non-visited city with best sub-distance from
		// previous city in the cycle
		for (int j = 0; j < n; j++) {
			if (!visited[j] && subdist[s][j] + dist[cycle.back()][j] < min_dist) {
				min_dist = subdist[s][j] + dist[cycle.back()][j];
				best_j = j;
			}
		}

		// Mark city as visited and exclude it from the subset
		cycle.push_back(best_j);
		visited[best_j] = true;
		s &= ~(1 << best_j);
	}

	cycle.push_back(0);
	return cycle;
}


int main(int argc, char *argv[])
{

	srand(time(NULL));
	n = 4;
	nset = 1 << n;

	list<int> cycle;
	int cycle_dist = 0.0;

	// Distance matrix
	dist_vec dist(n, vector<int>(n, 0));

	// Subset subdistance
	dist_vec subdist(nset, vector<int>(n, INT_INF));

	for(int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			dist[i][j] = rand() % 10 + 1;
			dist[j][i] = dist[i][j];
		}
	}

	for(int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%2d ", dist[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	build_subsets(subdist, dist);
	cycle = min_cycle(subdist, dist);

	// Print shortest cycle + distance
	list<int>::const_iterator it;

	for (it = cycle.begin(); it != cycle.end(); ++it) {
		printf("%d ", *it);

		if (it != cycle.begin())
			cycle_dist += dist[*it][*prev(it)];
	}
	printf("\n%d\n", cycle_dist);


	return 0;
}
