#pragma once
#include "pointer.hpp"
#include "expr.hpp"

class Val;
class Env {
public:
    static PTR(Env) empty;
    
    virtual PTR(Val) lookup(std::string find_name) = 0;
};



class EmptyEnv : public Env {
public:
    EmptyEnv();
    
private:
    /**
     * Method throws a runntime error telling the lookup string is a free variable
     *
     */
    PTR(Val) lookup(std::string find_name){
        throw std::runtime_error("free variable: "
                                 + find_name);
    }
};


class ExtendedEnv : public Env {
public:

    ExtendedEnv(PTR(Env) rest, std::string name, PTR(Val) val);
    
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    
private:
    
    /**
     * Method to look through the linked list of Env to find the matching string and return the value of that string
     *
     * @param
     *  the string to use to look throuh the environments
     */
    PTR(Val) lookup(std::string find_name) {
        if (find_name == name)
            return val;
        else
            return rest->lookup(find_name);
            }
};
