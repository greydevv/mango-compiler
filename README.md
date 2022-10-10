# 🥭 Mango Language

📝 Mango is still under development

### Introduction
Let's address the elephant in the room: why *yet another programming language*?
Well, I don't really know, I just wanted to make a compiler. The language itself is just a byproduct of it.

This repository houses the compiler for Mango, a statically-typed, general-purpose programming language inspired by [Python](https://www.python.org/), [Swift](https://developer.apple.com/swift/), [C](https://en.wikipedia.org/wiki/C_%28programming_language%29), [C++](https://en.wikipedia.org/wiki/C%2B%2B), and [Rust](https://www.rust-lang.org/).

### Mango Language

##### About
Mango is a compiled, statically-typed, general purpose programming language created as a byproduct of my passion project, the Mango compiler.

In creating Mango, I took inspiration from the syntax of Swift, C/C++, and Python type-hinting. The following example calculates the factorial of a 32-bit integer.
```c
func factorial(i32 n) -> i32 {
  if (n == 0) {
    return 1;
  }
  return n * factorial(n-1);
}
```

##### Typing
As mentioned above, Mango is both statically and strongly typed which is mainly inspired by the type system Rust implements.
```c
i8 x = 10;
i16 y = 356;

x + y;
```
Compiling this bit of code yields a `TypeError` because `i8` and `i16` are not the same type and therefore not compatible in a binary expression.
```text
samples/types.mg -> [4,3]
  |
4 | x + y;
  | ^~~~~  
TypeError: i8 and i16 are not compatible in binary expression
```

### Directory
| Stage                | Module                                                                |
| :--                  | :--                                                                   |
| Lexical analysis     | [src/Lexer.cpp](src/Lexer.cpp)                                        |
| Syntax analysis      | [src/Parser.cpp](src/Parser.cpp)                                      |
| Semantic analysis    | [src/visitors/ASTValidator.cpp](src/visitors/ASTValidator.cpp)        |
| IR generation        | [src/visitors/ASTCodegenner.cpp](src/visitors/ASTCodegenner.cpp)      |

### Development Progress
- [x] Front-end
	- [x] Lexer
	- [x] Parser -> AST
- [ ] AST Validation
	- [x] type-checking
	- [x] symbol/reference table
	- [ ] integer types (i8, i16, i32, i64, i128, u8, u16, u32, u64, u128)
	- [ ] function overloading
	- [ ] strings/arrays
	- [ ] pointers (?)
- [ ] IR
	- [x] binary expressions (arithmetic, boolean, etc.)
	- [x] conditional control flow (if/else if/else)
	- [x] control flow (while loop)
	- [x] unary expressions
