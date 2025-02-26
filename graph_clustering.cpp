#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct WeightedEdge {
    string sourceNode;
    string targetNode;
    double weight;
};

struct Cluster {
    unordered_set<string> nodes;
    double density;
};

unordered_map<string, vector<WeightedEdge>> graph;

void addEdge(const string& source, const string& target, double weight) {
    graph[source].push_back({source, target, weight});
    graph[target].push_back({target, source, weight});
}

void readGraphFromFile(const string& filename) {
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            istringstream iss(line);
            string source, target, discard;
            iss >> source >> target >> discard;

            addEdge(source, target, 1.0); // Assigning default weight for now
        }
        file.close();
    } else {
        cerr << "Error: Unable to open file" << endl;
        exit(1);
    }
}

double calculateDensity(const Cluster& cluster) {
    double numNodes = cluster.nodes.size();
    double numEdges = 0.0;

    for (const auto& node1 : cluster.nodes) {
        for (const auto& edge : graph[node1]) {
            if (cluster.nodes.find(edge.targetNode) != cluster.nodes.end()) {
                numEdges++;
            }
        }
    }

    return (numNodes > 1) ? (numEdges / (numNodes * (numNodes - 1))) : 0.0;
}

void expandCluster(const string& startNode, Cluster& currentCluster, unordered_map<string, bool>& visited, double densityThreshold, double cpThreshold) {
    set<string> neighbors;
    do {
        for (const auto& clusterNode : currentCluster.nodes) {
            for (const auto& edge : graph[clusterNode]) {
                if (!visited[edge.targetNode]) {
                    neighbors.insert(edge.targetNode);
                }
            }
        }

        vector<string> sortedNeighbors(neighbors.begin(), neighbors.end());

        for (const auto& neighbor : sortedNeighbors) {
            visited[neighbor] = true;
            currentCluster.nodes.insert(neighbor);
            if (calculateDensity(currentCluster) >= densityThreshold) {
                neighbors.erase(neighbor);
            } else {
                currentCluster.nodes.erase(neighbor);
            }
        }

        neighbors.clear();
    } while (!neighbors.empty());
}

vector<Cluster> performClustering(double densityThreshold, double cpThreshold) {
    unordered_map<string, bool> visited;
    vector<Cluster> clusters;

    for (const auto& pair : graph) {
        string node = pair.first;

        if (!visited[node]) {
            visited[node] = true;
            Cluster currentCluster;
            currentCluster.nodes.insert(node);

            expandCluster(node, currentCluster, visited, densityThreshold, cpThreshold);

            if (calculateDensity(currentCluster) >= densityThreshold) {
                clusters.push_back(currentCluster);
            }
        }
    }

    return clusters;
}

void displayClusterStatistics(const vector<Cluster>& clusters) {
    for (size_t i = 0; i < clusters.size(); ++i) {
        const auto& cluster = clusters[i];
        int edgeCount = 0;

        for (const auto& node : cluster.nodes) {
            for (const auto& edge : graph[node]) {
                if (cluster.nodes.find(edge.targetNode) != cluster.nodes.end()) {
                    edgeCount++;
                }
            }
        }

        double density = calculateDensity(cluster);

        cout << "Cluster " << (i + 1) << ":\n";
        cout << "Number of nodes: " << cluster.nodes.size() << "\n";
        cout << "Number of edges: " << edgeCount / 2 << "\n"; // Divide by 2 because edges are counted twice
        cout << "Density: " << density << "\n\n";
    }
}

void displayClusters(const vector<Cluster>& clusters) {
    for (size_t i = 0; i < clusters.size(); ++i) {
        cout << "Cluster " << (i + 1) << ": ";
        for (const auto& node : clusters[i].nodes) {
            cout << node << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: ./graph_clustering <filename> <densityThreshold> <cpThreshold> <mode>" << endl;
        return 1;
    }

    string filename = argv[1];
    double densityThreshold = stod(argv[2]);
    double cpThreshold = stod(argv[3]);
    string mode = argv[4]; // "stats" or "clusters"

    readGraphFromFile(filename);
    vector<Cluster> clusters = performClustering(densityThreshold, cpThreshold);

    if (mode == "stats") {
        displayClusterStatistics(clusters);
    } else if (mode == "clusters") {
        displayClusters(clusters);
    } else {
        cerr << "Invalid mode. Use 'stats' or 'clusters'." << endl;
    }

    return 0;
}
