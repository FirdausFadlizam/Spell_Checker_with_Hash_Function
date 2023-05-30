#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <string>
using namespace std;

int nextPrime( int n );
int hash1( const string & key );
int hash1( int key );

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hash( string str ) --> Global method to hash strings
// int getTableSize() --> Return array size
// int getTotalObj() --> Return total objects
// int getTotalCollision --> Return total collision
// int getLongestCollision --> Return longest collision
// double getAverageChain --> Return average collision
// double getLoadFactor --> Return load factor

template <typename HashedObj>
class HashTable
{
  public:
    explicit HashTable( int size = 101 ) : array( nextPrime( size ) )
      { makeEmpty( ); }

    bool contains( const HashedObj & x ) const
    {
        return isActive( findPos( x ) );
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( int i = 0; i < array.size( ); i++ )
            array[ i ].info = EMPTY;

        totalObj = 0;
    }

    bool insert( const HashedObj & x )
    {
            // Insert x as active
        totalCollision = 1;
        int currentPos = findInsertPos( x );
        ++totalObj;
        if( isActive( currentPos ) )
            return false;

        array[ currentPos ] = HashEntry( x, ACTIVE );

            // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

       
    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        --totalObj;
        array[ currentPos ].info = DELETED;
        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

    int getTotalObj() {
        return totalObj;
    }

    int getTotalCollision() {

        int x = 0;
        
        for (int i = 0; i < collision.size(); i++) {
            if (collision[i] >= 2) {
                ++x;
            }
        }

        return x;
    }

    int getTableSize() {
        return array.size();
    }
    int getLongestCollision() {

        int longest = collision[0];
        for (int i = 1; i < collision.size(); i++) {
            if (longest < collision[i]) {
                longest = collision[i];
            }
        }
        return longest;
    }

    double getLoadFactor() {

        double x = totalObj;
        return x / array.size();

    }

    double getAverageChain() {

        double overallCollision = 1;

        for (int i = 0; i < collision.size(); i++) {
            overallCollision = overallCollision + collision[i];
        }

        return overallCollision / collision.size();

    }


  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
          : element( e ), info( i ) { }
    };

    int totalObj;
    int totalCollision = 1;
    int longestCollision = 0;
    int insertCollision = 0;
    double loadFactor;
    vector<int>collision;


    
    vector<HashEntry> array;
    int currentSize;

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x ) const
    {
        int offset = 1;
        int currentPos = myhash( x );

          // Assuming table is half-empty, and table length is prime,
          // this loop terminates
        while( array[ currentPos ].info != EMPTY &&
                array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        return currentPos;
    }

    int findInsertPos(const HashedObj & x){

       
        int offset = 1;
        int currentPos = myhash(x);

        // Assuming table is half-empty, and table length is prime,
        // this loop terminates
        while (array[currentPos].info != EMPTY &&
            array[currentPos].element != x)
        {
            if (totalCollision == 1) {
               ++insertCollision;
            }
            
            ++totalCollision;
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if (currentPos >= array.size())
                currentPos -= array.size();
        }

        if (longestCollision < totalCollision) {
            longestCollision = totalCollision;
        }

        collision.push_back(totalCollision);
        return currentPos;
    }


    void rehash( )
    {
        vector<HashEntry> oldArray = array;

        if (longestCollision < totalCollision)
            longestCollision = totalCollision;

        totalCollision = 0;
        insertCollision = 0;
            // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( int j = 0; j < array.size( ); j++ )
            array[ j ].info = EMPTY;

            // Copy table over
        currentSize = 0;
        for( int i = 0; i < oldArray.size( ); i++ )
            if( oldArray[ i ].info == ACTIVE )
                insert( oldArray[ i ].element );
    }
    int myhash( const HashedObj & x ) const
    {
        int hashVal = hash1( x );


        hashVal %= array.size( );
        if( hashVal < 0 )
            hashVal += array.size( );

        return hashVal;
    }
};

#endif
