#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>

using namespace std::chrono;
using namespace std;

vector<int> find_primes_up_to(int n);

int main() {
    int N = 100'000'000;

    auto start = high_resolution_clock::now();
    vector<int> found_primes = find_primes_up_to(N);
    auto stop = high_resolution_clock::now();

    cout << "Found " << found_primes.size() << " prime numbers up to " << N << " ";
    cout << "in " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return 0;
}

vector<int> find_primes_up_to(int n) {
    int cutoff = n / 2;
    vector<vector<bool>> segments;

    // Divide into segments of max 256KB worth of data (making it cache friendly)
    int BLOCK_SIZE = 256 * 1000; // Not sure if it's 1000 or 1024. Keeping it at 1000 just to be on the safe side.
    for () {

    }

    // Create a thread for each segment (does threading preserve locality of reference?)


    segments.push_back(segment);
    vector<bool> marks(n + 1, true);
    for (int i = 1; 2*i + 2*i*i <= cutoff; i++) {
        for (int j = i; i + j + 2*i*j <= cutoff; j++) {
            marks[i+j+2*i*j] = false;
        }
    }

    vector<int> primes;
    for (int i = 0; i <= marks.size(); i++) {
        if (2 * i + 1 > marks.size()) break;
        if (marks[i])
            primes.push_back(2 * i + 1);
    }
    return primes;
}