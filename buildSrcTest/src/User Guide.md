When using the command line the parser follows the following grammar. The command line will have three run modes. When ./MSDScript is run with no flags then it will run in inerp mode. This will create an expression from the input, and interpet that expression to a value. The second mode is opt mode which can be run by adding the "-opt" flag. This will optimize the expression to a simplified expression. The final mode can be run by adding the -step flag. This will interpret the expression using continuations. 


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


Syntax and Expressions:

In the terminal, the parser will take the input and build an expression if the syntax is correct. If there is an error in the syntax the program will exit with an error message. Below is the valid expressions and the syntax the parser will expect. Once the input is given, to run the interpreter type control d. 

NumberExpr - msdcript will take any 4 byte integer

    Example1: 

        100   

    Example2:

        -5
    
BoolExpr - true and false

    Example1: 
    
        _true
    
    Example2:
    
        _false
    
    
    
VariableExpr  - variables in msdscript are any alpah characters. Any char from a - Z is acceptable.

    Example1:
    
        x
    
    Example2:
    
        variable
    
    Bad Examples:
        
        !var
        
        bad_example
        
        anotherBadExample3
    
AddExpr - this is the result of adding two expressions. 
    
    Example:
    
    1 + 2
    
    or
    
    3 + -1
    
    or 
    
    3 + x
    
    note: if x is a free variable it will throw an error. To bind a variable such as x you will need a letExpr or funExpr, which are described below.


MultExpr - This expression represents the multiplication of two expressions. 


    Example:

    1 * 2

    or

    3 * -1

    or 

    3 * x

    note: if x is a free variable it will throw an error. To bind a variable such as x you will need a letExpr or funExpr, which are described below.    
    
    
LetExpr - A let expression allows us to bind variables to an expression let expressions follow this syntax _let 〈VariableExpr〉 = 〈expr〉 _in 〈expr〉where expr can be any expression in this section

    Example: 
    
    _let x = 5 _in x + x
    
    
    the x's in the _in will be bound to 5 this will result in 10 when interpreted. The variable will only be bound to the _let it follows.
    
    _let x = 5 _in x + x + _let x = 2 _in x 
    
    the second _in will be bound by the second let. This means when this LetExpr gets interpreted the value will be 12.
    
    
IfExpr - allows the user to create conditional expressions by following the syntax 
_if 〈expr〉 _then 〈expr〉 _else 〈expr〉

    Example:
    
    _if 3 == 3 _then 1 _else 2
    
    
    This will compare 3 to 3, if that is true 1 this expression will be interpreted to 1 else 2.
    
    
EqualsEqualsExpr - This expression is used to compare and expression agains another expression and follows the syntax 〈comparg〉 == 〈expr〉 where a comparge is equal to an expr

    Example:
    
    3 == 3
    
    
    x == 3
    
    3 == 1 + 2  which will be interpreted as 3 == (1+2)
    

FunExpr - represents a function in MSDScript and follows the syntax 
_fun ( 〈variable〉 ) 〈expr〉

    Example:
    
    _fun (x) x + 1
    
    To call an expression you can bound the x with a letExpr
    
    
CallExpr - used to call a function. The syntax for a call expression is 〈expr〉 ( 〈expr〉 ) 

    Example:
    
    f(10)

    One way to call a function:
    
    _let f = _fun (x) x + 1 _in f(10)

    The call expression is f(10). f will be bound to the f following the _let. When this gets interpreted the f in the CallExpr with be replaced with x + 1 and will be called with the 10 and the output will be 11.



All of these expressions can be used to create more complex expressions. 

    Example:
    
    _let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(5)
    
    This is an example of writing a recursive factorial function and passing it 5.

    
    _let fib = _fun (fib) _fun (x) _if x == 0 _then 1 _else _if x == 2 + -1 _then 1 _else fib(fib)(x + -1) + fib(fib)(x + -2) _in fib(fib)(10)
    
    This is an example of a recursive fibinacci function to find the 10th fib number.
