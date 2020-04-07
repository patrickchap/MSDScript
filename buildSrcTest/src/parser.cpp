
#include "parser.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include "catch.hpp"
#include "expr.hpp"
#include "pointer.hpp"
#include "Env.hpp"
#include "step.hpp"

static PTR(Expr) parse_comparg(std::istream &in);
static PTR(Expr) parse_addend(std::istream &in);
static PTR(Expr) parse_inner(std::istream &in);
static PTR(Expr) calc_number(std::istream &in);
static char peek_next(std::istream &in);
static std::string valid_Variable(std::istream &in);
static bool is_Alpha_Char(char c);
static PTR(Expr) parse_letExpr(std::istream &in);
static std::string keyWord(std::istream &in);
static PTR(Expr) parse_expr(std::istream &in);
static PTR(Expr) parse_true(std::istream &in);
static PTR(Expr) parse_false(std::istream &in);
static PTR(Expr) interp_if(std::istream &in);
static PTR(Expr) parse_fun(std::istream &in);
static PTR(Expr) parse_equals_equals_expr(PTR(Expr) lhs, PTR(Expr) rhs);
static PTR(Expr) parse_multicand(std::istream &in) ;


/* The new grammer that we're trying to parse is
 
 〈expr〉 = 〈comparg〉
        | 〈comparg〉 == 〈expr〉
 
 〈comparg〉 = 〈addend〉
        | 〈addend〉 + 〈comparg〉
 〈addend〉 = 〈multicand〉
        | 〈multicand〉 * 〈addend〉
 
 〈multicand〉 = 〈inner〉
        | 〈multicand〉 ( 〈expr〉 )
 〈inner〉 = 〈number〉
        | ( 〈expr〉 )
        | 〈variable〉
        | _let 〈variable〉 = 〈expr〉 _in 〈expr〉
        | _true
        | _false
        | _if 〈expr〉 _then 〈expr〉 _else 〈expr〉
        | _fun ( 〈variable〉 ) 〈expr〉
 
 */

//If matches the grammar, then
//Exit 2 if interpret and “errors”
//Exit 0 if interpret not errors
//Exit 0 always for optimize
//If not matches grammar (failed to parse), then exits with code 1

PTR(Expr) parse(std::istream &in) {
    
    PTR(Expr) num = parse_expr(in);
    
    if (!in.eof()) {
        perror("expected an end of line\n");
        exit(1);

    }
    return num;
}

/*
 〈expr〉 = 〈comparg〉
        | 〈expr〉 == 〈expr〉
 */

static PTR(Expr) parse_expr(std::istream &in){
    PTR(Expr) num = parse_comparg(in);
    char c = peek_next(in);
    
    if(c == '='){
        c = in.get();
        c = peek_next(in);
        if(c == '='){
             c = in.get();
            PTR(Expr) num2 = parse_expr(in);
            PTR(Expr) equals_equals = parse_equals_equals_expr(num, num2);
            return equals_equals;
        }
    }
    return num;
}

//〈comparg〉 = 〈addend〉
//      | 〈addend〉 + 〈comparg〉

static PTR(Expr) parse_comparg(std::istream &in) {
    
    PTR(Expr) num = parse_addend(in);
    char c = peek_next(in);

    if (c == '+') {
        // Must have another <expr> here,
        c = in.get();
        PTR(Expr) num2 = parse_comparg(in);
        num = NEW(AddExpr)(num, num2);
    }

    return num;
}

// 〈addend〉 = 〈multicand〉
//      | 〈multicand〉 * 〈addend〉

//takes an input stream that starts with an addend
//returns new Expr that represents an addend
static PTR(Expr) parse_addend(std::istream &in) {
    // added starts by parsing multicand
    PTR(Expr) num = parse_multicand(in);
    
    char c = peek_next(in);
    
    if (c == '*') {
        c = in.get();
        PTR(Expr) num2 = parse_addend(in);
        num = NEW(MultExpr)(num, num2);
    }
    return num;
}


//〈multicand〉 = 〈inner〉
//        | 〈multicand〉 ( 〈expr〉 )
static PTR(Expr)parse_multicand(std::istream &in) {
    PTR(Expr) expr = parse_inner(in);
    char c = peek_next(in);
    while (c == '(') {
        in.get();
        PTR(Expr) actual_arg = parse_expr(in);
        expr = NEW(CallExpr)(expr, actual_arg);
        c = peek_next(in);
        if(c == ')'){
            in.get();
        }
        c = peek_next(in);
        
    }
    return expr;
}

