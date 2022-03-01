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
FunctionAST:
  PrototypeAST(add)
    Returns: int
    Parameters:
      VariableAST(a)
      VariableAST(b)
  CompoundAST:
    ReturnAST:
      ExpressionAST(+):
        VariableAST(a)
        VariableAST(b)
```

Emitted LLVM-IR:
```
define double @add(double %a, double %b) {
entry:
  %addtmp = fadd double %a, %b
  ret double %addtmp
}
```
