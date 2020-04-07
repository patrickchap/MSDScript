#include "catch.hpp"
#include <string>
#include "expr.hpp"
#include <iostream>
#include "pointer.hpp"
#include <memory>
#include "Env.hpp"
#include "step.hpp"




NumberExpr::NumberExpr(int val) {
    this->val = val;
}

bool NumberExpr::equals(PTR(Expr) e) {
    PTR(NumberExpr) n = CAST(NumberExpr)(e);
    if (n == NULL)
        return false;
    else
        return val == n->val;
}

PTR(Val) NumberExpr::interp(PTR(Env) env){
    return NEW(NumVal)(val);
}


void NumberExpr::step_interp(){
    
    Step::mode = Step::continue_mode;
    Step::val = NEW(NumVal)(this->val);
    Step::cont = Step::cont;
    
}

PTR(Expr) NumberExpr::subst(std::string var, PTR(Val) value){
    return NEW(NumberExpr)(this->val);
}


bool NumberExpr::contains_variables(){
    return false;
}


std::string NumberExpr::to_string(){
    return std::to_string(this->val);
}

PTR(Expr) NumberExpr::optimize(){
    return THIS;
}



VariableExpr::VariableExpr(std::string var){
    this->var = var;
}


bool VariableExpr::equals(PTR(Expr) e) {
    PTR(VariableExpr) a = CAST(VariableExpr)(e);
    if (a == NULL)
        return false;
    else
        return this->var == a->var;
}


PTR(Val) VariableExpr::interp(PTR(Env) env){
    return env->lookup(this->var);
}


void VariableExpr::step_interp(){
    
    Step::mode = Step::continue_mode;
    PTR(Val) v = (Step::env->lookup(this->var));
    Step::val = v;
    Step::cont = Step::cont;
    
}



PTR(Expr) VariableExpr::subst(std::string var, PTR(Val) value){
    if(this->var == var){
        return value->to_expr();
    }else{
        return NEW(VariableExpr)(this->var);
    }
}


bool VariableExpr::contains_variables(){
    return true;
}


std::string VariableExpr::to_string(){
    return this->var;
}

PTR(Expr) VariableExpr::optimize(){
    return NEW(VariableExpr)(this->var);
}


AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}


bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) a = CAST(AddExpr)(e);
    if (a == NULL)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}


PTR(Val) AddExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->add_to(rhs->interp(env));
}


 void AddExpr::step_interp() {
     Step::mode = Step::interp_mode;
     Step::expr = lhs;
     Step::env = Step::env;
     Step::cont = NEW(RightThenAddCont)(rhs, Step::env, Step::cont);
}
 

PTR(Expr) AddExpr::subst(std::string var, PTR(Val) value){
    return NEW(AddExpr)(lhs->subst(var, value), rhs->subst(var, value));
}


bool AddExpr::contains_variables(){
    return this->rhs->contains_variables() || this->lhs->contains_variables();
}


std::string AddExpr::to_string(){
    return "(" + this->lhs->to_string() + " + " + this->rhs->to_string() + ")";
}


PTR(Expr) AddExpr::optimize(){
    if(!this->contains_variables()){
        return (this->lhs->interp(NEW(EmptyEnv)())->add_to(this->rhs->interp(NEW(EmptyEnv)())))->to_expr();
    }else{
         return NEW(AddExpr)(this->lhs->optimize(), this->rhs->optimize() );

    }

}



MultExpr::MultExpr(PTR(Expr)lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}


bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr) m = CAST(MultExpr)(e);
    if (m == NULL)
        return false;
    else
        return (this->lhs->equals(m->lhs)
                && this->rhs->equals(m->rhs));
}

PTR(Val) MultExpr::interp(PTR(Env) env){
    return lhs->interp(env)->mult_with(rhs->interp(env));
}


void MultExpr::step_interp(){
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenMultCont)(rhs, Step::env, Step::cont);
    
}



PTR(Expr) MultExpr::subst(std::string var, PTR(Val) value){
    return  NEW(MultExpr)(lhs->subst(var, value), rhs->subst(var, value));
}


