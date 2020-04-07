#include "step.hpp"



Step::mode_t Step::mode;

PTR(Cont) Step::cont;
PTR(Val) Step::val;
//PTR(Cont) Cont::done;
PTR(Expr) Step::expr; /* for interp_mode */
PTR(Env) Step::env; /* for interp_mode */



PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    Step::mode = Step::interp_mode;
    Step::expr = e;
    Step::env = Env::empty;
    Step::val = nullptr;
    Step::cont = Cont::done;
    
    while (1) {
        if (Step::mode == Step::interp_mode){
            Step::expr->step_interp();
        }
            else {
                if (Step::cont == Cont::done)
                    return Step::val;
                else{
                    Step::cont->step_continue();
                }

                    }
    }
    

}
