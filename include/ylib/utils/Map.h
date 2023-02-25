//
// Created by Yaison Alcantara 3 on 2/24/23.
//

#include <map>
#include <ylib/core/lang.h>

namespace ylib::utils {
    using namespace std;

    template<typename K, typename V>
    class Map {
    private:
        map<K, V> _map;
    public:
        void put(K key, V val){
            _map.insert_or_assign(key, val);
        }

        optional<V> find(const K& key){
            auto pos = _map.find(key);
            if(pos == _map.end()){
                return nullopt;
            }

            return pos->second;
        }

        V get(const K& key){
            auto pos = _map.find(key);
            if(pos == _map.end()){
                throw Exception("Key does not exists.");
            }

            return pos->second;
        }

        size_t size(){
            return _map.size();
        }
    };
}