bool MultExpr::contains_variables(){
    return this->rhs->contains_variables() || this->lhs->contains_variables();
}


std::string MultExpr::to_string(){
    return "(" + this->lhs->to_string() + " * " + this->rhs->to_string() + ")";
}



PTR(Expr) MultExpr::optimize(){
    if(!this->contains_variables()){
        return (this->lhs->interp(NEW(EmptyEnv)())->mult_with(this->rhs->interp(NEW(EmptyEnv)())))->to_expr();
    }else{
        return NEW(MultExpr)(this->lhs->optimize(), this->rhs->optimize() );
    }
}



BoolExpr::BoolExpr(bool rep){
    this->rep = rep;
}

bool BoolExpr::equals(PTR(Expr) e){

    PTR(BoolExpr) a = CAST(BoolExpr)(e);
    if (a == NULL)
        return false;
    else
        return this->rep == a->rep;
}


PTR(Val) BoolExpr::interp(PTR(Env) env){
    return NEW(BoolVal)(this->rep);
}

void BoolExpr::step_interp(){
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(this->rep);
    Step::cont = Step::cont;
}

PTR(Expr) BoolExpr::subst(std::string var, PTR(Val) value){
    return THIS;
}

bool BoolExpr::contains_variables(){
    return false;
}


std::string BoolExpr::to_string(){
    if(this->rep)
        return " true ";
    else
        return " false ";
}
PTR(Expr) BoolExpr::optimize(){
    return NEW(BoolExpr)(this->rep);
}


LetExpr::LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body){
    this->var = var;
    this->rhs = rhs;
    this->body = body;
}

PTR(Val) LetExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(env, var, rhs_val);
    return body->interp(new_env);
}


void LetExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = Step::env;
    Step::cont = NEW(LetBodyCont)(var, body, Step::env, Step::cont);
}



PTR(Expr) LetExpr::subst(std::string var, PTR(Val) value){
    return NEW(LetExpr)(this->var, this->rhs->subst(var, value), this->body->subst(var, value));
}


bool LetExpr::contains_variables(){
    return true;
}

std::string LetExpr::to_string(){
    return "(_let " + this->var  + " = " + this->rhs->to_string() + " _in " + this->body->to_string() + ")";
}


bool LetExpr::equals(PTR(Expr) e){
    
    PTR(LetExpr) m = CAST(LetExpr)(e);
    if (m == NULL)
        return false;
    else
        return (this->var == m->var
                
                && this->rhs->equals(m->rhs)
                && this->body->equals(m->body));
}


PTR(Expr) LetExpr::optimize(){
    if(!rhs->contains_variables()){
        return this->body->subst(var, rhs->interp(NEW(EmptyEnv)()))->optimize();
    }else{
        return NEW(LetExpr)(this->var, this->rhs->optimize(), this->body->optimize());
    }
}


IfExpr::IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part){
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}


PTR(Val) IfExpr::interp(PTR(Env) env){
    if (test_part->interp(PTR(Env) (env))->is_true())
        return then_part->interp(PTR(Env) (env));
    else
        return else_part->interp(PTR(Env) (env));
}


void IfExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = test_part;
    Step::env = Step::env;
    Step::cont = NEW(IfBranchCont)(then_part, else_part, Step::env, Step::cont);
}



PTR(Expr) IfExpr::subst(std::string var, PTR(Val) value){
    return NEW(IfExpr)(this->test_part->subst(var, value), this->then_part->subst(var, value), this->else_part->subst(var, value));
}


bool IfExpr::contains_variables(){
    return this->test_part->contains_variables() || this->then_part->contains_variables() || this->else_part->contains_variables();
}

std::string IfExpr::to_string(){
    return "(_if " + test_part->to_string() + " _then " + then_part->to_string() + " _else " + else_part->to_string() + ")";
}


PTR(Expr) IfExpr::optimize(){
    
    if(this->test_part->optimize()->equals(NEW( BoolExpr)(true)))
        return then_part->optimize();
    else if(this->test_part->optimize()->equals(NEW(BoolExpr)(false)))
        return else_part->optimize();
    else
        return NEW(IfExpr)(this->test_part->optimize(), this->then_part->optimize(), this->else_part->optimize());
}



