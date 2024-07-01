# Separate Chaining Hash Table in C++
This project implements a separate chaining hash table in C++ as part of an Algorithms and Data Structures class. Separate chaining is a technique used in hash tables to handle collisions by maintaining a list of all elements that hash to the same value.

Features
Efficient Collision Handling: Uses linked lists to handle collisions, ensuring efficient storage and retrieval.
Custom Hash Function: Implements a custom hash function to distribute keys evenly.
Basic Operations: Supports insertion, deletion, and search operations.
Iterator Support: Provides iterator support for easy traversal of elements.
File Structure
ADS_set.h: Header file containing the implementation of the separate chaining hash table.
Usage
Clone the Repository
sh
Code kopieren
git clone https://github.com/LaraMij/ADS-Separate-Chaining.git
Include the Header File
To use the separate chaining hash table, include the ADS_set.h file in your C++ project:

cpp
Code kopieren
#include "ADS_set.h"
# Example
Here is a simple example demonstrating how to use the hash table:

cpp
Code kopieren
#include <iostream>
#include "ADS_set.h"

int main() {
    ADS_set<int> mySet;

    mySet.insert(10);
    mySet.insert(20);
    mySet.insert(30);

    if (mySet.contains(20)) {
        std::cout << "20 is in the set." << std::endl;
    }

    for (const auto& elem : mySet) {
        std::cout << elem << " ";
    }

    return 0;
}

# Class Interface
# Constructor
ADS_set(): Default constructor initializes the hash table.
ADS_set(std::initializer_list<key_type> ilist): Constructor to initialize the hash table with a list of keys.
template<typename InputIt> ADS_set(InputIt first, InputIt last): Constructor to initialize the hash table with a range of keys.
ADS_set(const ADS_set& other): Copy constructor.
# Destructor
~ADS_set(): Destructor to clean up allocated memory.
# Member Functions
void insert(const key_type& key): Inserts a key into the hash table.
bool contains(const key_type& key) const: Checks if the hash table contains a key.
void clear(): Clears the hash table.
size_type size() const: Returns the number of elements in the hash table.
bool empty() const: Checks if the hash table is empty.
void erase(const key_type& key): Removes a key from the hash table.
iterator begin() const: Returns an iterator to the beginning of the hash table.
iterator end() const: Returns an iterator to the end of the hash table.
