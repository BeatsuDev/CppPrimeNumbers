#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <string>
#include <thread>

using namespace std::chrono;
using namespace std;

vector<int> find_primes_up_to(int n);
void mark_segment(int segment_start, vector<bool> &segment);


int N = 10'000'000;
int BLOCK_SIZE = 256 * 1000 * 8;


int main() {
    auto start = high_resolution_clock::now();
    vector<int> found_primes = find_primes_up_to(N);
    auto stop = high_resolution_clock::now();

    cout << "Found " << found_primes.size() << " prime numbers up to " << N << " ";
    cout << "in " << duration_cast<milliseconds>(stop - start).count() << "ms." << endl;
    return 0;
}

vector<int> find_primes_up_to(int n) {
    int cutoff = n / 2;
    int segment_count = cutoff / BLOCK_SIZE + 1;


    vector<bool> segments[segment_count];
    for (int i = 0; i < segment_count; i++) {
        vector<bool> segment(min(BLOCK_SIZE, cutoff - i * BLOCK_SIZE), true);
        segments[i] = segment;
    }
    
    // Create threads for marking each segment
    thread segment_threads[segment_count];
    for (int s = 0; s < segment_count; s++) {
        // mark_segment(s, segments[s]);
        segment_threads[s] = thread(mark_segment, s, ref(segments[s]));
    }

    // Join the threads
    for (int i = 0; i < segment_count; i++) {
        segment_threads[i].join();
    }

    vector<int> primes;
    int counting_index = 0;
    
    for (vector<bool> segment : segments) {
        for (int i = 0; i < segment.size(); i++) {
            int index = counting_index++;
            if (!segment[i]) continue;
            primes.push_back(2 * (1 + index) + 1);
        }
    }
    return primes;
}

// Set all the numbers in an interval that will not be used in the last step to false
void mark_segment(int s, vector<bool> &segment) {
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