bool IfExpr::equals(PTR(Expr) e){
    PTR(IfExpr) m = CAST(IfExpr)(e);
    if (m == NULL)
        return false;
    else
        return (this->test_part->equals(m->test_part)
                && this->then_part->equals(m->then_part)
                && this->else_part->equals(m->else_part));
}


EqualsEqualsExpr::EqualsEqualsExpr(PTR(Expr) lhs, PTR(Expr) rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}


PTR(Val) EqualsEqualsExpr::interp(PTR(Env) env){
    if(this->lhs->interp(PTR(Env) (env))->equals(this->rhs->interp(PTR(Env) (env))))
        return NEW(BoolVal)(true);
    else
        return NEW(BoolVal)(false);
}

void EqualsEqualsExpr::step_interp(){
    
    Step::mode = Step::interp_mode;
    Step::expr = this->lhs;
    Step::env = Step::env;

    Step::cont = NEW(RightThenEqualEqualCont)(rhs, Step::env, Step::cont);
    
    
}


PTR(Expr) EqualsEqualsExpr::subst(std::string var, PTR(Val) value){
    return NEW(EqualsEqualsExpr)(this->lhs->subst(var, value), this->rhs->subst(var, value));
}

bool EqualsEqualsExpr::contains_variables(){
    return this->lhs->contains_variables() || this->rhs->contains_variables();
}


std::string EqualsEqualsExpr::to_string(){
    return "(" + this->lhs->to_string() + " == " + this->rhs->to_string() + ")";
}

PTR(Expr) EqualsEqualsExpr::optimize(){
    
    if (this->lhs->contains_variables() || this->rhs->contains_variables()) {
         return NEW(EqualsEqualsExpr)(this->lhs->optimize(), this->rhs->optimize());
    }else if(this->lhs->optimize()->equals(this->rhs->optimize())){
        return NEW(BoolExpr)(true);
    }else{
         return NEW(BoolExpr)(false);
    }

}



bool EqualsEqualsExpr::equals(PTR(Expr) e){
    PTR(EqualsEqualsExpr) m = CAST(EqualsEqualsExpr)(e);
    if (m == NULL)
        return false;
    else
        return (this->lhs->equals(m->lhs)
                && this->rhs->equals(m->rhs));
}


FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr)e){
    PTR(FunExpr) f = CAST(FunExpr)(e);
    if(f == NULL){
        return false;
    }else{
        return (this->formal_arg == f->formal_arg && this->body->equals(f->body));
    }
    
}


PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg, body, env);
}


void FunExpr::step_interp(){
    
    Step::mode = Step::continue_mode;
    Step::val = NEW(FunVal)(formal_arg, body, Step::env);
    Step::cont = Step::cont;
    
}


PTR(Expr)  FunExpr::subst(std::string var, PTR(Val) value){
    if(this->formal_arg != var){
        return NEW(FunExpr)(this->formal_arg, this->body->subst(var, value));
    }else
        return NEW(FunExpr)(this->formal_arg, this->body);
}

bool FunExpr::contains_variables(){
    return this->body->contains_variables();
}

std::string  FunExpr::to_string(){
    return "(_fun (" + this->formal_arg + ") " + this->body->to_string() + ")";
}


PTR(Expr) FunExpr::optimize(){
    return NEW(FunExpr)(this->formal_arg, this->body->optimize());
}


CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg){
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
    
}


bool CallExpr::equals(PTR(Expr) e){
     PTR(CallExpr) c = CAST(CallExpr)(e);
    if(c == NULL){
        return false;
    }else{
        return (this->to_be_called->equals(c->to_be_called) && this->actual_arg->equals(c->actual_arg) );
    }
}

PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}



void CallExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = to_be_called;
    Step::cont = NEW(ArgThenCallCont)(actual_arg, Step::env, Step::cont);
}

