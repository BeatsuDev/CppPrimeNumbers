#include <iostream>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <string>
#include <thread>

using namespace std::chrono;
using namespace std;

const int N = 1'000'000'000;
const int BLOCK_SIZE = 256 * 1024 * 8;
const int cutoff = N / 2;
const int segment_count = cutoff / BLOCK_SIZE + 1;

vector<int> find_primes();
void mark_segment(int &s, bitset<BLOCK_SIZE> &segment);

int main() {
    auto start = high_resolution_clock::now();
    vector<int> found_primes = find_primes();
    auto stop = high_resolution_clock::now();

    cout << "Found " << found_primes.size() << " prime numbers up to " << N << " ";
    cout << "in " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return 0;
}

vector<int> find_primes() {
    bitset<BLOCK_SIZE> segments[segment_count];
    for (int i = 0; i < segment_count; i++) {
        bitset<BLOCK_SIZE> segment;
        segment.set();
        segments[i] = segment;
    }
    
    // Create threads for marking each segment
    thread segment_threads[segment_count];
    for (int s = 0; s < segment_count; s++) {
        segment_threads[s] = thread(mark_segment, ref(s), ref(segments[s]));
    }

    // Join the threads
    for (int i = 0; i < segment_count; i++) {
        segment_threads[i].join();
    }

    vector<int> primes;
    int counting_index = 0;

    auto start = high_resolution_clock::now();
    for (bitset segment : segments) {
        for (int i = 0; i < segment.size(); i++) {
            int index = counting_index++;
            if (!segment[i]) continue;
            if (2 * (1 + index) + 1 > N) break;
            primes.push_back(2 * (1 + index) + 1);
        }
    }
    auto stop = high_resolution_clock::now();
    cout << "Conversion time: " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return primes;
}

// Set all the numbers in an interval that will not be used in the last step to false
void mark_segment(int &s, bitset<BLOCK_SIZE> &segment) {
    for (int i = 1; 2*i + 2*i*i < (s+1) * BLOCK_SIZE; i++) {
        // Make j start at the value which makes i+j+2*i*j the value of the start of the segment
        // e.g. if the segment starts at 256, j should be 85, because 1 + 85 + 2 * 1 * 85 = 256
        for (int j = max(i, (s * BLOCK_SIZE - i) / (1 + 2 * i)); i + j + 2*i*j < (s+1)*BLOCK_SIZE; j++) {
            if (i+j+2*i*j < s*BLOCK_SIZE || i+j+2*i*j >= (s+1)*BLOCK_SIZE) continue;
            if (-(s*BLOCK_SIZE) + i+j+2*i*j >= segment.size()) continue;
            segment[-(s*BLOCK_SIZE) + i+j+2*i*j] = false;
        }
    }
}