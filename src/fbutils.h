//
//  fbutils.h
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#ifndef fantasybit_fbutils_h
#define fantasybit_fbutils_h

#include <cstdint>
#include <map>
#include <vector>
#include <memory>
#include <utility>

namespace fantasybit {

//forwards
struct FantasyName;

using Int = int_fast32_t;
using MapS2I = std::map<std::string,int_fast32_t>;
template<class T>
    using NameValuePairs = std::vector<std::pair<std::shared_ptr<FantasyName>, T>>;

}

#endif