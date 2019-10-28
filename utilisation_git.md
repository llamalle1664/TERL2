Avant de commencer une nouvelle branche

- `git checkout master` pour retourner sur la branche master
- `git pull` pour récupérer le code du dépôt (s'il y a eu des changements)

Créer une nouvelle branche

- `git checkout -b <nom de la branche>` pour créer la nouvelle branche et passer dessus

Faire les changements et puis commit

- `git add <fichiers>` pour ajouter les fichiers au prochain commit
- `git commit -m <message>` pour commit les changements

Une fois que tous les changements sont terminés

- `git checkout master` pour retourner sur la branche master
- `git pull` pour récupérer les changements du dépôt
- `git merge <nom de la branche>` pour fusionner la branche <nom de la branche> avec master

Et puis si la branche n'est plus utilisée

- `git branch -d <nom de la branche>` pour supprimer la branche du dépôt local

---

Commandes utiles

Pour push la branche dans le dépôt

- `git push`

Pour savoir sur quelle branche on est

- `git branch`

Pour avoir une liste des changements dans la branche actuelle

- `git status`

Les noms en vert sont les fichiers qui seront dans le prochain commit. Les fichiers en rouges ne sont pas dans le prochain commit (pour les ajouter il faut faire `git add <fichiers>`)