//〈inner〉 = 〈number〉
//      | ( 〈expr〉 )
//      | 〈variable〉
//      | _let 〈variable〉 = 〈expr〉 _in 〈expr〉
//      | _true
//      | _false
//      | _if 〈expr〉 _then 〈expr〉 _else 〈expr〉
//      | _fun ( 〈variable〉 ) 〈expr〉


// takes an input stream
// returns a Expr of a parse_expr if the stream starts with '('
// returns Expr Number if stream starts with digit
// returns alpha string if the stream starts with alpha char
static PTR(Expr) parse_inner(std::istream &in) {
    PTR(Expr)  num = NULL;
    char c = peek_next(in);
    
    if (c == '(') {
        c = in.get();
        num = parse_comparg(in);
        c = peek_next(in);
        if(c ==')'){
            c = in.get();
        }else{
            perror("expected ')'\n");
            exit(2);
        }
    } else if (isdigit(c) || c == '-') {
        num = calc_number(in);
    } else if(is_Alpha_Char(c)){
        num = NEW(VariableExpr)(valid_Variable(in));
    } else if(c == '_'){

        std::string key = keyWord(in);
        if(key == "_let"){
            num = parse_letExpr(in);
        }else if( key == "_true"){
            num = parse_true(in);
        }else if( key == "_false"){
            num = parse_false(in);
        }else if( key == "_if"){
            num = interp_if(in);
        }else if(key == "_fun"){
            num = parse_fun(in);
        }
        else{
            perror("expected 'let' \n" );
            exit(2);
        }
    }else{
        perror("expected a digit, string, or open parenthesis");
        exit(2);
    }
    return num;
}

//FunExpr::FunExpr(std::string formal_arg, Expr *body)
static PTR(Expr) parse_fun(std::istream &in){
    char c = peek_next(in);
    std::string arg = "";
    if(c == '('){
        in.get();
        arg = valid_Variable(in);
        c = peek_next(in);
        if(c == ')'){
            in.get();
        }else{
            perror("expected a ) ");
            exit(2);
        }
    }
   
    PTR(Expr) body = parse_expr(in);
    return NEW(FunExpr)( arg, body);
}

static PTR(Expr) parse_equals_equals_expr(PTR(Expr) lhs, PTR(Expr) rhs){
    return NEW(EqualsEqualsExpr)(lhs,rhs);
}

static PTR(Expr) parse_true(std::istream &in){
    return NEW(BoolExpr)(true);
}

static PTR(Expr) parse_false(std::istream &in){
    return NEW(BoolExpr)(false);
}

//key word to find _let
static std::string keyWord(std::istream &in){
    std::string s;
    
    char c = in.get();
    while(c != ' ' && c != '\n'){
        s += c;
        c = in.get();
    }
    return s;
}

static PTR(Expr) interp_if(std::istream &in){
    
    PTR(Expr) test_expr = parse_expr(in);
    PTR(Expr) then_expr = nullptr;
    PTR(Expr) else_expr = nullptr;
    
    char c = peek_next(in);
    
    if(c == '_'){
        std::string key = keyWord(in);
        if(key == "_then"){
            then_expr = parse_expr(in);
            c = peek_next(in);
            if(c == '_'){
                key = keyWord(in);
                if(key == "_else"){
                    else_expr = parse_expr(in);
                }
            }
        }
    }
    return NEW(IfExpr)(test_expr, then_expr, else_expr);
}


static PTR(Expr) parse_letExpr(std::istream &in){
    
    //variable .. letExpr var
    std::string let_var = valid_Variable(in);
    PTR(Expr) let_rhs = nullptr;
    PTR(Expr) let_body = nullptr;
    
    char c = peek_next(in);
    if( c == '='){
        in.get();

        let_rhs = parse_expr(in);
        c = peek_next(in);
        if(c == '_'){
            std::string key = keyWord(in);
            if(key == "_in"){
                let_body = parse_expr(in);
            }
        }
    }
    return NEW(LetExpr)(let_var, let_rhs, let_body);
}


// Takes an input stream and
// parses the stream
// returns and Number Expr
static PTR(Expr) calc_number(std::istream &in) {
    int i = 0;
    in >> i;
    return NEW(NumberExpr)(i);
}

// This function peeks the next character.
// Returns the next char in the input stream
static char peek_next(std::istream &in) {
    while(isspace(in.peek())){
        in.get();
    }
    return in.peek();
}

// Takes a char
// returns true if char is an alpha
static bool is_Alpha_Char(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') );
}

//This function returns a string of a variable expression
static std::string valid_Variable(std::istream &in){
    std::string s;
    while((in.peek() >= 'a' && in.peek() <= 'z') || (in.peek() >= 'A' && in.peek() <= 'Z')){
        s += in.get();
    }
    return s;
}


