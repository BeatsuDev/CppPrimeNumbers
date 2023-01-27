#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std::chrono;

using namespace::std;

vector<int> find_primes_up_to(int n);
bool is_prime(int n);

int main() {
    int N = 1'000'000;

    auto start = high_resolution_clock::now();
    vector<int> found_primes = find_primes_up_to(N);
    auto stop = high_resolution_clock::now();

    cout << "Found " << found_primes.size() << " prime numbers up to " << N << " ";
    cout << "in " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return 0;
}

vector<int> find_primes_up_to(int n) {

    // Create a boolean sieve vector containing only every odd number from 3 and up to n
    vector<bool> boolean_sieve = vector<bool>(n/2 + (n % 2 == 1 ? 1 : 0), true);

    for (int i = 0; i < boolean_sieve.size(); i++) {
        if (!boolean_sieve[i]) continue;

        // Example: index 0 is 3, index 1 is 5
        // number = (index+1)*2 + 1
        // The "real" value of the index we're at:
        int number = (i+1)*2 + 1;
        if (is_prime(number)) continue;
        boolean_sieve[i] = false;

        // Turn all future factors of n into false
        // Watch out for integer overflows
        if (number >= 65536) continue;
        for (int x = number * number; number <= n; number += number) {
            // We need to calculate the index from the value n.
            // Remember that index 0 is 3, index 1 is 5 etc...
            int calculated_index = number / 2 - 1;
            boolean_sieve[calculated_index] = false;
        }
    }

    // Convert the indicies of the true values into ints
    vector<int> primes = vector<int>();
    primes.push_back(2);

    for (int i = 0; i < boolean_sieve.size(); i++) {
        if (!boolean_sieve[i]) continue;
        primes.push_back((i+1)*2 + 1);
    }

    // return the primes!
    return primes;
}

// TODO: Only check if the number is divisble by any previous
// primes found in the prime sieve.
bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 3; i < sqrt(n); i += 2) {
        if (n % i == 0) return false;

        // Watch out for integer overflows. Next run will i*i will cause an overflow
        // if (i+1 >= 65536) break;
    }
    return true;
}