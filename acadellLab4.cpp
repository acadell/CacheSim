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
    
    cache = new int*[setNum];//[wayNum];
    valid = new int*[setNum];//[wayNum];
    age = new int*[setNum];//[wayNum];

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
    
    
    //start putting stuff in the cache
    

    indexBits = log2(setNum);

    //loop to read in every value
    while(inFile >> x)
    {
        int hit = 0;
        int set = (x & ((1<<indexBits)-1));
        int replaceWay = 0;
        replaceWay = age[set][0];
        passes++;
        
        //check the index to see if there is a hit and update hit counter and age/
        //if there is no hit, prefer invalid blocks to be replaced
        //otherwise, replace the oldest bits
        for(int i = 0; i < wayNum; i++)
        {
            accesses++;
            
            //if there is a hit, update the hit count and the age and break the loop.
            if(valid[set][i] && cache[set][i] == x)
            {
                hit = 1;
                hitCount++;
                age[set][i] = passes;
                break;
            }
            else
            {
                //if there is an invalid block
                //set the block to replace if there is no
                //hit to that invalid block.
                //otherwise set the oldest block as the one to replace
                if(!valid[set][i])
                {
                    replaceWay = i;
                    break;
                }
                else
                {
                    //if the current element is older than
                    //the last oldest element, set the block to replace
                    //to the current index
                    if(age[set][i] < age[set][replaceWay])
                    {
                        replaceWay = i;
                    }
                }
            }
        }
        //if there wasn't a hit, put the value in the cache
        //and set the age and set the block to valid.
        if(!hit)
        {
            accesses++;
            valid[set][replaceWay] = 1;
            cache[set][replaceWay] = x;
            age[set][replaceWay] = passes;
        }

    }



    
    
    //print name and cache
    std::cout << std::endl << "Anderson Cadell" << std::endl;
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
    std::cout << "Total Accesses " << accesses << std::endl;



    for(int i = 0; i < setNum; i++)
    {
        delete [] cache[i];
        delete [] valid[i];
        delete [] age[i];
    }
    
    delete [] cache;
    delete [] valid;
    delete [] age;



    return 0;
    
}

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
