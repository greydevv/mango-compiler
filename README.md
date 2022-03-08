# C++ Compiler

Compiler for a custom programming language.

### Warning
This compiler is still under development.

### Language


#### Examples
```
func add(int a, int b) -> int
{
    return a + b;
}
```

AST of the above snippet:
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

Emitted LLVM-IR:
```
define i32 @add(i32 %a, i32 %b) {
entry:
    %addtmp = add i32 %a, %b
    ret i32 %addtmp
}
```
