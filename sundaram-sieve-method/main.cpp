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