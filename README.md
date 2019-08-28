# CORE WAR - @42born2code

"Core War (or Core Wars) is a Programming game in which two or more battle programs (called warriors) compete for the control of the MARS virtual computer (Memory Array Redcode Simulator). These battle programs are written in an abstract Assembly language called Redcode. The goal of the game is to cause all processes of the opposing program(s) to terminate, leaving your program in sole possession of the machine." (from: Wikipedia)

Corewar is the final project of the inner circle algorithm branch at [42][2].
It is made of two parts, an *assembler* and a *virtual machine*.
The assembler compiles the _redcode_ in a ``.cor`` file, which can be run with the virtual machine.

You can find pre-compiled programs in `test/ressources/ctrl_cor`, and redcode examples in `test/ressources/test_asm`.

[![asciicast](https://asciinema.org/a/QVLSe7pcstSVOrRS1hnSqB28J.png)](https://asciinema.org/a/QVLSe7pcstSVOrRS1hnSqB28J?speed=2&autoplay=1&preload=1&loop=1&theme=monokai)

## The implementation
This Corewar implementation follows the directives of the 42's Corewar. This means the project is a little different from a classical Corewar. For instance, the champions are not charged randomly in the memory, there are more instructions and each processes have their own registers.
For more information, refer to the [project instructions][1].

### GUI
The GUI allows easy debugging with a step by step mode and shows various informations about the processes (registers and so on).
You can use ' _space_ ' to start/pause the VM, ' _s_ ' to go to the next cycle, up/down arrows to adjust 'Cycles by Step', and ' _o/p_ ' to iterate through the processes.
The left/right arrows adjusts the speed of the match.

## The code
The code is very straightforward and should prove easy to maintain.

In the VM, all instructions use the function ``int32_t vm_readarg()`` to get the arguments, so adding an instruction should be easy.
Moving a pointer in the VM is done with ``uint8_t *vm_move()`` which allows the memory to be circular, meaning there cannot be overflows.
All writings are done with ``void vm_write()`` which writes in the memory and notifies the gui at the same time.

The instructions structure is in `src/vm/op.c` and the header is in 'includes/op.h'.

## Build
Release build:
```
$ make
```
Sanitize build (flags: `-fsanitize=address,undefined -ferror-limit=5`):
```
$ make san
```
Developer build (flags: `-g3 -DDEBUG`):
```
$ make dev
```

## Examples
This is an example of a simple corewar program written in simil-asm (redcode):
```
$ cat test/ressources/test_asm/bee_gees.s
.name "stayin' alive"
.comment "Ha, Ha, Ha, stayiiiiin' aliiiiiiiiiive"
	
sti		r1, %:live, %1		;change lives with the right value
sti		r1, %:live2, %1		;
ld		%1, r3
ld		%33, r6

#While (r2 < 10)
forks:
add		r2, r3, r2		;increment r2
xor		r2, %15, r4		;if (r4) {carry = 0}

live2:
live	%4
zjmp	%:endwhile		;if (carry)
fork	%:forks
ld		%0, r4			;carry = 1
zjmp	%:forks

#EndWhile
endwhile:
ld		%0, r4			;carry = 1

live:
live	%4
zjmp	%:live
```

Compile a corewar program:
```
$ ./asm  test/ressources/test_asm/bee_gees.s
Writing output program to test/ressources/test_asm/bee_gees.cor
```

Run a match:
```
$ ./corewar -g brouette.cor test/ressources/test_asm/bee_gees.cor test/ressources/ctrl_cor/helltrain.cor
```

### Usage
```
corewar: Usage: ./corewar [ options ] <[-n <champ.id>] champ.cor> <...>
	-g      : Ncurses GUI
		- space      : pause/resume execution
		- o/p        : navigate through processes (show registers)
		- s          : step by step
		- up/down    : cycles by step
		- left/right : cycles/second
	-d N    : Dumps memory after N execution cycles
	-c N    : CTMO - Cycles till memory opens
	-p N    : Pause a `N` cycle in GUI
	-v N    : Sets verbosity level to N (bitwise)
		- 0 : Show essentials
		- 1 : Show lives
		- 2 : Show cycles
		- 4 : Show operations
		- 8 : Show deaths
		-16 : Show PC movement
```

[1]: https://github.com/jon-finkel/corewar/blob/master/test/ressources/ress_42/project_instructions/corewar.en.pdf
[2]: http://42.fr
