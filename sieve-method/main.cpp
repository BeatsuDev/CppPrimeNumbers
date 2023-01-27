#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std::chrono;
using namespace::std;

vector<long> find_primes_up_to(long cutoff);

int main() {
    long N = 10'000'000;

    auto start = high_resolution_clock::now();
    vector<long> found_primes = find_primes_up_to(N);
    auto stop = high_resolution_clock::now();

    cout << "Found " << found_primes.size() << " prime numbers up to " << N << " ";
    cout << "in " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return 0;
}

vector<long> find_primes_up_to(long cutoff) {
    // Create a boolean sieve vector containing only every odd n from 3 and up to cutoff
    vector<bool> boolean_sieve = vector<bool>(cutoff + 1, true);
    boolean_sieve[0] = false;
    boolean_sieve[1] = false;

    for (long i = 2; i <= cutoff; i++) {
        if (!boolean_sieve[i]) continue;

        for (long future_index = i*i; future_index <= cutoff; future_index += i) {
            boolean_sieve[future_index] = false;
        }
    }

    // Convert from indices to numbers
    vector<long> primes = vector<long>();

    for (long i = 0; i <= cutoff; i++) {
        if (boolean_sieve[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}