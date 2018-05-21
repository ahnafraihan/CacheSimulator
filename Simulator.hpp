/* Ahnaf Raihan
 * cache-sim
 * Simulator.hpp
 */

#include <iostream>
#include <vector>

using namespace std;

struct instruction {
    unsigned long long address;
    bool value;
};
struct result {
    string name;
    unsigned long long numHits;
};

class Simulator {
private:
    int numInstructions;
    vector<instruction> instructions;
    vector<result> results;

public:
    Simulator(string inputFile);
    void directMapped(int size);
    void setAssociative(int associativity);
    void fullyAssociative();
    void fullyAssociativeHC();
    void setAssociativeNAWM(int associativity);
    void setAssociativeNLP(int associativity);
    void setAssociativePOM(int associativity);
    void print(string file);
};
