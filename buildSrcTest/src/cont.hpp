#pragma once
#include "pointer.hpp"
#include "expr.hpp"
#include "Env.hpp"

class Expr;
class Cont;

class Cont {
public:
    virtual void step_continue() = 0;
    static PTR(Cont) done;
};

class DoneCont : public Cont {
public:
    DoneCont();
    void step_continue();
};


class RightThenAddCont : public Cont {
public:
    RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    
    /**
     * Method a to remember the rhs Expr and add the value of lhs to the Continuations
     *
     */
    void step_continue();
};


class AddCont : public Cont {
public:
    
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    /**
     * Adds the lhs value to the rhs value and continue
     *
     */
    void step_continue();
};


class IfBranchCont : public Cont {
public:
    
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);
    
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;
    
    /**
     * Sets the global Expr to either the then or else part and puts the global mode into interp mode
     *
     */
    
    void step_continue();
};

class RightThenMultCont : public Cont {
public:
    
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    
    /**
     * Method a to remember the rhs Expr and add the value of lhs to the Continuations
     *
     */
    void step_continue();
};

class MultCont : public Cont {
public:
    
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    
    /**
     * Sets the global value to the value of the lhs value multiplied with the rhs value
     *
     */
    void step_continue();
};


class RightThenEqualEqualCont : public Cont {
public:

    RightThenEqualEqualCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    
    /**
     * Saves the rhs Expr to the global Expr and then adds the lhs value to the continuation
     *
     */
    void step_continue();
};

class EqualEqualCont : public Cont {
public:

    EqualEqualCont(PTR(Val) lhs_val, PTR(Cont) rest );

    PTR(Val) lhs_val;
    PTR(Cont) rest;
    
    /**
     * compares the lhs value to the rhs value and sets the global Val to true or false
     *
     */
    void step_continue();
};

class LetBodyCont : public Cont {
    
public:
    
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;
    
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);
    
    /**
     * Method to remember the body Expr and add the env, var, and Step::val to the global env
     *
     */
    void step_continue();
};


class ArgThenCallCont : public Cont {
public:
    
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;
    
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest);
    
    /**
     * Remeber the actual arg and set the global cont to a new call continuation
     *
     */
    void step_continue();
};



class CallCont : public Cont {
public:
    
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;
    
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);
    
    /**
     * calls call_step on the to_be_called_val with the global val and the rest continuation
     *
     */
    void step_continue();
};





