Pour ce niveau on va devoir trouver le bon chiffre pour qu'a la fin notre comparaison ecrit "Congratulation!".

# Deroulement du code
On va prendre le code a l'inverse pour trouver la valeur de ctx

## Etape 1

A la fin il faut qu'on arrive a data = "Congratulation!".

```bash
if (strcmp((char *)data, "Congratulations!") == 0)
```

sachant que pour l'instant data = "Q}|u`sfg~sf{}|a3"

## Etape 2

Notre data passe dans un XOR

```bash
for (size_t i = 0; i < len; i++) {
        data[i] ^= (unsigned char)(uintptr_t)ctx;
    }
```
Ducoup on peut dire:

```
end = "Congratulations!"

data[i] ^ ctx = end[i]
ctx = data[i] ^ end[i]
```

0x51 ^ 0x43 = 0x12
0x7d ^ 0x6f = 0x12
...

ctx = 0x12 = 18

## Etape 3

On a cette ligne 
```bash
void *ctx = (void *)(param_2 - param_1);
```
sachant que param2 c'est notre input et param1 = 322424845

ctx = 322424845 - input
18 = 322424845 - input
input = 322424845 - 18
input = 322424827