/* for tests */
static PTR(Expr)  calc_str(std::string s) {
    std::istringstream in(s);
    return parse(in);
}

///* for tests */
//static std::string calc_str_error(std::string s) {
//    std::istringstream in(s);
//    std::ostringstream err(std::ios_base::out);
//    try{
//        (void)parse(in);
//        return "";
//    }catch(std::runtime_error exn){
//        return exn.what();
//    }
//}

TEST_CASE( "calc" ) {

    CHECK( calc_str("10+1")->equals( NEW (AddExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(1)) ));
    CHECK( calc_str("(10)+1")->equals( NEW (AddExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(1)) ));
    CHECK( calc_str("10+(1)")->equals( NEW (AddExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(1))) );
    CHECK( calc_str("1+2*3")->equals( NEW (AddExpr)( NEW (NumberExpr)(1), NEW (MultExpr)(NEW (NumberExpr)(2), NEW (NumberExpr)(3)))) );
    CHECK( calc_str("3*(2+width)")->equals(NEW (MultExpr)(NEW (NumberExpr)(3),
                                                          NEW (AddExpr)(NEW (NumberExpr)(2), NEW (VariableExpr)("width"))))  );
    CHECK( calc_str("x")->equals(NEW (VariableExpr)("x")) );
    CHECK( calc_str(" 10 ") ->equals( NEW (NumberExpr)(10)) );
    CHECK( calc_str(" 10  + 1")->equals( NEW (AddExpr)(NEW (NumberExpr)(10), NEW (NumberExpr)(1))) );

}

TEST_CASE( "if" ) {
    CHECK( calc_str(" _if _true _then 10 _else 5")->interp(NEW(EmptyEnv)())->equals( NEW (NumVal)(10)) );
    CHECK( calc_str(" _if _false _then 10 _else 5")->interp(NEW(EmptyEnv)())->equals( NEW (NumVal)(5)) );
}

TEST_CASE( "expr == " ) {
    CHECK( calc_str(" 5 == 5")->equals( NEW (EqualsEqualsExpr)(NEW (NumberExpr)(5), NEW (NumberExpr)(5))) );
    CHECK( calc_str(" 5 == 6")->equals(NEW (EqualsEqualsExpr)(NEW (NumberExpr)(5), NEW (NumberExpr)(6))) );
    CHECK( calc_str(" 1 + 1 == 2")->equals(NEW (EqualsEqualsExpr)(NEW (AddExpr)(NEW (NumberExpr)(1), NEW (NumberExpr)(1)), NEW (NumberExpr)(2))) );
}

TEST_CASE( "_let _in"){
    CHECK(  (calc_str("_let x = 5 _in x + x"))->equals( NEW (LetExpr)("x", NEW (NumberExpr)(5), NEW (AddExpr)(NEW (VariableExpr)("x"), NEW (VariableExpr)("x")))) );
    CHECK(  (calc_str("_let x = 5 _in x + x"))->optimize()->equals(NEW (NumberExpr)(10)) );
}

TEST_CASE( "to string with no variable" ){
    CHECK( calc_str("10+1")->to_string() == "(10 + 1)" );
    CHECK( calc_str("(10)+1")->to_string() == "(10 + 1)" );
    CHECK( calc_str("10+(1)")->to_string() == "(10 + 1)" );
    CHECK( calc_str("1+2*3")->to_string() == "(1 + (2 * 3))" );
}

TEST_CASE( "value with no variables" ){
    CHECK( calc_str("10+1")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(11)) );
    CHECK( calc_str("(10)+1")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(11)) );
    CHECK( calc_str("10+(1)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(11)) );
    CHECK( calc_str("1+2*3")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(7)) );
}

TEST_CASE( "to string with  variable" ){
    CHECK( calc_str("3*(2+width)")->to_string() == "(3 * (2 + width))" );
    CHECK( calc_str("(10)+x")->to_string() == "(10 + x)" );
    CHECK( calc_str("(10+1) * x")->to_string() == "((10 + 1) * x)" );
}

TEST_CASE( "_let simplify"){
    CHECK( calc_str("_let x = 2 _in _let y = 1_in y + x")->optimize()->equals(NEW (NumberExpr)(3))   );
    CHECK( calc_str("_let x = 5 _in _let y = z + 2 _in x + y + (2 * 3)")->optimize()->equals(  calc_str("_let y = z + 2 _in 5 + y + 6") )   );
    CHECK( calc_str("_let x = 5 _in _let y = z + 2 _in x + y + (2 * 3)")->optimize()
            ->equals(  calc_str("_let y = z + 2 _in 5 + y + 6")->optimize()) );
}

