#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std::chrono;

using namespace::std;

vector<int> primes(int n);
bool is_prime(int n);

int main() {
    int N = 1'000'000;

    auto start = high_resolution_clock::now();
    vector<int> found_primes = primes(N);
    auto stop = high_resolution_clock::now();

    cout << "Found " << found_primes.size() << " prime numbers up to " << N << " ";
    cout << "in " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return 0;
}

vector<int> primes(int n) {
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (!is_prime(i)) continue;
        primes.push_back(i);
    }
    return primes;
}

bool is_prime(int n) {
    if (n <= 1) { return false; }
    for (int i = 2; i < sqrt(n)+1; i++) {
        if (n % i == 0) { return false; }
    }
    return true;
}