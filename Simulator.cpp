/* Ahnaf Raihan
 * cache-sim
 * Simulator.cpp
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <deque>
#include "Simulator.hpp"

using namespace std;

Simulator::Simulator(string file) {
    string address, value;
    numInstructions = 0;
    ifstream inf(file);
    
    if(inf.fail()) {
        cerr << "Error: Cannot find input file: " << file << endl;
        exit(1);
    }
    
    while (inf >> value >> address) {
        numInstructions++;
        instruction instr;
        address = address.substr(2);
        stringstream stream;
        stream << address;
        stream >> hex >> instr.address;
        
        if (value == "L")
            instr.value = true;
        else if (value == "S")
            instr.value = false;
        else {
            cerr << "Error: Undefined instruction value: " << value << endl;
            exit(1);
        }
        instructions.push_back(instr);
    }
    inf.close();
}

void Simulator::directMapped(int size) {
    result r;
    r.name = "Direct-Mapped Cache: " + to_string(size);
    r.numHits = 0;
    int line = 0, cacheSize = size/32, tag = 0;
    int cache[cacheSize];
    
    fill_n(cache, cacheSize, 0);
    
    for (int i = 0; i < instructions.size(); i++) {
        line = (instructions[i].address >> 5) % cacheSize;
        tag = (instructions[i].address >> (unsigned long long)(log2(cacheSize) + 5));
        if (cache[line] == tag)
            r.numHits++;
        else
            cache[line] = tag;
    }
    results.push_back(r);
}

void Simulator::setAssociative(int associativity) {
    result r;
    r.name = "Set-Associative Cache: " + to_string(associativity);
    r.numHits = 0;
    int numSets = 512 / associativity;
    vector <deque<int>> cache;
    
    for (int i = 0; i < numSets; i++) {
        deque<int> d;
        for (int j = 0; j < associativity; j++)
            d.push_back(0);
        cache.push_back(d);
    }
    
    for (int i = 0; i < instructions.size(); i++) {
        bool flag = false;
        int set = (instructions[i].address >> 5) % (numSets);
        int tag = instructions[i].address >> ((unsigned long long)(log2(numSets) + 5));
        
        for (int j = 0; j < associativity; j++) {
            if (cache[set][j] == tag) {
                cache[set].erase(cache[set].begin() + j);
                cache[set].push_front(tag);
                flag = true;
            }
        }
        
        if (flag)
            r.numHits++;
        else {
            cache[set].pop_back();
            cache[set].push_front(tag);
        }
    }
    results.push_back(r);
}

void Simulator::fullyAssociative() {
    result r;
    r.name = "Fully-Associative Cache with LRU";
    r.numHits = 0;
    int numSets = 1, numLines = 512;
    vector <deque<int>> cache;
    
    for (int i = 0; i < numSets; i++) {
        deque<int> d;
        for (int j = 0; j < numLines; j++)
            d.push_back(0);
        cache.push_back(d);
    }
    
    for (int i = 0; i < instructions.size(); i++) {
        bool flag = false;
        int set = (instructions[i].address >> 5) % (numSets);
        int tag = instructions[i].address >> ((unsigned long long)(log2(numSets) + 5));
        
        for (int j = 0; j < numLines; j++) {
            if (cache[set][j] == tag) {
                cache[set].erase(cache[set].begin() + j);
                cache[set].push_front(tag);
                flag = true;
            }
        }
        
        if (flag)
            r.numHits++;
        else {
            cache[set].pop_back();
            cache[set].push_front(tag);
        }
    }
    results.push_back(r);
}

void Simulator::fullyAssociativeHC() {
    result r;
    r.name = "Fully-Associative cache with Hot-Cold LRU";
    r.numHits = 0;
    int numLines = 512;
    
    int cache[numLines];
    int hc[numLines];
    
    fill_n(cache, numLines, 0);
    fill_n(hc, numLines, 0);
    
    for (int i = 0; i < instructions.size(); i++) {
        bool flag = false;
        int tag = instructions[i].address >> (unsigned long long)(log2(32));
        for (int j = 0; j < numLines; j++) {
            if (cache[j] == tag) {
                r.numHits++;
                int x = 0;
                for (int k = 0; k < log2(numLines); k++) {
                    int temp = (j/(numLines/(2 << k))) % 2;
                    if (temp == 1) {
                        hc[x] = 0;
                        x = 2 * (x + 1);
                    }
                    else if (temp == 0) {
                        hc[x] = 1;
                        x = (2 * x) + 1;
                    }
                }
                flag = true;
                break;
            }
        }
        if (!flag) {
            int y = 0;
            for (int i = 0; i < log2(numLines); i++) {
                if (hc[y] == 0) {
                    hc[y] = 1;
                    y = (2 * y) + 1;
                }
                else {
                    hc[y] = 0;
                    y = 2 * (y + 1);
                }
            }
            y = y + 1 - numLines;
            cache[y] = tag;
        }
    }
    results.push_back(r);
}

void Simulator::setAssociativeNAWM(int associativity) {
    result r;
    r.name = "Set-Associative Cache with no Allocation on a Write Miss: " + to_string(associativity);
    r.numHits = 0;
    int numSets = 512 / associativity;
    vector <deque<int>> cache;
    
    for (int i = 0; i < numSets; i++) {
        deque<int> d;
        for (int j = 0; j < associativity; j++)
            d.push_back(0);
        cache.push_back(d);
    }
    
    for (int i = 0; i < instructions.size(); i++) {
        bool flag = false;
        int set = (instructions[i].address >> 5) % (numSets);
        int tag = instructions[i].address >> ((unsigned long long)(log2(numSets) + 5));
        
        for (int j = 0; j < associativity; j++) {
            if (cache[set][j] == tag) {
                cache[set].erase(cache[set].begin() + j);
                cache[set].push_front(tag);
                flag = true;
            }
        }
        
        if (flag)
            r.numHits++;
        else {
            if (instructions[i].value) {
                cache[set].pop_back();
                cache[set].push_front(tag);
            }
        }
    }
    results.push_back(r);
}

void Simulator::setAssociativeNLP(int associativity) {
    result r;
    r.name = "Set-Associative Cache with Next-line Prefetching: " + to_string(associativity);
    r.numHits = 0;
    int numSets = 512 / associativity;
    vector <deque<int>> cache;
    
    for (int i = 0; i < numSets; i++) {
        deque<int> d;
        for (int j = 0; j < associativity; j++)
            d.push_back(0);
        cache.push_back(d);
    }
    
    for (int i = 0; i < instructions.size(); i++) {
        bool flag = false;
        int set = (instructions[i].address >> 5) % (numSets);
        int tag = instructions[i].address >> ((unsigned long long)(log2(numSets) + 5));
        unsigned long long NLPAddress = instructions[i].address + 32, NLPTag = NLPAddress >> ((unsigned long long)(log2(numSets) + 5));
        int NLPSet = (NLPAddress >> 5) % (numSets);
        
        for (int j = 0; j < associativity; j++) {
            if (cache[set][j] == tag) {
                cache[set].erase(cache[set].begin() + j);
                cache[set].push_front(tag);
                flag = true;
            }
        }
        
        if (flag)
            r.numHits++;
        else {
            cache[set].pop_back();
            cache[set].push_front(tag);
        }
        
        flag = false;
        
        for (int j = 0; j < associativity; j++) {
            if (cache[NLPSet][j] == NLPTag) {
                cache[NLPSet].erase(cache[NLPSet].begin() + j);
                cache[NLPSet].push_front(NLPTag);
                flag = true;
            }
        }
        
        if (!flag) {
            cache[NLPSet].pop_back();
            cache[NLPSet].push_front(NLPTag);
        }
    }
    results.push_back(r);
}

void Simulator::setAssociativePOM(int associativity) {
    result r;
    r.name = "Prefetch-on-a-Miss: " + to_string(associativity);
    r.numHits = 0;
    int numSets = 512 / associativity;
    vector <deque<int>> cache;
    
    for (int i = 0; i < numSets; i++) {
        deque<int> d;
        for (int j = 0; j < associativity; j++)
            d.push_back(0);
        cache.push_back(d);
    }
    
    for (int i = 0; i < instructions.size(); i++) {
        bool flag = false;
        int set = (instructions[i].address >> 5) % (numSets);
        int tag = instructions[i].address >> ((unsigned long long)(log2(numSets) + 5));
        unsigned long long NLPAddress = instructions[i].address + 32, NLPTag = NLPAddress >> ((unsigned long long)(log2(numSets) + 5));
        int NLPSet = (NLPAddress >> 5) % (numSets);
        
        for (int j = 0; j < associativity; j++) {
            if (cache[set][j] == tag) {
                cache[set].erase(cache[set].begin() + j);
                cache[set].push_front(tag);
                flag = true;
            }
        }
        
        if (flag)
            r.numHits++;
        else {
            cache[set].pop_back();
            cache[set].push_front(tag);
            
            flag = false;
            
            for (int j = 0; j < associativity; j++) {
                if (cache[NLPSet][j] == NLPTag) {
                    cache[NLPSet].erase(cache[NLPSet].begin() + j);
                    cache[NLPSet].push_front(NLPTag);
                    flag = true;
                }
            }
            
            if (!flag) {
                cache[NLPSet].pop_back();
                cache[NLPSet].push_front(NLPTag);
            }
        }
    }
    results.push_back(r);
}

void Simulator::print(string file) {
    ofstream ofs(file);
    
    if(ofs.fail()) {
        cerr << "Error: Invalid output file: " << file << endl;
        exit(1);
    }
    
    for (int i = 0; i < 4; i++) {
        if (i == 3)
            ofs << results.at(i).numHits << "," << numInstructions << ";";
        else
            ofs << results.at(i).numHits << "," << numInstructions << "; ";
    }
    ofs << endl;
    for (int i = 4; i < 8; i++) {
        if (i == 7)
            ofs << results.at(i).numHits << "," << numInstructions << ";";
        else
            ofs << results.at(i).numHits << "," << numInstructions << "; ";
    }
    ofs << endl;
    ofs << results.at(8).numHits << "," << numInstructions << ";";
    ofs << endl;
    ofs << results.at(9).numHits << "," << numInstructions << ";";
    ofs << endl;
    for (int i = 10; i < 14; i++) {
        if (i == 13)
            ofs << results.at(i).numHits << "," << numInstructions << ";";
        else
            ofs << results.at(i).numHits << "," << numInstructions << "; ";
    }
    ofs << endl;
    for (int i = 14; i < 18; i++) {
        if (i == 17)
            ofs << results.at(i).numHits << "," << numInstructions << ";";
        else
            ofs << results.at(i).numHits << "," << numInstructions << "; ";
    }
    ofs << endl;
    for (int i = 18; i < 22; i++) {
        if (i == 21)
            ofs << results.at(i).numHits << "," << numInstructions << ";";
        else
            ofs << results.at(i).numHits << "," << numInstructions << "; ";
    }
    ofs << endl;
    ofs.close();
}
