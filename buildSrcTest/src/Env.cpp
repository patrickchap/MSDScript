#include "Env.hpp"
#include "pointer.hpp"
#include "catch.hpp"



EmptyEnv::EmptyEnv(){
    
}

PTR(Env) Env::empty = NEW(EmptyEnv)();



ExtendedEnv::ExtendedEnv(PTR(Env) rest, std::string name, PTR(Val) val){
    this->rest = rest;
    this->name = name;
    this->val = val;
}
