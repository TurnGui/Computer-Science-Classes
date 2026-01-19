# Pascal Compiler (PLC Project)

This project was developed for the **Processing of Languages and Compilers** course.  
Its goal is to implement a simple compiler for **Standard Pascal**, using Python and the PLY (Python Lex-Yacc) library.

The compiler analyzes Pascal source code, checks its correctness, and translates it into instructions for a virtual machine.

---

## Project Overview

The compiler follows the classical compilation pipeline:

1. **Lexical Analysis**  
   Converts the Pascal source code into tokens (keywords, identifiers, literals, operators).

2. **Syntactic Analysis**  
   Validates the grammatical structure of the program using a context-free grammar.

3. **Semantic Analysis**  
   Performs basic semantic checks such as variable declaration and type compatibility.

4. **Code Generation**  
   Produces code for a stack-based virtual machine.

---
## How to Run

### Run a single Pascal program

From the project root directory:
The output will be the generated virtual machine code.

---

### Run all test programs

To compile all test programs automatically:
the script programPASCAL.py executes the compiler for every `.pas` file inside the `exemplos_testes` directory.

---

## Notes

- The compiler does not execute Pascal programs; it only generates virtual machine code.
- This project is intended for educational purposes and does not fully implement the entire Pascal language.

