#include"list.h"
#include"pair.h"

template<typename T,typename U> class Map{
    List<Pair<T,U> >* buckets;
    int K;
    int getIndex(T key){
        return key%K;
    }
    static bool match(Pair<T,U> a,T b){
        return a.key == b;
    }
public:
    Map(): K(10){
        buckets = new List<Pair<T,U> >[K];
    }
    void set(T key, U val){
        int x = this->getIndex(key);
        Pair<T,U> newPair(key,val);
        this->buckets[x].insert(newPair);
    }
    U get(T key){
        int x = this->getIndex(key);
        return this->buckets[x].find(key,match).val;

    }

};

