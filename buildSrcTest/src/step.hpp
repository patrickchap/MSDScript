#pragma once
#include "pointer.hpp"
#include "Env.hpp"
#include "expr.hpp"
#include "cont.hpp"

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
    
    /**
     * Interprests the passed Expr using continuations to get the value of the Expr
     *
     * @param e
     *  the Expr to interpret
     */
    static PTR(Val) interp_by_steps(PTR(Expr) e);
};
