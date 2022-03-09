# C++ Compiler

Compiler for a custom programming language.

### Warning
This compiler is still under development.

### Language

![viper](https://user-images.githubusercontent.com/58391520/157448364-4f268f9b-f362-4fa0-9df8-e190c5091588.png)

This language uses the classic lexer-parser-AST structure. Source code is parsed into the [Abstract-Syntax-Tree (AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree), whose output of the above snippet is as follows:
```
ModuleAST:
  FunctionAST:
    PrototypeAST(add)
      Returns: int
      Parameters:
        VariableAST(a, int)
        VariableAST(b, int)
    CompoundAST:
      ReturnAST:
        ExpressionAST(+):
          VariableAST(a)
          VariableAST(b)
```

After the source code is structured into the AST, the AST is fed to the code-generation module of the compiler. This module receives the AST and compiles it down into the intermediate representation, in this case [LLVM-IR](https://llvm.org/docs/LangRef.html). For the above snippet, the LLVM-IR looks like:
```
define i32 @add(i32 %a, i32 %b) {
entry:
    %addtmp = add i32 %a, %b
    ret i32 %addtmp
}
```
