#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

using namespace std::chrono;
using namespace std;

vector<int> find_primes_up_to(int n);
void mark_segment(int start, vector<bool>& segment);

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
    vector<thread> threads;
    vector<vector<bool>> segments;

    // Divide into segments of max 256KB worth of data (making it cache friendly)
    int BLOCK_SIZE = 256 * 1000 * 8; // Not sure if it's 1000 or 1024. Keeping it at 1000 just to be on the safe side.
    for (int start_range = 0; start_range < cutoff; start_range += BLOCK_SIZE) {
        vector<bool> segment(min(BLOCK_SIZE, cutoff - start_range), true);
        segments.push_back(segment);
        
        thread t(mark_segment, start_range, &segment);
        threads.push_back(t);
    }

    for (unsigned int i = 0; i < threads.size(); i++) {
        threads.at(i).join();
    }

    vector<int> primes;
    int counting_index = 0;
    
    for (vector<bool> segment : segments) {
        for (int i = 0; i < segment.size(); i++) {
            if (!segment[i]) continue;
            primes.push_back(2 * (1 + counting_index++) + 1);
        }
    }
    return primes;
}

// Set all the numbers in an interval that will not be used in the last step to false
void mark_segment(int start, vector<bool>& segment) {
    int segment_end = start + segment.size();

    // 2i^2 + 2i - s = 0
    // a = 2, b = 2, c = start
    // Use quadratic formula to find necessary starting i value
    int start_i = (-2 + sqrt(4 + 8*start)) / 4;

    for (int i = start_i; 2*i + 2*i*i <= segment_end; i++) {
        for (int j = i; i + j + 2*i*j <= segment_end; j++) {
            segment[-start + i+j+2*i*j] = false;
        }
    }
}