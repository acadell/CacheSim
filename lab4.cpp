/*
Author: Anderson Cadell
Description: A cache simulator that can simulate different associativity for caches given an input file.
Date: April 18, 2018
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <iomanip>

std::ifstream inFile;


//check the number of arguments.
//if there are enough arguments, return 1, otherwise return 0
int checkArguments(int numArgs);

int main(int argc, char *argv[])
{
    int currentAddress;     //address that was next in the file
    int hitCount = 0;
    int accesses = 0;
    int passes = 0;
    int x, indexBits = 0;
    
    if(!checkArguments(argc))
    {
        return 1;
    }

    //Open the input file
    inFile.open(argv[1]);
    
    //check if file can be read
    //if not, exit the program
    if(!inFile)
    {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }
 
    //various variable declarations to set up the cache,
    //valid bits, age for LRU, and set up the number of sets and ways

    int wayNum = std::atoi(argv[2]);
    int setNum = 16/wayNum;
    int** cache = NULL;
    int** valid = NULL;
    int** age = NULL;
    
    cache = new int*[setNum];
    valid = new int*[setNum];
    age = new int*[setNum];

    //initialize cache variables
    for(int i = 0; i < setNum; i++)
    {
        cache[i] = new int[wayNum];
        valid[i] = new int[wayNum];
        age[i] = new int[wayNum];
    }


    //print name and cache
    std::cout << "Anderson Cadell" << std::endl;
    for( int set = 0; set < setNum; set++)
    {
        std::cout << "Set " << set+1 << " ";
        for( int way = 0; way < wayNum; way++)
        {
            valid[set][way] = 0;
            if(valid[set][way])
            {
                cache[set][way] =0;
                age[set][way] =0;
                std::cout << std::setw(4) << std::setfill('0') << cache[set][way] << "    ";
            }
            else
            {
                std::cout << "INVL" << "    ";
            }
            
        }
        std::cout << std::endl;
    } 
    
    
    //start indexes in the cache
    
    //find out the index bits
    indexBits = log2(setNum);
    int index;
    //loop to read in every value in the dataset
    while(inFile >> x)
    {
        index = x >> indexBits;
        
        int hit = 0;
        int set = (x & ((1<<indexBits)-1));
        int replaceWay = 0;
        replaceWay = age[set][0];
        passes++;
        
        //int index = 
        
        //check every block in the set to see what one to replace
        for(int i = 0; i < wayNum; i++)
        {
            //check if the item is in the cache, if so, break out, otherwise, find the least recently used block
            if(!hit)
            {
                if( valid[set][i]  && cache[set][i] == index )
                {
                    hit = 1;
                    replaceWay = i;
                    break;
                }
                else if( !valid[set][i] )
                {
                   replaceWay = i;
                   break;
                }
                else if(age[set][i] < age[set][replaceWay])
                {
                    replaceWay = i;
                }
            }
            
        }
        
        //if there was a hit, update the hit count, set the block to valid and give it a recent age.
        //if there is not a hit, then set the youngest block to valid, give it a recent age, and add it to the cache
        if(hit)
        {
            hitCount++;
            valid[set][replaceWay] = 1;
            age[set][replaceWay] = passes;
            //cache[set][replaceWay] = x;
            //break;
        }
        else
        {
            valid[set][replaceWay] = 1;
            age[set][replaceWay] = passes;
            cache[set][replaceWay] = index;
        }
        
    }


    //print name and cache
    std::cout << "Anderson Cadell" << std::endl;
    for( int set = 0; set < setNum; set++)
    {
        std::cout << "Set " << set+1 << " ";
        for( int way = 0; way < wayNum; way++)
        {
            if(valid[set][way])
            {
                std::cout << std::setw(4) << std::setfill('0') << cache[set][way] << "    ";
            }
            else
            {
                std::cout << "INVL" << "    ";
            }
            
        }
        std::cout << std::endl;
    } 
    std::cout << "Hits " << hitCount << std::endl;
    std::cout << "Total Accesses " << passes << std::endl;
    
    
}



//check the number of arguments.
//if there are enough arguments, return 1, otherwise return 0
int checkArguments(int numArgs)
{
    int enoughArgs = 1;    

    //if there aren't enough arguments, return 0
    if(numArgs < 3 )
    {
        std::cout << "Invalid number of arguments. acadellLab4 fileName.txt associativity" << std::endl;
        enoughArgs = 0;
        
    }
    return enoughArgs;
}
