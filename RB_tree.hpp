#pragma once

template <class T>
class RBtree{
public:
    virtual bool find(const T&)=0;
    virtual bool find2(const T&)=0;

};
