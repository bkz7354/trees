#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <set>

#include "AVL_tree.hpp"
#include "RB_tree.hpp"

namespace chr = std::chrono;

struct timer{
    using clock = chr::high_resolution_clock;
    chr::time_point<clock> mem;

    void start(){
        mem = clock::now();
    }
    double get(){
        chr::duration<double, std::milli> d = clock::now() - mem;
        return d.count();
    }
};

std::mt19937 gen(std::random_device{}());
std::vector<int> random_vec(const int sz){
    std::vector<int> res(sz);
    std::generate(res.begin(), res.end(), gen);
    return res;
}

template<class Tree>
void test_insert_and_find(const int n){
    Tree t;
    auto samp = random_vec(n);

    for(auto x: samp)
        t.insert(x);
    for(auto x: samp)
        if(!t.count(x))
            throw std::runtime_error{"failed test_insert_and_find"};

    auto samp2 = random_vec(n);
    std::unordered_set<int> s(samp.begin(), samp.end());
    for(auto x: samp2)
        if(t.count(x) && !s.count(x))
            throw std::runtime_error{"failed test_insert_and_find"};
}

template<class Tree>
void test_delete(const int n, const int k){
    Tree t;
    auto samp = random_vec(n);

    for(auto x: samp)
        t.insert(x);
    std::unordered_set<int> s(samp.begin(), samp.end());

    std::shuffle(samp.begin(), samp.end(), gen);
    for(int i = 0, j = 0; i < n; i += k){
        for(; j < i; ++j){
            s.erase(samp[j]);
            t.erase(samp[j]);
        }

        for(auto x: samp)
            if(s.count(x) xor t.count(x))
                throw std::runtime_error{"failed test_delete"};
    }
}



int main(){
    try{
        test_insert_and_find<std::set<int>>(1000);
        test_delete<std::set<int>>(1000,100);

        return 0;
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
}