#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "alglibmisc.h"
#include <nlohmann/json.hpp>
#include <chrono>


using json = nlohmann::json;


int main(int argc, char* argv[]) {
    auto program_start = std::chrono::high_resolution_clock::now();

    if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <query.json> <passages.json> <K> <eps>\n";
    return 1;
    }

    auto processing_start = std::chrono::high_resolution_clock::now();
    // Load and parse query JSON
    std::ifstream query_ifs(argv[1]);
    if (!query_ifs) {
        std::cerr << "Error opening query file: " << argv[1] << "\n";
        return 1;
    }
    json query_json;
    query_ifs >> query_json;
    if (!query_json.is_array() || query_json.size() < 1) {
        std::cerr << "Query JSON must be an array with at least 1 element\n";
        return 1;
    }

    // Load and parse passages JSON
    std::ifstream passages_ifs(argv[2]);
    if (!passages_ifs) {
        std::cerr << "Error opening passages file: " << argv[2] << "\n";
        return 1;
    }
    json passages_json;
    passages_ifs >> passages_json;
    if (!passages_json.is_array() || passages_json.size() < 1) {
        std::cerr << "Passages JSON must be an array with at least 1 element\n";
        return 1;
    }


    // Convert JSON array to a dict mapping id -> element
    std::unordered_map<int, json> dict;
    for (auto &elem : passages_json) {
        int id = elem["id"].get<int>();
        dict[id] = elem;
    }


    // Parse K and eps
    int k = std::stoi(argv[3]);
    double eps = std::stof(argv[4]);

    try{
        // Extract the query embedding
        auto query_obj   = query_json[0];
        size_t D         = query_obj["embedding"].size();
        alglib::real_1d_array query;
        query.setlength(D);
        for (size_t d = 0; d < D; ++d) {
            query[d] = query_obj["embedding"][d].get<double>();
        }
        /*
        TODO:
        1. Extract embeddings from each passage and store them in an alglib::real_2d_array (rows = passages, cols = embedding dimensions).
        2. Store passage IDs in an alglib::integer_1d_array as tags to keep track of passage identities.
        3. Build a KD-tree index on the passage embeddings using alglib::kdtreebuildtagged for efficient nearest neighbor search.
        4. Perform a k-nearest neighbor (k-NN) search with the query embedding using alglib::kdtreequeryaknn.
        5. Retrieve the IDs (tags) and distances of the found nearest neighbors using alglib::kdtreequeryresultstags and alglib::kdtreequeryresultsdistances.
        */

    }
    catch(alglib::ap_error &e) {
        std::cerr << "ALGLIB error: " << e.msg << std::endl;
        return 1;
    }

    return 0;
}