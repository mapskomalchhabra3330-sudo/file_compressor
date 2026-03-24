#pragma once

template<typename T,typename U>class Pair{
    public:
        T key;
        U val;
        Pair(){}
        Pair(T k,U v):key(k),val(v){}
};