PTR(Expr) CallExpr::subst(std::string var, PTR(Val) value){

    return NEW(CallExpr)(this->to_be_called->subst(var, value), this->actual_arg->subst(var, value));
}

bool CallExpr::contains_variables(){
    return this->to_be_called->contains_variables() || this->actual_arg->contains_variables();
}

std::string CallExpr::to_string(){
    return "(" + this->to_be_called->to_string() + " (" + this->actual_arg->to_string() + "))";
}

PTR(Expr) CallExpr::optimize(){
    return NEW(CallExpr)(this->to_be_called->optimize(), this->actual_arg->optimize());
}



TEST_CASE( "Fun interp" ){

    CHECK( NEW(LetExpr)("f",NEW(FunExpr)( "x",NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumberExpr)(1))), NEW(CallExpr)(NEW(VariableExpr)("f"), NEW(NumberExpr)(10)))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(11)) );
}


TEST_CASE( "CallExpr contains_variabls" ) {
    CHECK( (NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(2)))
          ->contains_variables() == false);

    CHECK( (NEW (CallExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(2)))
          ->contains_variables());
}

TEST_CASE( "CallExpr equals" ) {
    CHECK( (NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(2)))
          ->equals((NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(2)))));
    CHECK( (NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(5)))
          ->equals((NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(2)))) == false);

    CHECK( (NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(5)))
          ->equals((NEW (NumberExpr)(1))) == false);
}

TEST_CASE( "CallExpr subst" ) {
    CHECK( (NEW (CallExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x")))
          ->subst("x", NEW (NumVal)(10))
          ->equals((NEW (CallExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(10)))));

    CHECK( (NEW (CallExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("y")))
          ->subst("x", NEW (NumVal)(10))
          ->equals((NEW (CallExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("y")))));
}

TEST_CASE( "FunExpr interp" ) {
    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(1))))
          ->optimize()
          ->equals((NEW (FunExpr)("x", NEW (NumberExpr)(2)))));

    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))
          ->optimize()
          ->equals((NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))));
}

TEST_CASE( "FunExpr contains_variables" ) {
    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))
          ->contains_variables() == true);

    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(6))))
          ->contains_variables() == false);
}

TEST_CASE( "FunExpr subst" ) {
    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))
          ->subst("x", NEW (NumVal)(5))
          ->equals((NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))) );
}

TEST_CASE( "FunExpr equals" ) {
    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))
          ->equals((NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))) );
    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))
          ->equals((NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("y"))))) == false);
    CHECK( (NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))
          ->equals(NEW (NumberExpr)(3)) == false );
}

TEST_CASE( "Equals Opt" ) {
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(10)))
          ->optimize()->equals(NEW (BoolExpr)(true)) );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(11), NEW (NumberExpr)(10)))
          ->optimize()->equals(NEW (BoolExpr)(false)) );

    CHECK( (NEW (EqualsEqualsExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(10)))
          ->optimize()->equals((NEW (EqualsEqualsExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(10)))) );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(5), NEW (VariableExpr)("x")))
          ->optimize()->equals((NEW (EqualsEqualsExpr)(NEW (NumberExpr)(5), NEW (VariableExpr)("x")))) );
}

TEST_CASE( "Equals containsVariables" ) {
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (VariableExpr)("x")))
          ->contains_variables() == true );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (VariableExpr)("x"), NEW (VariableExpr)("x")))
          ->contains_variables() == true );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(10)))
          ->contains_variables() == true );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(10)))
          ->contains_variables() == false );
}

TEST_CASE( "Equals subst" ) {
     CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (VariableExpr)("x")))
           ->subst("x", NEW (NumVal)(10))->equals(NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(10))) );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (VariableExpr)("x")))
          ->subst("y", NEW (NumVal)(10))->equals((NEW (EqualsEqualsExpr)(NEW (NumberExpr)(10), NEW (VariableExpr)("x")))) );
}

