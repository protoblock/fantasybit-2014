//
//  FantasyName.cpp
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#include <fc/io/raw.hpp>
#include "FantasyName.h"
#include <fc/crypto/city.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/exception/exception.hpp>
#include <algorithm>
#include <locale>


namespace fantasybit
{



/** valid chars:  A-Z 0-9 _ - . */
bool is_invalid_char( char c )
{
  return c == 0;
}

/** @note assumes c has already been converted to upper case asci  */
char replace_similar( char c )
{
  switch( c )
  {
     case 'H': return 'H';
     case '4': 
     case 'A': return 'A';


     case '6':
     case 'G': return 'G';

     case 'K': return 'K';

     case '9': // 9999PPP999999P99999P 
     case 'P': return 'P';

     case '2':
     case 'Z': 
     case '5':  
     case 'S': return 'S';

     case '7':
     case 'T': return 'T';

     case 'X': return 'X';
     case 'Y': return 'Y'; 

     case '3': 
     case 'E': return 'E';
     case 'F': return 'F';

     case 'L':
     case 'I':
     case '1': return 'I';
     case 'J': return 'J';

     case 'R': // when next to m or n is confusing ie: rn rm 
     case 'M':
     case 'N': return 'N';

     case 'C': return 'C';
     case '0': 
     case '8': // D 0 8 O B are all easily confused
     case 'B': 
     case 'D':
     case 'O':
     case 'Q': return 'O';

     case 'U':
     case 'W':
     case 'V': return 'U';

     case '.': 
     case '_': 
     case '-': return '.';

     default:
       return 0;
  }

}

void replace_char_runs( std::string& s )
{
  for( size_t p = 0; p < s.size() - 1; )
  {
    if( s[p] == s[p+1] )
    {
      s.erase(p,1);
    }
    else
    {
      ++p;
    }
  }
}

static inline void convertToASCII(const std::string& input, std::string* buffer)
  {
  assert(buffer != nullptr);
  buffer->reserve(input.size());

  for(const auto& c : input)
    {
    unsigned int cCode = c;
    if(cCode > 0x7F)
      {
      /// Non ASCII character
      char numBuffer[64];
      sprintf(numBuffer, "_0x%X_", cCode);
      buffer->append(numBuffer);
      }
    else
      {
      *buffer += toupper(c);
      }
    }
  }


/**
 * @param n - the name in UTF-8 format
 */
uint64_t  FantasyName::name_hash( const std::string& n )
{
  if( n.size() == 0 ) return 0;

  std::string asciiName;
  convertToASCII(n, &asciiName);

  for( auto itr = asciiName.begin(); itr != asciiName.end(); ++itr )
    *itr = replace_similar(*itr);
 
  asciiName.erase(std::remove_if(asciiName.begin(), asciiName.end(), is_invalid_char), asciiName.end());

  if(asciiName.empty())
    return 0;

  if( asciiName.size() && asciiName.front() == '.' )
    asciiName.erase(0,1);

  if( asciiName.size() && asciiName.back() == '.' )
    //asciiName.pop_back();
    asciiName.erase (asciiName.size() - 1);

  FC_ASSERT( asciiName.size() > 0 );

  // secure hash function
  fc::sha256 h = fc::sha256::hash( asciiName.c_str(), asciiName.size() );

  // compress it down to 64 bits
  return fc::city_hash64( (char*)&h, sizeof(h) );
}

nameid_t name_transaction::id() const
{
    nameid_t::encoder enc;
    fc::raw::pack(enc,name_hash);
    fc::raw::pack(enc,pubkey);
    fc::raw::pack(enc,nonce);
    fc::raw::pack(enc,utc_sec);
    fc::raw::pack(enc,prev);
    return enc.result();
}

nameid_t name_transaction::sigid() const
{
    nameid_t::encoder enc;
    fc::raw::pack(enc,name_hash);
    fc::raw::pack(enc,pubkey);
    fc::raw::pack(enc,nonce);
    fc::raw::pack(enc,utc_sec);
    fc::raw::pack(enc,prev);
    fc::raw::pack(enc,sig);
    return enc.result();
}

fc::sha256 name_transaction::digest() const
{
    return fc::sha256::hash(id());
}

fc::sha256 name_transaction::sigdigest() const
{
    return fc::sha256::hash(sigid());
}


}