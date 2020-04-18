MSDScript in the command line parses the given input, and if it follows the grammar, creates an expression. If there is no flag passed to the command line, then the expression is interpreted to produce a numeric value of the expression. If the command line option “-opt” is given, then the expression is simplified. If the command line argument “-step” is given, then the expression is interpreted using continuation
instead of recursive calls to interp. The command line grammer and syntax can be found in the [User Guide] (./md___users_patrickchapple__m_s_d_script_repo__m_s_d_script_build_src_test_src__user__guide.html) under Command line.

Another way to use MSDScript is as a static library. Using the library you can use the classes and subclasses or just use the parse function to create expressions from user input. That information cans also be found in the User Guide under Embed MSDScript. 


