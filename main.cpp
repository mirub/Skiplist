// Copyright [2019] <Banu Miruna Elena 311CA>

#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <random>
#include <utility>
#include <string>
#include "functions.h"

int main() {
    srand(time(NULL));
    std::ifstream f("races.in");
    std::ofstream g("races.out");
    if (!f.is_open() || !g.is_open()) {
        std::cout << "File error!\n";
    }
    int n, m, k;
    f >> n >> m >> k;
    SkipList<int> p(n);
    for (int i = 0; i < n; ++i) {
    	// adds players by id in the main scoreboard
        p.insertPlayerbyPoints(i + 1, 0, 0, 0, 0);
    }
    std::string line;
    char c;
    int *t;
    // t is the array where the time of the current race is kept
    t = new int[n];
    int print = 0;
    int j = 0;
    while (j < m + k) {
        j++;
        f >> c;
        if (c == 'p') {
            for (int i = 0; i < n; ++i) {
                t[i] = 0;
            }
            receive_points(t, p, n, 1, g, print);
            print++;
        } else {
            f.seekg(-1, std::ios::cur);
            for (int i = 0; i < n; ++i) {
                f >> t[i];
            }
            receive_points(t, p, n, 0, g, print);
        }
        getline(f, line);
    }
    delete [] t;
    f.close();
    g.close();
    return 0;
}