TEST_CASE( "Equals value" ) {
    CHECK( (NEW (EqualsEqualsExpr)(NEW (BoolExpr)(true), NEW (BoolExpr)(true)))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(true)) );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (BoolExpr)(false), NEW (BoolExpr)(true)))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(false)) );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(5), NEW (NumberExpr)(5)))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(true)) );
    CHECK( (NEW (EqualsEqualsExpr)(NEW (NumberExpr)(5), NEW (NumberExpr)(6)))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(false)) );

}

TEST_CASE( "IfExpr value" ) {
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (NumberExpr)(5)))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(10)) );
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(false), NEW (NumberExpr)(10), NEW (NumberExpr)(5)))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(5)) );

}

TEST_CASE( "IfExpr subs" ) {
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))->subst("x",NEW (NumVal)(5))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(10)) );
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(false), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))->subst("x",NEW (NumVal)(5))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(5)) );
}

TEST_CASE( "IfExpr contains_variable" ) {
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))->contains_variables() == true );
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (VariableExpr)("x"), NEW (NumberExpr)(5)))->contains_variables() == true );
    CHECK( (NEW (IfExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(10), NEW (NumberExpr)(5)))->contains_variables() == true );
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (NumberExpr)(5)))->contains_variables() == false);

}

TEST_CASE( "IfExpr equals" ) {
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))->equals((NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))));
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(1), NEW (VariableExpr)("x")))
          ->equals((NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))) == false);
    CHECK( (NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("y")))
          ->equals((NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))) == false);
     CHECK( (NEW (IfExpr)(NEW (BoolExpr)(false), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))
           ->equals((NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumberExpr)(10), NEW (VariableExpr)("x")))) == false);
}

TEST_CASE( "boolExpr equals" ) {
    CHECK((NEW (BoolExpr)(true))->equals(NEW (BoolExpr)(true)) );
    CHECK((NEW (BoolExpr)(false))->equals(NEW (BoolExpr)(false)) );
    CHECK(!(NEW (BoolExpr)(false))->equals(NEW (BoolExpr)(true)) );
    CHECK(!(NEW (BoolExpr)(true))->equals(NEW (BoolExpr)(false)) );
}

TEST_CASE( "boolExpr value" ) {
    CHECK((NEW (BoolExpr)(true))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(true)) );
    CHECK((NEW (BoolExpr)(false))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(false)) );
    CHECK(!(NEW (BoolExpr)(true))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(false)) );
    CHECK(!(NEW (BoolExpr)(false))->interp(NEW(EmptyEnv)())->equals(NEW (BoolVal)(true)) );
}



TEST_CASE( "letExpr" ) {
    CHECK( (NEW (LetExpr)("x",NEW (NumberExpr)(10), NEW (AddExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(5))))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(15))  );
    CHECK( (NEW (LetExpr)("x",NEW (NumberExpr)(10), NEW (AddExpr)(NEW (VariableExpr)("x"), NEW (NumberExpr)(5))))
           ->to_string() == "(_let x = 10 _in (x + 5))" );

    CHECK( (NEW (LetExpr)("x",
                        NEW (AddExpr)(NEW (NumberExpr)(2), NEW (NumberExpr)(3)),
                        NEW (MultExpr)(NEW (VariableExpr)("x"), NEW (VariableExpr)("x"))))
          ->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(25)) );
}


TEST_CASE( "letExpr equals" ) {

    CHECK( (NEW (LetExpr)("x",
                        NEW (AddExpr)(NEW (NumberExpr)(2), NEW (NumberExpr)(3)),
                        NEW (MultExpr)(NEW (VariableExpr)("x"), NEW (VariableExpr)("x"))))
          ->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(26)) == false );
}

TEST_CASE( "value" ) {
    CHECK( (NEW (NumberExpr)(10))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(10)) );
    CHECK( (NEW (AddExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(5)))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(6)) );
    CHECK( (NEW (MultExpr)(NEW (NumberExpr)(5), NEW (NumberExpr)(2)))->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(10)) );

}


