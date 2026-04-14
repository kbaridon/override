En analysant le code, nous constatons que l'objectif est qu'auth() renvoie 0:

```c
    hash = ((int)login[3] ^ 0x1337U) + 0x5eeded;

    for (i = 0; i < (int)len; i++)
    {
        if (login[i] < 32)
            return 1;

        hash += ((int)login[i] ^ hash) % 0x539;
    }

    if (serial == hash)
        return 0;
```

--> Il faut donc appliquer ce hash sur notre login, et mettre ce dernier en tant que serial.

Nous allons donc reccrer la fonction hash dans `get_hash.c` et lui donner un login, cela nous renverra notre hash, et donc, nous pourrons passer auth.

Pour `012345`, le hash est `6231514`.

Donc:

```shell
./level06
012345
6231514
cat /home/users/level07/.pass
```

--> Nous avons notre flag pour le level07 !