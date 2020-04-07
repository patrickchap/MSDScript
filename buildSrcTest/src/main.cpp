#include <iostream>
#include "parser.hpp"
#include <string>
#include <sstream>
#include "Env.hpp"
#include "step.hpp"

int main(int argc, char **argv){
    
  //    std::istringstream in("2147483647+1");
  //   PTR(Expr) d = parse(in);
  //  std::cout << d->interp(NEW(EmptyEnv)())->to_string() << "\n";

    PTR(Expr) d = parse(std::cin);

    if(argc == 2){
        std::string arg = argv[1];
        if(arg == "-opt"){
            std::cout << "Optimized Expression:\n";
            std::cout << d->optimize()->to_string() << std::endl;
        }else if(arg == "-step"){
            std::cout << "Expression Value Step:\n";
            std::cout << Step::interp_by_steps(d)->to_string() << "\n";
        }
    }else{
        std::cout << "Expression Value:\n";
        std::cout << d->interp(NEW(EmptyEnv)())->to_string() << std::endl;
    }
    
    return 0;
}






