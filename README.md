# Philosophers 

![Le Dîner des philosophes](https://fr.wikipedia.org/wiki/D%C3%AEner_des_philosophes).

**Notes sur le multi-threading :**

- Un programme va quitter tous ses threads non détachés en quittant
- Lire et écrire un entier CPU est une opération atomique
- Les opérations atomiques n'ont pas besoin d'être protégées 
- Les opérations non-atomiques doivent êtres protégées
- ``volatile`` protège les entiers CPU des optimisations non thread-safe

**Le diner des philosophes modélisé en Réseau de Petri (wikipedia / CC BY-SA 3.0) :**

![Threading](https://upload.wikimedia.org/wikipedia/commons/7/78/4-philosophers.gif?20080713171847 "Schema")


**TODO**

for bonus : 
1 thread that watch each philosophers
1 semaphore for global dead check
maybe 1 semaphore for console 
