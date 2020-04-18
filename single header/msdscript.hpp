#pragma once

#ifndef pointer_h
#define pointer_h

#if 0

# define NEW(T)  new T
# define PTR(T)  T*
# define CAST(T) dynamic_cast<T*>
# define THIS    this
# define ENABLE_THIS(T) /* empty */

#else

# define NEW(T)  std::make_shared<T>
# define PTR(T)  std::shared_ptr<T>
# define CAST(T) std::dynamic_pointer_cast<T>
# define THIS  shared_from_this()
# define ENABLE_THIS(T) : public std::enable_shared_from_this<T>

#endif

#endif /* pointer_h */


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
    PTR(Val) lookup(std::string find_name) {
        if (find_name == name)
            return val;
        else
            return rest->lookup(find_name);
            }
};






//class Env;

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
    NumberExpr(int val);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    void step_interp();
    
};

class VariableExpr : public Expr {
public:
    std::string var;
    
    VariableExpr(std::string var);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
};

class AddExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    AddExpr(PTR(Expr)lhs, PTR(Expr) rhs);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    void step_interp();
};

class MultExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    MultExpr(PTR(Expr) lhs, PTR(Expr)rhs);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
    
};


class LetExpr : public Expr {
public:
    std::string var;
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body);
    
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    bool equals(PTR(Expr) e);
    PTR(Expr) optimize();
    
    
    void step_interp();
    
};

class BoolExpr : public Expr {
public:
    bool rep;
    
    BoolExpr(bool rep);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    void step_interp();
    
};

class IfExpr : public Expr {
public:
    
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);
    
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    bool equals(PTR(Expr) e);
    
    
    void step_interp();
    
};



class EqualsEqualsExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    
    EqualsEqualsExpr();
    EqualsEqualsExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    bool equals(PTR(Expr) e);
    
    
    void step_interp();
    
};


/**************************** Add Function Expression and  Call Expression *****************************/

class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;
    
    FunExpr(std::string formal_arg, PTR(Expr) body);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
};


class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
};



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
    void step_continue();
};


class AddCont : public Cont {
public:
    
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
};


class IfBranchCont : public Cont {
public:
    
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);
    
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;
    
    void step_continue();
};

class RightThenMultCont : public Cont {
public:
    
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
};

class MultCont : public Cont {
public:
    
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
};


class RightThenEqualEqualCont : public Cont {
public:
    
    RightThenEqualEqualCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
};

class EqualEqualCont : public Cont {
public:
    
    EqualEqualCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
};

class LetBodyCont : public Cont {
    
public:
    
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;
    
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);
    
    void step_continue();
};


class ArgThenCallCont : public Cont {
public:
    
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;
    
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest);
    
    void step_continue();
};



class CallCont : public Cont {
public:
    
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;
    
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);
    
    void step_continue();
};



class Step {
public:
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;
    
    static mode_t mode; /* chooses mode */
    static PTR(Expr) expr; /* for interp_mode */
    static PTR(Env) env; /* for interp_mode */
    static PTR(Val) val; /* for continue_mode */
    static PTR(Cont) cont; /* all modes */
    
    static PTR(Val) interp_by_steps(PTR(Expr) e);
};



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
    
    bool equals(PTR(Val) val);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    PTR(Expr) to_expr();
    std::string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val) actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

class BoolVal : public Val {
public:
    bool rep;
    BoolVal(bool rep);
    
    bool equals(PTR(Val) val);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    PTR(Expr) to_expr();
    std::string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val) actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};


/*************** add function value **************/

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    

    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    
    bool equals(PTR(Val) val);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    PTR(Expr) to_expr();
    std::string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val)actual_arg);
    
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};
#pragma once

#ifndef pointer_h
#define pointer_h

#if 0

# define NEW(T)  new T
# define PTR(T)  T*
# define CAST(T) dynamic_cast<T*>
# define THIS    this
# define ENABLE_THIS(T) /* empty */

#else

# define NEW(T)  std::make_shared<T>
# define PTR(T)  std::shared_ptr<T>
# define CAST(T) std::dynamic_pointer_cast<T>
# define THIS  shared_from_this()
# define ENABLE_THIS(T) : public std::enable_shared_from_this<T>

#endif

#endif /* pointer_h */


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
    PTR(Val) lookup(std::string find_name) {
        if (find_name == name)
            return val;
        else
            return rest->lookup(find_name);
            }
};






//class Env;

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
    NumberExpr(int val);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    void step_interp();
    
};

class VariableExpr : public Expr {
public:
    std::string var;
    
    VariableExpr(std::string var);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
};

class AddExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    AddExpr(PTR(Expr)lhs, PTR(Expr) rhs);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    void step_interp();
};

class MultExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    MultExpr(PTR(Expr) lhs, PTR(Expr)rhs);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
    
};


class LetExpr : public Expr {
public:
    std::string var;
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body);
    
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    bool equals(PTR(Expr) e);
    PTR(Expr) optimize();
    
    
    void step_interp();
    
};

class BoolExpr : public Expr {
public:
    bool rep;
    
    BoolExpr(bool rep);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    void step_interp();
    
};

class IfExpr : public Expr {
public:
    
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);
    
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    bool equals(PTR(Expr) e);
    
    
    void step_interp();
    
};



class EqualsEqualsExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    
    EqualsEqualsExpr();
    EqualsEqualsExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    bool equals(PTR(Expr) e);
    
    
    void step_interp();
    
};


/**************************** Add Function Expression and  Call Expression *****************************/

class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;
    
    FunExpr(std::string formal_arg, PTR(Expr) body);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
};


class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string var, PTR(Val) value);
    bool contains_variables();
    std::string to_string();
    PTR(Expr) optimize();
    
    
    void step_interp();
};



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
    void step_continue();
};


class AddCont : public Cont {
public:
    
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
};


class IfBranchCont : public Cont {
public:
    
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);
    
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;
    
    void step_continue();
};

class RightThenMultCont : public Cont {
public:
    
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
};

class MultCont : public Cont {
public:
    
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
};


class RightThenEqualEqualCont : public Cont {
public:
    
    RightThenEqualEqualCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
};

class EqualEqualCont : public Cont {
public:
    
    EqualEqualCont(PTR(Val) lhs_val, PTR(Cont) rest );
    
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
};

class LetBodyCont : public Cont {
    
public:
    
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;
    
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);
    
    void step_continue();
};


class ArgThenCallCont : public Cont {
public:
    
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;
    
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest);
    
    void step_continue();
};



class CallCont : public Cont {
public:
    
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;
    
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);
    
    void step_continue();
};



class Step {
public:
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;
    
    static mode_t mode; /* chooses mode */
    static PTR(Expr) expr; /* for interp_mode */
    static PTR(Env) env; /* for interp_mode */
    static PTR(Val) val; /* for continue_mode */
    static PTR(Cont) cont; /* all modes */
    
    static PTR(Val) interp_by_steps(PTR(Expr) e);
};



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
    
    bool equals(PTR(Val) val);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    PTR(Expr) to_expr();
    std::string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val) actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

class BoolVal : public Val {
public:
    bool rep;
    BoolVal(bool rep);
    
    bool equals(PTR(Val) val);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    PTR(Expr) to_expr();
    std::string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val) actual_arg);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};


/*************** add function value **************/

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    

    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    
    bool equals(PTR(Val) val);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    PTR(Expr) to_expr();
    std::string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val)actual_arg);
    
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

PTR(Expr) parse(std::istream &in);
