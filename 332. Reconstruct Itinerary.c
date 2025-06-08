#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set> // multiset for sorted destinations
#include <algorithm> // for reverse

class Solution {
public:
    // Adjacency list: from airport -> multiset of destination airports.
    // Using a multiset keeps destinations sorted lexicographically and handles duplicate tickets.
    std::unordered_map<std::string, std::multiset<std::string>> adj;
    
    // The final reconstructed itinerary.
    std::vector<std::string> itinerary;

    /**
     * @brief Finds the lexicographically smallest itinerary that uses all tickets.
     * @param tickets A list of [from, to] flight tickets.
     * @return A vector of strings representing the itinerary.
     */
    std::vector<std::string> findItinerary(std::vector<std::vector<std::string>>& tickets) {
        // Step 1: Build the graph from the given tickets.
        for (const auto& ticket : tickets) {
            adj[ticket[0]].insert(ticket[1]);
        }

        // Step 2: Start the DFS traversal from the required starting point, "JFK".
        dfs("JFK");

        // Step 3: The itinerary is constructed backwards during the post-order traversal.
        // Reverse it to get the correct chronological order.
        std::reverse(itinerary.begin(), itinerary.end());

        return itinerary;
    }

    /**
     * @brief Performs a post-order DFS to find the Eulerian path.
     * @param airport The current airport to visit from.
     */
    void dfs(const std::string& airport) {
        // While there are still outgoing flights from the current airport...
        while (!adj[airport].empty()) {
            // Get the lexicographically smallest destination.
            std::string next_airport = *adj[airport].begin();
            
            // "Use" the ticket by removing it from the graph.
            adj[airport].erase(adj[airport].begin());
            
            // Recursively travel to the next airport.
            dfs(next_airport);
        }
        
        // After all destinations from the current airport have been visited
        // (i.e., we've hit a dead end or completed a sub-tour), add the
        // current airport to our path.
        itinerary.push_back(airport);
    }
};
