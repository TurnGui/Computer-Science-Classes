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

## Project Structure


