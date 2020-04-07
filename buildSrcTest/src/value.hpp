#pragma once
#include <iostream>
#include "pointer.hpp"
//#include "cont.hpp"
//#include "Env.hpp"



/* A forward declaration, so `Val` can refer to `Expr`, while
 `Expr` still needs to refer to `Val`. */
class Cont;
class Expr;
class Env;


class Val ENABLE_THIS(Val){
public:
    virtual bool equals(PTR(Val) val) = 0;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other_val) = 0;
    virtual PTR(Expr) to_expr() = 0;
    virtual std::string to_string() = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) = 0;
};

class NumVal : public Val {
public:
    int rep;
    NumVal(int rep);
    
    /**
     * Compares this Val with the Val passed to the method
     *
     * @param val
     *  the val to compare with this Val
     */
    bool equals(PTR(Val) val);
    
    /**
     * Adds the passed Val with this Val
     *
     * @param other_val
     *  the value to add to this Val
     */
    PTR(Val) add_to(PTR(Val) other_val);
    
    /**
     * Multiplies the passed Val with this Val
     *
     * @param other_val
     *  the value to multiply to this Val
     */
    PTR(Val) mult_with(PTR(Val) other_val);
    
    /**
     * Converts this Val to an Expr
     *
     */
    PTR(Expr) to_expr();
    
    /**
     * Returns the string representation of this Val
     *
     */
    std::string to_string();
    bool is_true();
    
    /**
     * Returns nullptr for NumVal
     *
     */
    PTR(Val) call(PTR(Val) actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

class BoolVal : public Val {
public:
    bool rep;
    BoolVal(bool rep);
    
    /**
     * Compares this Val with the Val passed to the method
     *
     * @param val
     *  the val to compare with this Val
     */
    bool equals(PTR(Val) val);
    
    /**
     * Adds the passed Val with this Val
     *
     * @param other_val
     *  the value to add to this Val
     */
    PTR(Val) add_to(PTR(Val) other_val);
    
    /**
     * Multiplies the passed Val with this Val
     *
     * @param other_val
     *  the value to multiply to this Val
     */
    PTR(Val) mult_with(PTR(Val) other_val);
    
    /**
     * Converts this Val to an Expr
     *
     */
    PTR(Expr) to_expr();
    
    /**
     * Returns the string representation of this Val
     *
     */
    std::string to_string();
    bool is_true();
    
    /**
     *
     *
     */
    PTR(Val) call(PTR(Val) actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};



class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    

    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    
    /**
     * Compares this Val with the Val passed to the method
     *
     * @param val
     *  the val to compare with this Val
     */
    bool equals(PTR(Val) val);
    
    /**
     * Adds the passed Val with this Val
     *
     * @param other_val
     *  the value to add to this Val
     */
    PTR(Val) add_to(PTR(Val) other_val);
    
    /**
     * Multiplies the passed Val with this Val
     *
     * @param other_val
     *  the value to multiply to this Val
     */
    PTR(Val) mult_with(PTR(Val) other_val);
    
    /**
     * Converts this Val to an Expr
     *
     */
    PTR(Expr) to_expr();
    
    /**
     * Returns the string representation of this Val
     *
     */
    std::string to_string();
    bool is_true();
    
    /**
     *  Interprets the body of this Val
     *
     * @param actual_arg
     *  Val to create a new ExtendedEnv with
     */
    PTR(Val) call(PTR(Val)actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};
