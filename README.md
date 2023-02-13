# Philosophers 

Le Dîner des philosophes.

![Threading](https://upload.wikimedia.org/wikipedia/commons/7/78/4-philosophers.gif?20080713171847 "Schema")

- Un programme va quitter tous ses thread en quittant
- Lire et écrire un entier CPU est une opération atomique
- Les opérations atomiques n'ont pas besoin d'être protégées 
- Les opérations non-atomiques doivent êtres protégées
- ``volatile`` protège les entiers CPU des optimisations non thread-safe


