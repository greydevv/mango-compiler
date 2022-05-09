# Mango Language

### Warning
The compiler is still under development. Not everything listed in the [Language Reference](#language-reference) is completely implemented.

### Contents

- [Introduction](#introduction)
- [Why Another Programming Language](#why-another-programming-language)
- [Language Reference](#language-reference)
- [About the Compiler](#about-the-compiler)
- [Development Progress](#development-progress)

### Introduction
This repository houses the compiler for Mango, a statically-typed general-purpose programming language.

Mango is a programming language inspired by the likes of
[Python](https://www.python.org/), [Swift](https://developer.apple.com/swift/),
[C](https://en.wikipedia.org/wiki/C_%28programming_language%29), and
[C++](https://en.wikipedia.org/wiki/C%2B%2B). The goal of this project is to
create a compiler with Mango as the proof of concept.    

### Why Another Programming Language?

Let's address the elephant in the room: why *yet another programming language*?
Well, I don't really know, I just wanted to make a compiler. The language itself is just a byproduct of it.

### Language Reference

#### Basic Syntax

Compound statements are composed within curly braces, `{` and `}`. Like many
languages, statements must end with a semicolon, `;`.

#### Types

| Type   | Size (bits) | Description                                              |
| :--    | :--:        | :--                                                      |
| `void` | 0           | Used to denote that a function does not return anything. |
| `int`  | 32          | 32-bit signed integer.                                   |
| `bool` | 1           | Can be either 1 (true) or 0 (false).                     |

#### Expressions

This language handles expressions no different than C++ and other similar
languages. Parenthesis can be used for correct evaluation order, like so:
```
int a = 3 + 4 * 10;   // 4 * 10 evaluated first (* has higher precedence than +)
int b = (3 + 4) * 10; // 3 + 4 evaluated first
```
This is all trivial and should be very familiar.

The operator precedence was inspired by the [C++ Operator Precedence
table](https://en.cppreference.com/w/cpp/language/operator_precedence). In
descending order, the operator precedence is as follows:

| Operator             | Description                    | Associativity  |
| :--                  | :--                            | :--            |
| `!`                  | Logical not (boolean)          | Right-to-left  |
| `*`, `/`             | Multiplication, division       | Left-to-right  |
| `+`, `-`             | Addition, subtraction          | Left-to-right  |
| `<`, `>`, `>=`, `<=` | Relational operators (boolean) | Left-to-right  |
| `==`, `!=`           | Equality operators (boolean)   | Left-to-right  |
| `&&`                 | Logical and (boolean)          | Left-to-right  |
| `\|\|`               | Logical or (boolean)           | Left-to-right  |
| `=`                  | Assignment                     | Right-to-left  |

#### Variables

Similar to most statically-typed languages:
```
// define 
int a = 10;
int b = 3;

// reference
a = a + b;
```

#### Control Flow
If/else-if/else statements are similar to most languages where multiple else-if statements can be chained together and else-if/else statements are optional.
```
if ( [expression] ) { [body...] }
else if ( [expression] ) { [body...] } 
else { [body...] }
```

##### WARNING: FOR-LOOPS ARE NOT CURRENTLY IMPLEMENTED; THEY ARE EXPERIMENTAL.
Inspiration for for-loops is a combination of Python and Swift.
```
for (int i in 1..3) { [body...] }
```

#### Functions
Functions in the Swift programming language look very similar to Python
type-hinting which both inspired Mango's function prototypes.
```
func [id]([params...]) -> [return type]
{
    [body...]
}
```
The parameters are a comma-separated list where each parameter is specified as
`[type] [variable name]`. If the return type is omitted, the compiler will
assume the function returns `void`.

#### Includes
Include statements are denoted by the `include` keyword followed by the path to
the module which is supplied as a string.
```
include "path/to/module.mg";
```
At the moment, include paths are only resolved as relative imports.

#### Miscellaneous
Comments begin with a double forward slash, `//`.

### About the Compiler

This compiler employs the classic lexer-parser front end where tokens are read
into the parser from the lexer. The parser then builds an [Abstract-Syntax-Tree
(AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree), while performing
syntax analysis. The parser works by 'expecting' certain tokens depending on
what the current/previous token(s) were. 

For example, when the compiler encounters a `TOK_KWD` (keyword token), it needs
to check what that keyword is. If the value of the keyword is `func`, then the
parser knows that the user is attempting to define a function. In the Mango
programming language, the function name comes after the `func` keyword. So, the
parser will then ask the lexer for the next token. If the token is `TOK_ID`,
then all is well; continue. If it is not `TOK_ID`, then the program exits with
a SyntaxError.

In the back end of the compiler, the code generation module takes control of the
AST that the parser creates. This module's job is to convert the AST into an
intermediate form between source code (human-readable) and machine code
(definitely not human-readable), known as Intermediate Representation (IR). For
this, [LLVM-IR](https://llvm.org/docs/LangRef.html) is used as it is a very
robust, flexible, and scalable IR generation API.

#### Compilation Steps (directory)
| Stage                | Module                                                                |
| :--                  | :--                                                                   |
| Lexical analysis     | [src/Lexer.cpp](src/Lexer.cpp)                                        |
| Syntax analysis      | [src/Parser.cpp](src/Parser.cpp)                                      |
| Semantic analysis    | [src/visitors/ASTValidator.cpp](src/visitors/ASTValidator.cpp)        |
| IR generation        | [src/visitors/ASTCodegenner.cpp](src/visitors/ASTCodegenner.cpp)      |

The process of the compiler is illustrated below:

First, the compiler needs some source code:
![language](https://user-images.githubusercontent.com/58391520/157451845-a0c21836-bf69-483f-8d8c-fa6fdd49ba5a.png)

Then, the AST is constructed from the source code in the front end:
![AST](https://user-images.githubusercontent.com/58391520/157453597-a025783d-a71e-49f7-868b-644907fb738f.png)

After the AST is constructed, it is fed to the code-generation module, which
emits the IR:
![LLVM-IR](https://user-images.githubusercontent.com/58391520/157454073-f8acd217-76a9-4789-99d2-eb002e03a6bf.png)

### Development Progress

- [x] Front end: Lexer->Parser->AST
- [x] IR: expressions (arithmetic, boolean, etc.)
- [x] IR: functions
- [x] IR: if/else if/else
- [x] IR: while loops
- [ ] IR: for loops (need arrays first)
- [ ] Static (and weak) type-checking
