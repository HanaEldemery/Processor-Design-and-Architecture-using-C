# Computer Architecture
I completed a project simulating a fictional processor design and architecture using C. This project, based on the Von Neumann architecture, required a thorough 
understanding of various processor components and their interactions. I implemented a processor design and architecture which featured a memory size of 2048x32 bits
and a register set comprising 31 general-purpose registers, a zero register, and a program counter. I implemented a 32-bit instruction set architecture with 12 unique
instructions (add, subtract, multiply, move immediate, jump if equal, and, exclusive or immediate, jump, logical shift left, logical shift right, move to register and
move to memory), encompassing stages of Instruction Fetch (IF), Instruction Decode (ID), Execute (EX), Memory (MEM), and Write Back (WB). The processor's pipeline
allowed for up to 4 instructions running in parallel, adhering to specific clock cycle patterns for efficient execution. I developed a text file to write programs
in assembly language, particularly MIPS, read and parsed these instructions, and stored them in the memory. The execution of the pipelined implementation began by 
fetching the first instruction from memory at Clock Cycle 1, with subsequent cycles handling different stages in parallel, handling both control and data hazards. 
Throughout the simulation, the program printed crucial details such as the clock cycle number, pipeline stages, input parameters, and updates to registers and memory. 
This project enhanced my proficiency in low-level programming, optimized code debugging, and practical application of theoretical computer architecture concepts,
demonstrating my dedication to expanding my software development skills.
