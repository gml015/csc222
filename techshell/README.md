# Techshell 

A C program that imitates simple functionality of a UNIX-based 

***

**Garrett Lee King**\
CSC 222 - Systems Programming

***

## Description

### **tokenizeInput()**
- This function converts user input into tokens that 
are then stored into the array named ```args[]```.
- This function also handles redirection by checking if a token is 
a redirection character *(< or >)*, and then makes sure that
```inputFile``` or ```outputFile``` points to the same character array
as the following token. 

### **executeCommand()**
- This function creates a child process that executes regular commands by passing in 
our arguments created by the ```tokenizeInput()``` function through ```execvp```.
- This function also handles redirection by taking in ```inputFile``` or ```outputFile```
if they exist and then either reading the file contents or creating a new file and passing them to 
```dup2()```, which handles redirecting input and output.

### **changeDirectory()**
- This function handles the ```cd``` command by passing the inputted directory through
the system call ```chdir()```.
- If no specific directory is given, it travels to the home directory if it exists.

### **main()**

- Retrieves the current working directory by using ```getcwd()``` function and then 
formats so each line is ```(PATH)$```.
- User input is taken through ```fgets()``` and then passed through ```tokenizeInput()```
- Commands are handled sort of like Room Adventure; if the user inputs exit, ```arg[0]``` should be "exit" and 
the program then knows to just ```return``` to close the program. 
- If the command is ```cd```, the program handles it
by passing them through the ```changeDirectory()``` function. 
- All other commands are simply passed
through ```executeCommand()``` in order to execute.

## Usage