TEST_CASE( "Fib"){
    CHECK( calc_str("_let fib = _fun (fib) _fun (x) _if x == 0 _then 1 _else _if x == 2 + -1 _then 1 _else fib(fib)(x + -1) + fib(fib)(x + -2) _in fib(fib)(10)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(89))   );
}

TEST_CASE( "Factorial"){
    CHECK( calc_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(5)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(120))   );
}

TEST_CASE( "Factorial interp_by_steps"){
     
    CHECK( Step::interp_by_steps(calc_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(5)"))->equals(NEW (NumVal)(120))   );
}

TEST_CASE( "fun"){

    CHECK( calc_str("_let f = _fun (x) x + 1 _in f(10)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(11))   );
    CHECK( calc_str("(_fun (x) x+2)(1)")->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(3)) );
    CHECK( calc_str("_let f = (_fun (x) (_fun (y) x*x + y*y)) _in f(2)(3 + 2)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(29)) );
    CHECK( calc_str( "_let add = _fun (x) _fun (y) x + y _in _let addFive = add(5) _in addFive(10)")->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(15)) );



    CHECK( calc_str("_let f = (_fun (x) (_fun (y) x*x + y*y)) _in 2 * f(2)(3)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(26)) );


    CHECK( calc_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1)  _in factrl(factrl)(5)")
          ->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(120)));
    CHECK( calc_str("_let f = (_fun (x) (_fun (y) x*x + y*y)) _in f(2)(3)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(13)) );

    CHECK( calc_str("  _let fib = _fun (fib) _fun (x) _if x == 0 _then 1 _else _if x == 2 + -1 _then 1 _else fib(fib)(x + -1) + fib(fib)(x + -2)_in fib(fib)(10)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(89))   );


}

//Step::interp_by_steps(e5)

TEST_CASE( "fun interp_by_steps"){
    
    
//    Step::interp_by_steps(calc_str("_let add = _fun (x) _fun (y) x + y _in _let addFive = add(5) _in addFive(10)"));
    
    CHECK( Step::interp_by_steps(calc_str("_let f = _fun (x) x + 1 _in f(10)"))->equals(NEW (NumVal)(11))   );
    CHECK( Step::interp_by_steps(calc_str("(_fun (x) x+2)(1)"))->equals(NEW(NumVal)(3)) );
    CHECK( Step::interp_by_steps(calc_str("_let f = (_fun (x) (_fun (y) x*x + y*y)) _in f(2)(3 + 2)"))->equals(NEW (NumVal)(29)) );
    CHECK( Step::interp_by_steps(calc_str("_let add = _fun (x) _fun (y) x + y _in _let addFive = add(5) _in addFive(10)"))->equals(NEW(NumVal)(15)) );

//    CHECK( calc_str("_let f = (_fun (x) (_fun (y) x*x + y*y)) _in 2 * f(2)(3)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(26)) );
//
//
//    CHECK( calc_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1)  _in factrl(factrl)(5)")
//          ->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(120)));
//    CHECK( calc_str("_let f = (_fun (x) (_fun (y) x*x + y*y)) _in f(2)(3)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(13)) );
//
//    CHECK( calc_str("  _let fib = _fun (fib) _fun (x) _if x == 0 _then 1 _else _if x == 2 + -1 _then 1 _else fib(fib)(x + -1) + fib(fib)(x + -2)_in fib(fib)(10)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(89))   );
    
    
}

TEST_CASE( "callExpr call "){
    
    CHECK( calc_str("(_fun (x) x+2)(1)")->interp(NEW(EmptyEnv)())->equals(NEW (NumVal)(3))   );
}


TEST_CASE( "callExpr call interp_by_steps"){
    CHECK( Step::interp_by_steps(calc_str("(_fun (x) x+2)(1)"))->equals(NEW (NumVal)(3))   );
}


TEST_CASE( "Countdown interp_by_steps"){
    CHECK( Step::interp_by_steps(calc_str("_let countdown = _fun (countdown) _fun (n) _if n == 0 _then 0 _else countdown(countdown)(n + -1) _in countdown(countdown)(1000000)"))->equals(NEW (NumVal)(0))   );
}

TEST_CASE( "Count interp_by_steps"){
    CHECK( Step::interp_by_steps(calc_str("_let count = _fun (count) _fun (n) _if n == 0 _then 0 _else 1 + count(count)(n + -1) _in count(count)(100000)"))->equals(NEW (NumVal)(100000))   );
}




