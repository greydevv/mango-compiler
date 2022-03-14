# C++ Compiler

Compiler for a custom programming language.

### Warning
This compiler is still under development.

### Language

![viper](https://user-images.githubusercontent.com/58391520/157451845-a0c21836-bf69-483f-8d8c-fa6fdd49ba5a.png)

This language uses the classic lexer-parser-AST structure. Source code is parsed into the [Abstract-Syntax-Tree (AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree), whose output of the above snippet is as follows:
![AST](https://user-images.githubusercontent.com/58391520/157453597-a025783d-a71e-49f7-868b-644907fb738f.png)

After the source code is structured into the AST, the AST is fed to the code-generation module of the compiler. This module receives the AST and compiles it down into the intermediate representation, in this case the intermediate repsentation is [LLVM-IR](https://llvm.org/docs/LangRef.html). For the above snippet, the LLVM-IR looks like:
![LLVM-IR](https://user-images.githubusercontent.com/58391520/157454073-f8acd217-76a9-4789-99d2-eb002e03a6bf.png)

### Language Reference

#### Types

| Type   | Size (bits) | Description                                                               |
| ---    | ---         | ---                                                                       |
| `void` | 0           | `void` should be used to denote that a function does not return anything. | 
| `int`  | 32          | 32-bit signed integer.                                                    |
| `bool` | 1           | Can be either 1 (true) or 0 (false).                                      |

#### Functions

Functions can be defined in the following ways. Parameters are optional, as always.
```
func [id]( [params...] ) -> [return type]
{
    [body...]
}
```
```
func [id]( [params...] )
{
    [body...]
}
```
If the return type is omitted, the compiler will assume the function returns `void`.
