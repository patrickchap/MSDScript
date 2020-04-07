#pragma once
#include "value.hpp"
#include <string>
#include "pointer.hpp"
//#include "Env.hpp"


class Env;

class Expr ENABLE_THIS(Expr){
public:

    virtual  bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual PTR(Expr) subst(std::string var, PTR(Val) value) = 0;
    virtual bool contains_variables() = 0;
    virtual std::string to_string() = 0;
    virtual PTR(Expr) optimize() = 0;
    virtual void step_interp() = 0;
};

class NumberExpr : public Expr  {

public:
    int val;
    
    NumberExpr();
    
    /**
     * Expression to represent a int value
     *
     * @param val
     *   the actual value the the NumberExpr represents
     */
    NumberExpr(int val);
    
    /**
     * Compares this NumberExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this NumberExpr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     * Note: subst on NumberExpr returns a new NumberExpr with the NumberExpr's value
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplified
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
    
};

class VariableExpr : public Expr {
public:
    std::string var;
    
    VariableExpr(std::string var);
    
    /**
     * Compares this VariableExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */

    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     * Note: returns true for a VariableExpr
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplified
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
};

class AddExpr : public Expr {
public:

    PTR(Expr) lhs;
    PTR(Expr) rhs;

    AddExpr(PTR(Expr)lhs, PTR(Expr) rhs);
    
    /**
     * Compares this AddExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
};

class MultExpr : public Expr {
public:

    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    MultExpr(PTR(Expr) lhs, PTR(Expr)rhs);

    /**
     * Compares this MultExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
    
};


class LetExpr : public Expr {
public:
    std::string var;
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Compares this LetExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
    
};

class BoolExpr : public Expr {
public:
    bool rep;

    BoolExpr(bool rep);
    
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();

};

class IfExpr : public Expr {
public:
    
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Compares this IfExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();

};



class EqualsEqualsExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    
    EqualsEqualsExpr();
    EqualsEqualsExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Compares this EqualsEqualsExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
    
};



class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;

    FunExpr(std::string formal_arg, PTR(Expr) body);
    
    /**
     * Compares this FunExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
};


class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);

    /**
     * Compares this CallExpr agains a Expr*
     *
     * @param e
     *  the Expr* to compare against
     */
    bool equals(PTR(Expr) e);
    
    /**
     * Method to interpret this Expr to a numeric representation
     *
     * @param env
     *  The environment that holds a dictionary of variable to value pairs
     */
    PTR(Val) interp(PTR(Env) env);
    
    /**
     * Method used to substitue a variable with a value
     *
     *
     * @param var
     *  varaible to look for in the Expr
     * @param value
     *  value to substitute the variable in the Expr that matches the value
     */
    PTR(Expr) subst(std::string var, PTR(Val) value);
    
    /**
     * Method used to check if the Expr has a variable
     *
     */
    bool contains_variables();
    
    /**
     * Returns a string representation of this Expr
     */
    std::string to_string();
    
    /**
     * Returns this Expr after being simplifiee
     *
     */
    PTR(Expr) optimize();
    
    /**
     * Method to interpret this Expr to a numeric representation using continuations
     *
     */
    void step_interp();
};