TEST_CASE(  "subst" ){

    CHECK(  (NEW (NumberExpr)(10))->subst("dog", NEW (NumVal)(3)) ->equals(NEW (NumberExpr)(10)) );
    CHECK(  (NEW (VariableExpr)("fish"))->subst("dog", NEW (NumVal)(3)) ->equals(NEW (VariableExpr)("fish")) );
    CHECK(  (NEW (VariableExpr)("dog"))->subst("dog", NEW (NumVal)(3)) ->equals(NEW (NumberExpr)(3)) );
    CHECK(  (NEW (AddExpr)( NEW (NumberExpr)(2), NEW (VariableExpr)("dog")))
          ->subst("dog", NEW (NumVal)(3)) ->equals(NEW (AddExpr)( NEW (NumberExpr)(2), NEW (NumberExpr)(3))) );
    CHECK(  (NEW (MultExpr)( NEW (NumberExpr)(2), NEW (VariableExpr)("dog")))
          ->subst("dog", NEW (NumVal)(3)) ->equals(NEW (MultExpr)( NEW (NumberExpr)(2), NEW (NumberExpr)(3))) );
}


TEST_CASE(  "contains_variable" ){

    CHECK( (NEW (VariableExpr)( "Fish" ))->contains_variables() == true );
    CHECK( (NEW (NumberExpr)( 1 ))->contains_variables() == false );

    //add no variable
    CHECK( (NEW (AddExpr)( NEW (NumberExpr)(2), NEW (NumberExpr)(5) ))->contains_variables() == false );
    //add with one variable rhs
    CHECK( (NEW (AddExpr)( NEW (NumberExpr)(2), NEW (VariableExpr)("Fish") ))->contains_variables() == true );
    //add with one variable lhs
    CHECK( (NEW (AddExpr)( NEW (VariableExpr)("fish"), NEW (NumberExpr)(1) ))->contains_variables() == true );
    //add with two variables
    CHECK( (NEW (AddExpr)( NEW (VariableExpr)("dog"), NEW (VariableExpr)("fish") ))->contains_variables() == true );


    //Mult with no variables
    CHECK( (NEW (MultExpr)( NEW (NumberExpr)(2), NEW (NumberExpr)(5) ))->contains_variables() == false );
    //Mult with one variable rhs
    CHECK( (NEW (MultExpr)( NEW (NumberExpr)(2), NEW (VariableExpr)("Fish") ))->contains_variables() == true );
    //Mult with one variable lhs
    CHECK( (NEW (MultExpr)( NEW (VariableExpr)("fish"), NEW (NumberExpr)(1) ))->contains_variables() == true );
    //Mult with two variables
    CHECK( (NEW (MultExpr)( NEW (VariableExpr)("dog"), NEW (VariableExpr)("fish") ))->contains_variables() == true );

    CHECK((NEW (MultExpr)(NEW (NumberExpr)(3),NEW (AddExpr)(NEW (NumberExpr)(2), NEW (VariableExpr)("width"))))
          ->contains_variables() == true);

    CHECK((NEW (MultExpr)(NEW (NumberExpr)(3),NEW (AddExpr)(NEW (NumberExpr)(2), NEW (NumberExpr)(1))))
          ->contains_variables() == false);
}



TEST_CASE( "to_string " ){

    CHECK( (NEW (AddExpr)(NEW (NumberExpr)(5), NEW (AddExpr)(NEW (VariableExpr)("y"),
                                           NEW (MultExpr)(NEW (NumberExpr)(2), NEW (NumberExpr)(3)))))->to_string() == "(5 + (y + (2 * 3)))" );

    CHECK( (NEW (AddExpr)(NEW (NumberExpr)(5), NEW (AddExpr)(NEW (NumberExpr)(6),
                                           NEW (MultExpr)(NEW (NumberExpr)(2), NEW (NumberExpr)(3)))))->to_string() == "(5 + (6 + (2 * 3)))" );

    CHECK( (NEW (AddExpr)(NEW (NumberExpr)(5), NEW (AddExpr)(NEW (NumberExpr)(5),
                                           NEW (MultExpr)(NEW (VariableExpr)("y"), NEW (NumberExpr)(3)))))
                                            ->to_string() == "(5 + (5 + (y * 3)))");
}
