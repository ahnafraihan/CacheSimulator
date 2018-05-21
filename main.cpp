/* Ahnaf Raihan
 * cache-sim
 * main.cpp
 */

#include "Simulator.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cerr << "Usage Error: Incorrect program arguments." << endl;
        cerr << "Usage: ./cache-sim input_trace.txt output.txt" << endl;
        exit(1);
    }
    
    string inputFile = argv[1];
    string outputFile = argv[2];
    Simulator s(inputFile);
    
    /********************************************* Direct-Mapped Cache *********************************************/
    
    s.directMapped(1024);
    s.directMapped(4096);
    s.directMapped(16384);
    s.directMapped(32768);
    
    /****************************************** Set-Associative Cache ******************************************/
    
    s.setAssociative(2);
    s.setAssociative(4);
    s.setAssociative(8);
    s.setAssociative(16);
    
    /*********************************** Fully-Associative cache with LRU ************************************/
    
    s.fullyAssociative();
    
    /******************************* Fully-Associative cache with Hot-Cold LRU ********************************/
    
    s.fullyAssociativeHC();
    
    /************************* Set-Associative Cache with no Allocation on a Write Miss ***************************/
    
    s.setAssociativeNAWM(2);
    s.setAssociativeNAWM(4);
    s.setAssociativeNAWM(8);
    s.setAssociativeNAWM(16);
    
    /****************************** Set-Associative Cache with Next-line Prefetching ******************************/
    
    s.setAssociativeNLP(2);
    s.setAssociativeNLP(4);
    s.setAssociativeNLP(8);
    s.setAssociativeNLP(16);
    
    /******************************************** Prefetch-on-a-Miss ********************************************/
    
    s.setAssociativePOM(2);
    s.setAssociativePOM(4);
    s.setAssociativePOM(8);
    s.setAssociativePOM(16);
    
    /*********************************************** Print ***********************************************/
   
    s.print(outputFile);
}
