#ifndef AGGREGATE_H
#define AGGREGATE_H
#include "Iterator.h"

class Aggregate{

public:
   virtual Iterator* createIterator() = 0;

};

#endif