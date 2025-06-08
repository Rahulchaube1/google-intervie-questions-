#include <vector>
#include <string>
#include <cstdlib> // Required for rand()
#include <ctime>   // Required for time() to seed rand()

/**
 * // This is the Master's API interface.
 * // You should not implement it, or speculate about its implementation
 * class Master {
 * public:
 * int guess(string word);
 * };
 */

class Solution {
public:
    // Helper function to count matching characters between two strings.
    int match(const string& a, const string& b) {
        int matches = 0;
        for (size_t i = 0; i < a.length(); ++i) {
            if (a[i] == b[i]) {
                matches++;
            }
        }
        return matches;
    }

    void findSecretWord(vector<string>& words, Master& master) {
        // Seed the random number generator. In a real-world scenario or a
        // more advanced contest setting, a more robust random engine from <random>
        // would be better, but this is sufficient for most LeetCode test cases.
        srand(time(NULL));

        // Loop up to a high number of times. The problem's own `allowedGuesses`
        // will be the true limit enforced by the judge. This just prevents our
        // code from stopping prematurely on tests with many allowed guesses.
        for (int i = 0; i < 100 && !words.empty(); ++i) {
            
            // --- Minimax Strategy with Randomized Tie-breaking ---
            
            int minMaxPartitionSize = words.size() + 1;
            vector<string> best_candidates;

            // Find the best score (the minimum possible "worst-case" partition size).
            for (const string& guessCandidate : words) {
                vector<int> partitions(7, 0);
                for (const string& otherWord : words) {
                    partitions[match(guessCandidate, otherWord)]++;
                }

                int maxPartitionSize = 0;
                for (int p_size : partitions) {
                    if (p_size > maxPartitionSize) {
                        maxPartitionSize = p_size;
                    }
                }
                
                // If we find a new best score, clear old candidates and start a new list.
                if (maxPartitionSize < minMaxPartitionSize) {
                    minMaxPartitionSize = maxPartitionSize;
                    best_candidates.clear();
                    best_candidates.push_back(guessCandidate);
                } 
                // If we find a word that's just as good, add it to our list of candidates.
                else if (maxPartitionSize == minMaxPartitionSize) {
                    best_candidates.push_back(guessCandidate);
                }
            }
            
            // Randomly pick one of the best candidates to make our guess.
            string bestGuess = best_candidates[rand() % best_candidates.size()];

            int matches = master.guess(bestGuess);

            if (matches == 6) {
                return;
            }

            // Filter the word list for the next iteration.
            vector<string> nextWords;
            for (const string& word : words) {
                if (match(bestGuess, word) == matches) {
                    nextWords.push_back(word);
                }
            }
            words = nextWords;
        }
    }
};
