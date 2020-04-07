#include "expr.hpp"
#include <stdexcept>
#include "catch.hpp"
#include "value.hpp"
#include "Env.hpp"
#include "step.hpp"


NumVal::NumVal(int rep) {
    this->rep = rep;
}

bool NumVal::equals(PTR(Val) other_val) {
     PTR(NumVal) other_num_val = CAST(NumVal)(other_val);
    if (other_num_val == nullptr)
        return false;
    else
        return rep == other_num_val->rep;
}

PTR(Val) NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal) other_num_val = CAST(NumVal)(other_val);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return NEW(NumVal)(rep + other_num_val->rep);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val) {
    PTR(NumVal) other_num_val = CAST(NumVal)(other_val);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return NEW(NumVal)(rep * other_num_val->rep);
}

PTR(Expr) NumVal::to_expr() {
    return NEW(NumberExpr)(rep);
}

std::string NumVal::to_string() {
    return std::to_string(rep);
}

bool NumVal::is_true(){
    return false;
}

PTR(Val) NumVal::call(PTR(Val) actual_arg){

    return nullptr;
}

void NumVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest){
    
}

BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}

bool BoolVal::equals(PTR(Val) other_val) {
    PTR(BoolVal) other_bool_val = CAST(BoolVal)(other_val);
    if (other_bool_val == nullptr)
        return false;
    else
        return rep == other_bool_val->rep;
}

PTR(Val) BoolVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("no adding booleans");
}

PTR(Val) BoolVal::mult_with(PTR(Val) other_val) {
    throw std::runtime_error("no multiplying booleans");
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(rep);
}

std::string BoolVal::to_string() {
    if (rep)
        return "_true";
    else
        return "_false";
}

bool BoolVal::is_true(){
    return this->rep;
}


PTR(Val) BoolVal::call(PTR(Val) actual_arg){
    return THIS;
}

void BoolVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest){
    
}

FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env){
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;

}

bool FunVal::equals(PTR(Val) val){
    PTR(FunVal) f = CAST(FunVal)(val);
    if(f == NULL){
        return false;
    }else{
        return this->formal_arg == formal_arg && this->body->equals(f->body);
    }

}


PTR(Val) FunVal::add_to(PTR(Val) other_val){
     throw std::runtime_error("no adding FunVal");
}

PTR(Val) FunVal::mult_with(PTR(Val) other_val){
    
    throw std::runtime_error("no multiplying FunVal");

}

PTR(Expr) FunVal::to_expr(){
    return NEW(FunExpr)(this->formal_arg, this->body);
}

std::string FunVal::to_string(){
    return this->formal_arg + " " + this->body->to_string();
}


bool FunVal::is_true(){
    return false;
}

void FunVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(env, this->formal_arg, actual_arg_val);
    Step::cont = rest;
}


PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return body->interp(NEW(ExtendedEnv)(this->env,this->formal_arg, actual_arg));

}


TEST_CASE( "FunVal equal" ) {
    CHECK( (NEW (FunVal)("x" , NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x")), NEW(EmptyEnv)()))
          ->equals((NEW (FunVal)("x" , NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x")), NEW(EmptyEnv)()))) );

    CHECK( (NEW (FunVal)("x" , NEW (NumberExpr)(1), NEW (EmptyEnv)()))
          ->equals((NEW (FunVal)("x" , NEW (NumberExpr)(1),NEW(EmptyEnv)()))) );
}

TEST_CASE( "FunVal to_expr" ) {
    CHECK( (NEW (FunVal)("x" , NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x")), NEW(EmptyEnv)()))->to_expr()
          ->equals((NEW (FunExpr)("x" , NEW (AddExpr)(NEW (NumberExpr)(1), NEW (VariableExpr)("x"))))) );

    CHECK( (NEW (FunVal)("x" , NEW (NumberExpr)(1), NEW(EmptyEnv)()))
          ->to_expr()
          ->equals((NEW (FunExpr)("x" , NEW (NumberExpr)(1)))) );
}


TEST_CASE( "values equals" ) {
    CHECK( (NEW (NumVal)(5))->equals(NEW (NumVal)(5)) );
    CHECK( ! (NEW (NumVal)(7))->equals(NEW (NumVal)(5)) );

    CHECK( (NEW (BoolVal)(true))->equals(NEW (BoolVal)(true)) );
    CHECK( ! (NEW (BoolVal)(true))->equals(NEW (BoolVal)(false)) );
    CHECK( ! (NEW (BoolVal)(false))->equals(NEW (BoolVal)(true)) );

    CHECK( ! (NEW (NumVal)(7))->equals(NEW (BoolVal)(false)) );
    CHECK( ! (NEW (BoolVal)(false))->equals(NEW (NumVal)(8)) );
}

TEST_CASE( "add_to" ) {

    CHECK ( (NEW (NumVal)(5))->add_to(NEW (NumVal)(8))->equals(NEW (NumVal)(13)) );

    CHECK_THROWS_WITH ( (NEW (NumVal)(5))->add_to(NEW (BoolVal)(false)), "not a number" );
    CHECK_THROWS_WITH ( (NEW (BoolVal)(false))->add_to(NEW (BoolVal)(false)),
                       "no adding booleans" );
}

TEST_CASE( "mult_with" ) {

    CHECK ( (NEW (NumVal)(5))->mult_with(NEW (NumVal)(8))->equals(NEW (NumVal)(40)) );

    CHECK_THROWS_WITH ( (NEW (NumVal)(5))->mult_with(NEW (BoolVal)(false)), "not a number" );
    CHECK_THROWS_WITH ( (NEW (BoolVal)(false))->mult_with(NEW (BoolVal)(false)),
                       "no multiplying booleans" );
}

TEST_CASE( "value to_expr" ) {
    CHECK( (NEW (NumVal)(5))->to_expr()->equals(NEW (NumberExpr)(5)) );
    CHECK( (NEW (BoolVal)(true))->to_expr()->equals(NEW (BoolExpr)(true)) );
    CHECK( (NEW (BoolVal)(false))->to_expr()->equals(NEW (BoolExpr)(false)) );
}

TEST_CASE( "value to_string" ) {
    CHECK( (NEW (NumVal)(5))->to_string() == "5" );
    CHECK( (NEW (BoolVal)(true))->to_string() == "_true" );
    CHECK( (NEW (BoolVal)(false))->to_string() == "_false" );
}
