# PREPROCESSOR

# C Preprocessor in C

## Project Overview

This project implements a basic C preprocessor using the C programming language. The program takes a C source file as input and performs three main operations: comment removal, header file inclusion, and macro substitution. The processed source code is then written into an output file with the `.i` extension.

The project is divided into multiple C source files, with each file responsible for a specific preprocessing operation. The main file controls the execution flow and calls the required functions from the other source files. A Makefile is used to compile the individual source files, generate object files, and link them together to create the final executable named `mypreprocessor`.

## Source Code Files

### 1. pre.c

The `pre.c` file is the main file of the project. It controls the overall execution of the preprocessor and connects the different preprocessing operations.

When the program starts, it accepts the input filename through command-line arguments. It opens the specified file and reads its contents into a character array. If the input file cannot be opened, the program displays an error message.

After reading the source code, the main function calls the comment removal function defined in `comment_removal.c`. Once comments have been removed, it calls the header inclusion function defined in `header_inclusion.c`. It then calls the macro substitution function defined in `macro_sub.c`.

After all three operations are completed, the modified source code is written into an output file. The output file contains the source code after the preprocessing operations implemented in this project have been performed.

### 2. comment_removal.c

The `comment_removal.c` file is responsible for removing comments from the input C source code. Comments are used to explain the program and are ignored by the C compiler during normal compilation.

This file handles two types of comments: single-line comments and multi-line comments. Single-line comments begin with `//` and continue until the end of the line. Multi-line comments begin with `/*` and end with `*/`, and they can extend across multiple lines.

The function defined in this file scans the source code to identify comment markers. When a comment is found, the corresponding characters are removed from the source text. The remaining code is preserved for the subsequent preprocessing stages.

This operation reduces the input source code by removing comments before header inclusion and macro substitution are performed.

### 3. header_inclusion.c

The `header_inclusion.c` file handles supported header inclusion directives in the input source code. In C, the `#include` directive is commonly used to include the contents of another file in the current source file.

The function defined in this file searches for header inclusion directives written using angle brackets. When a supported directive is identified, the program extracts the filename between `<` and `>`. It then attempts to open the specified file and read its contents.

After reading the file, the program replaces the corresponding include directive in the source text with the contents obtained from that file. This allows the preprocessing program to insert external file contents into the source code.

The implementation supports the header inclusion format handled by the project. It does not automatically reproduce all the header search paths or other advanced header-processing features of a standard C preprocessor.

### 4. macro_sub.c

The `macro_sub.c` file is responsible for processing macro definitions and performing macro substitution. Macros are commonly defined using the `#define` directive in C programs.

This file handles object-like macros and function-like macros. An object-like macro associates a macro name with replacement text. A function-like macro accepts arguments and uses them to generate replacement text.

The macro substitution function identifies macro definitions and stores the required information, including macro names, replacement text, and function-like macro parameters where applicable. It processes the source code to identify matching macro names and replaces them with their corresponding replacement text.

For function-like macros, the implementation reads the arguments supplied in the source code and uses them during substitution. It also performs repeated substitution passes to handle certain cases in which one replacement introduces another macro name.

The processed macro definition lines are removed as part of the implementation. The resulting source text is returned to the main program for output generation.

The implementation covers the macro features supported by this project and is not intended to provide every feature of the standard C preprocessor.

## Makefile

The Makefile is used to automate the compilation of the project. Instead of manually compiling every C source file and linking the resulting object files, the required commands can be organized into separate targets.

Each source file is compiled into an object file. The object files are then linked together to generate the executable named `mypreprocessor`. The Makefile also includes a `clean` target that removes the generated object files and executable.

### Creating the Makefile

Create a file named `Makefile` in the same directory as the C source files. Make sure the filename is exactly `Makefile`, with a capital `M`.

Makefile code:

    mypreprocessor: pre.o comment_removal.o header_inclusion.o macro_sub.o
    	gcc pre.o comment_removal.o header_inclusion.o macro_sub.o -o mypreprocessor

    pre.o: pre.c
    	gcc -c pre.c -o pre.o

    comment_removal.o: comment_removal.c
    	gcc -c comment_removal.c -o comment_removal.o

    header_inclusion.o: header_inclusion.c
    	gcc -c header_inclusion.c -o header_inclusion.o

    macro_sub.o: macro_sub.c
    	gcc -c macro_sub.c -o macro_sub.o

    clean:
    	rm -f pre.o comment_removal.o header_inclusion.o macro_sub.o mypreprocessor

**Important:** Spaces cannot be used as a replacement for the required TAB indentation in a traditional Makefile.

### Understanding the Makefile

The first target is `mypreprocessor`. It depends on four object files: `pre.o`, `comment_removal.o`, `header_inclusion.o`, and `macro_sub.o`. These object files must be available before the final executable can be generated.

The command `gcc pre.o comment_removal.o header_inclusion.o macro_sub.o -o mypreprocessor` links the object files together and creates the executable. The `-o mypreprocessor` option specifies the name of the generated executable.

The `pre.o` target depends on `pre.c`. Its compilation command, `gcc -c pre.c -o pre.o`, compiles the main source file into an object file. The `-c` option tells GCC to compile the source file without linking it into an executable.

The other three object file targets follow the same process. Each target depends on its corresponding C source file and generates an object file with the matching name. This separates compilation from the final linking stage.

The `clean` target removes the generated object files and executable using the `rm` command. It is useful when you want to remove the previous build files before compiling the project again.

When `make` is executed, it reads the Makefile, checks the dependencies, compiles the required source files, and links the object files to produce the final executable.

## Compilation and Execution

### Method 1: Compilation Using Make

First, open a terminal and navigate to the directory containing the project files. Ensure that the four C source files and the Makefile are present in that directory.

Run the following command:

    make

The Make utility reads the Makefile and executes the compilation commands in the required order. It generates the object files and links them to create the `mypreprocessor` executable.

Once compilation is successful, run the program using the following command:

    ./mypreprocessor inputfile.c

Replace `inputfile.c` with the actual name of your input C source file. The program reads the file, performs the implemented preprocessing operations, and writes the resulting source code to the output file specified in `pre.c`.

To remove the generated object files and executable, run:

    make clean

You can then run `make` again to rebuild the project.

### Method 2: Compilation Using GCC Directly

The project can also be compiled without using a Makefile. In this method, all the C source files are passed directly to the compiler in a single command.

Run the following command from the project directory:

    cc pre.c comment_removal.c header_inclusion.c macro_sub.c -o mypreprocessor

The compiler processes the source files and links them together to generate the executable named `mypreprocessor`.

After successful compilation, execute the program using:

    ./mypreprocessor inputfile.c

The input filename must match an existing C source file in the current directory, unless a valid path to the file is provided.

**Note:** The filenames used in the Makefile and compilation commands must match the actual filenames in your project directory. The output filename is determined by the implementation in `pre.c`.

## Running the Project

To run the project, provide a valid C source file as a command-line argument. The main program reads the file and passes its contents through the comment removal, header inclusion, and macro substitution functions.

After these operations are completed, the modified source code is written into the output file. You can open that file to inspect the results of the preprocessing operations.

The executable can be reused with different input files by supplying a different filename when running the program, provided that the input files satisfy the requirements of the implementation.
