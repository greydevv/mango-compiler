# C++ Compiler

Compiler for a custom programming language.

### Warning
This compiler is still under development. Not everything listed in the [Language Reference](https://github.com/greysonDEV/pseudo-rng#introduction) section is completely implemented.

### Language

![viper](https://user-images.githubusercontent.com/58391520/157451845-a0c21836-bf69-483f-8d8c-fa6fdd49ba5a.png)

This language uses the classic lexer-parser-AST structure. Source code is parsed into the [Abstract-Syntax-Tree (AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree), whose output of the above snippet is as follows:
![AST](https://user-images.githubusercontent.com/58391520/157453597-a025783d-a71e-49f7-868b-644907fb738f.png)

After the source code is structured into the AST, the AST is fed to the code-generation module of the compiler. This module receives the AST and compiles it down into the intermediate representation, in this case the intermediate repsentation is [LLVM-IR](https://llvm.org/docs/LangRef.html). For the above snippet, the LLVM-IR looks like:
![LLVM-IR](https://user-images.githubusercontent.com/58391520/157454073-f8acd217-76a9-4789-99d2-eb002e03a6bf.png)

### Language Reference

#### Basic Syntax

Compound statements are composed within curly braces, `{` and `}`. Like many languages, standalone statements and expressions must end in a semicolon, `;`.

#### Types

| Type   | Size (bits) | Description                                                               |
| :--    | :--:        | :--                                                                       |
| `void` | 0           | Used to denote that a function does not return anything.                  | 
| `int`  | 32          | 32-bit signed integer.                                                    |
| `bool` | 1           | Can be either 1 (true) or 0 (false).                                      |

#### Expressions

This language handles expressions no different than C++ and other similar languages. Parenthesis can be used for correct evaluation order, like so:
```
int a = 3 + 4 * 10; // 4 * 10 evaluated first (* has higher precedence than +)
int a = (3 + 4) * 10; // 3 + 4 evaluated first
```
This is all trivial and should be very familiar.

The operator precedence was stolen straight from the [C++ precedence table](https://en.cppreference.com/w/cpp/language/operator_precedence). In descending order, the operator precedence is as follows:

| Operator             | Description                    | Associativity
| :--                  | :--                            | :--
| `!`                  | Logical not (boolean)          | Left-to-right
| `*`, `/`             | Multiplication, division       | Left-to-right
| `+`, `-`             | Addition, subtraction          | Left-to-right
| `<`, `>`, `>=`, `<=` | Relational operators (boolean) | Left-to-right
| `==`, `!=`           | Equality operators (boolean)   | Left-to-right
| `&&`                 | Logical and (boolean)          | Left-to-right
| `||`                 | Logical or (boolean)           | Left-to-right
| `=`                  | Assignment                     | Right-to-left


#### Functions

Functions can be defined in the following ways. Parameters are optional, as always.
```
func [id]([params...]) -> [return type]
{
    [body...]
}
```
```
func [id]([params...])
{
    [body...]
}
```
If the return type is omitted, the compiler will assume the function returns `void`.

#### Miscellaneous

Comments begin with a double forward slash, `//`.
