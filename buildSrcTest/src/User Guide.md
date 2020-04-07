When using the command line the parser follows the following grammar.


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
