//
//  Generator.h
//  fantasybit
//
//  Created by Jay Berg on 3/18/14.
//
//  class Generator to generate "CoinBase" transactions based on a source

#ifndef fantasybit_Generator_h
#define fantasybit_Generator_h


#include <vector>
#include <map> 
#include <string>
#include "fbutils.h"

namespace fantasybit
{

class Transaction {};
using Source = FantasyName;

//generates transactions for new fantasybits
class Generator
{
public:
    std::vector<Transaction> generate(const Source&,const MapS2I&) const;
};
    


} //fantasybit

#endif