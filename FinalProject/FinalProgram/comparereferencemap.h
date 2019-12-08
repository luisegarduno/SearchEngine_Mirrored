#ifndef COMPAREREFERENCEMAP_H
#define COMPAREREFERENCEMAP_H

#include <set>
#include <string>
#include <algorithm>

using std::pair;
using std::string;
using std::multiset;
using std::reference_wrapper; // Class template that wraps a reference in a copyable

/* Implementation base on:
 * http://www.cplusplus.com/forum/beginner/168761/
 */

struct compareReferencedMap{
    template <typename T>
    bool operator() (const T& left, const T& right) const{
        return left.second.get() > right.second.get();
    }
};

template <typename T>
multiset< pair<reference_wrapper<const string>, reference_wrapper<const int> >, compareReferencedMap > sortMap(const T& map){
    multiset< pair<reference_wrapper<const string>, reference_wrapper<const int> >, compareReferencedMap > newMultiSet;

    for(auto& pair : map){
        newMultiSet.emplace(pair.first, pair.second);
    }

    return newMultiSet;
}

#endif // COMPAREREFERENCEMAP_H
