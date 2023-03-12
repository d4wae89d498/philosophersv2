# Philosophers 

[Le Dîner des philosophes](https://fr.wikipedia.org/wiki/D%C3%AEner_des_philosophes).

**Notes sur le multi-threading :**

- Un programme va quitter tous ses threads non détachés en quittant
- Lire et écrire un entier CPU est une opération atomique
- Les opérations atomiques n'ont pas besoin d'être protégées 
- Les opérations non-atomiques doivent êtres protégées
- ``volatile`` protège les entiers CPU des optimisations non thread-safe

**Le diner des philosophes modélisé en Réseau de Petri (Rp22 wikipedia / CC BY-SA 3.0) :**

![Threading](https://upload.wikimedia.org/wikipedia/commons/7/78/4-philosophers.gif "Schema")


**TODO**

for normal :
- [x] finish when last eat is done (ok for bonus?)
- [x] sometimes program dont exit after death
- [x] clean code
- [x] think before anything
- [x] dont stop thread after eat 
- [x] fix double unlock fsanitize warning on linux

for bonus : 
- [x] finish on first death
- [x] 1 thread that watch each philosophers
- [x] 1 semaphore per philo for dead check
- [x] maybe 1 semaphore for console 
- [x] clean code
- [x] think before anything
- [x] dont stop thread after eat 
