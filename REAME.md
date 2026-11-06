# RPG médiéval en C / Raylib

Cahier des charges, version 2.

Les cases `[ ]` sont des décisions encore ouvertes. Tant qu'elles ne sont pas cochées, on ne code pas ce qui en dépend.

## Modif version 1

- **Périmètre** : le vrai périmètre est la section 17 de l'ancienne version (reprise ci-dessous en section 1). Tout le reste devient "idées après le MVP".
- **Niveaux** : 15 niveaux au lieu de 99. Exp du premier palier : 15, puis +20% par niveau (total 887 exp avec arrondis, voir section 7). L'ancienne formule en x1,15 sur 99 niveaux donnait environ 770 000 fois l'exp du niveau 1 au dernier palier : injouable.
- **Aura supprimée** : il ne reste que le Mana. Toutes les mentions de l'Aura disparaissent du modèle de données et du combat.
- **Build** : CMake au lieu de Makefile (Windows, macOS, Linux). Les commandes `make` de l'ancienne version sont remplacées.
- **Dépendances** : Raylib et cJSON, versions figées dans le dépôt (plus de "dernière version stable").
- **Données** : JSON, lu avec cJSON.
- **Vérification automatique** : une GitHub Actions compile le projet à chaque Pull Request.
- **Modèle de données** : stats de gameplay séparées de la référence au sprite. La position reste dans le joueur (c'est de l'état du monde, pas du rendu).
- **Équipe** : rôles et binômes de relecture écrits noir sur blanc (section 2).

## 1. Périmètre (MVP)

Une map, un donjon, un boss, 3 quêtes, 5 items, 3 types de monstres.

Exploration en temps réel, combats au tour par tour (inspirés de Monster Hunter Stories pour les mécaniques, jamais pour les noms ni les assets).

Tout ce qui n'est pas dans cette liste est hors périmètre tant que le MVP n'est pas jouable de bout en bout.

À décider :

- [ ] Titre du jeu
- [ ] Pitch en 3 phrases (histoire, objectif du joueur)
- [ ] Plateforme de référence pour les tests (Windows, macOS ou Linux)

## 2. Équipe et relecture

6 personnes : 2 avancés, 3 intermédiaires, 1 débutant.

| Niveau | Prénom |
| --- | --- |
| Avancé | Naider |
| Avancé | |
| Intermédiaire | |
| Intermédiaire | |
| Intermédiaire | |
| Débutant | |

Règles :

- Les deux avancés se relisent mutuellement sur le moteur (core, combat, mémoire).
- Le débutant est en binôme avec un intermédiaire dès sa première tâche : il code, l'intermédiaire relit.
- Personne n'approuve une PR qu'il ne comprend pas. Dans ce cas, il le dit et on explique.

## 3. Langage et conventions

- C11, bibliothèque Raylib.
- Noms de fichiers, fonctions, variables : `snake_case`. Constantes et macros : `MAJUSCULES_SNAKE_CASE`.
- Types : suffixe `_t` (ex. `player_t`).
- Documentation Doxygen sur chaque fonction (`@brief`, `@param`, `@return`). On commente le pourquoi, pas l'évidence.
- Zéro warning avec `-Wall -Wextra` (GCC / Clang).
- [ ] Langue du code et des commentaires : anglais (recommandé, cohérent avec les commits) ou français ?
- [ ] Compilateur de référence sous Windows (MinGW ou MSVC). Les flags d'avertissement diffèrent sous MSVC.

## 4. Build (CMake)

Commande unique pour tout le monde, depuis le dépôt cloné :

```.
cmake -B build
cmake --build build
```

Règles :

- `CMAKE_C_STANDARD 11`.
- Raylib et cJSON dans `third_party/` (sous-module Git ou `FetchContent` avec un tag précis). Aucune version "latest".
- Le `CMakeLists.txt` s'écrit avant toute autre tâche de code. Tant qu'un membre n'arrive pas à compiler, personne ne code.
- GitHub Actions : compilation sur Windows, macOS et Linux à chaque PR. Une PR qui ne compile pas n'est pas relue.

À décider :

- [ ] Versions exactes de Raylib et de cJSON
- [ ] Nom de l'exécutable

## 5. Git / GitHub

- Messages de commit en anglais : `prefix: short description` (ex. `feat: add player collision detection`).
- Préfixes : `feat`, `fix`, `docs`, `refactor`, `test`, `chore`.
- Une branche par tâche, en anglais : `feat/player-movement`, `fix/mana-gauge-overflow`.
- Personne ne code directement sur `main`. Intégration par Pull Request, relue par au moins une autre personne.
- Chaque tâche est une issue GitHub, décrite avant de coder. La branche référence l'issue.
- `.gitignore` à la racine : `build/`, `*.o`, `*.exe`, `.vscode/`, fichiers d'IDE.
- [ ] Qui a les droits administrateur pour protéger `main` (push direct interdit) ?

## 6. Répartition des tâches

Selon le niveau de C.

Débutant :

- Données : dictionnaire de dialogues, items, PNJ (fichiers JSON).
- Sprites et maps.
- Fonctions simples : affichage du menu, du texte, des jauges.

Intermédiaire :

- Déplacement du joueur, collisions, caméra.
- Inventaire.
- Système de quêtes.

Avancé :

- Cœur du moteur : `main.c`, boucle de jeu, machine à états.
- Combat au tour par tour.
- Gestion mémoire, chargement et déchargement des assets.

Le cœur moteur concentre les avancés : prévoir qu'un intermédiaire suive le moteur en observation pour ne pas dépendre de deux personnes.

## 7. Système de pouvoir

**Mana** : consommé par les sorts, affiché avec une jauge. Pas d'autre ressource dans le MVP.

**Exp** : gagnée en tuant des monstres et en terminant des quêtes.

**Niveaux** : de 1 à 15. Premier palier : 15 exp, puis +20% par palier. Table proposée (arrondie à l'entier), à stocker dans un fichier de données et non recalculée en code :

| Niveau atteint | Exp du palier | Exp totale cumulée |
| --- | --- | --- |
| 2 | 15 | 15 |
| 3 | 18 | 33 |
| 4 | 22 | 55 |
| 5 | 26 | 81 |
| 6 | 31 | 112 |
| 7 | 37 | 149 |
| 8 | 45 | 194 |
| 9 | 54 | 248 |
| 10 | 64 | 312 |
| 11 | 77 | 389 |
| 12 | 93 | 482 |
| 13 | 111 | 593 |
| 14 | 134 | 727 |
| 15 | 160 | 887 |

À décider :

- [ ] Exp donnée par chaque monstre et chaque quête (sans ça, la courbe ne veut rien dire)
- [ ] Les attaques physiques spéciales existent-elles ? Si oui, quel coût (Mana, cooldown, autre) ?

## 8. Inventaire

Objets, armes, armures, consommables. Armes enchantées : bonus de dégâts (ex. feu +10).

MVP : 5 items.

À décider :

- [ ] Taille max de l'inventaire et comportement quand il est plein (refus, empilement)
- [ ] Équipement : armes seulement, ou armes + armures + accessoires
- [ ] Liste des enchantements et leur amplitude

## 9. Modèle de données du joueur

Les stats de gameplay sont séparées du reste. La position est de l'état du monde (elle reste dans le joueur). Le rendu se limite à une référence (`sprite_id`) vers un asset chargé une seule fois par le module d'assets.

```c
typedef enum character_type {
    CHARACTER_MALE,
    CHARACTER_FEMALE
} character_type_t;

typedef struct player_stats_t {
    int level;
    int exp;
    int health_current;
    int health_max;
    int mana_current;
    int mana_max;
    int damage;
    int speed;
} player_stats_t;

typedef struct player_t {
    char name[64];
    character_type_t type;
    player_stats_t stats;
    float position_x;
    float position_y;
    int sprite_id;
    /* inventory : type à définir avec la section 8 */
} player_t;
```

## 10. Combat

Tour par tour, inspiré de Monster Hunter Stories.

À décider :

- [ ] Nombre d'actions par tour
- [ ] Types d'actions (attaque, sort, objet, fuite)
- [ ] Détermination de l'ordre (stat de vitesse)
- [ ] Stats des monstres et formule de dégâts
- [ ] Monstres capturables ou non (hors MVP par défaut)
- [ ] Mécanique spéciale du boss (phases, faiblesses)

## 11. Quêtes

MVP : 3 quêtes. L'exp est aussi gagnée en les terminant.

- [ ] Types retenus (chasse, collecte, escorte, histoire principale)

## 12. Formats de données (JSON, cJSON)

Schémas à écrire dans `docs/` avant que quiconque remplisse un fichier :

- dialogue : id, locuteur, texte, condition, réponses possibles
- item : id, nom, type, statistiques, prix
- pnj : id, position, dialogue lié, quête liée
- quête : id, étapes, récompense (exp, or, item)

Toute erreur de chargement est loggée avec le chemin du fichier. Aucun crash silencieux.

## 13. Sauvegarde

- [ ] Format (texte structuré ou struct binaire)
- [ ] Emplacement du fichier
- [ ] Quand sauvegarder (points de sauvegarde, auto, les deux)
- [ ] Comportement si la sauvegarde est corrompue

## 14. Erreurs et debug

- Toute allocation mémoire est vérifiée.
- Toute erreur de chargement d'asset affiche le chemin du fichier.
- Journal de debug activé par la macro `DEBUG`.
- Chaque module expose des fonctions testables hors de la boucle graphique.

## 15. Structure des dossiers (proposition à valider)

```.

├── CMakeLists.txt
├── README.md
├── .gitignore
├── third_party/      Raylib, cJSON (versions figées)
├── src/
│   ├── core/         main.c, boucle de jeu, machine à états, init
│   ├── physic/       physique, collisions
│   └── combat/       combat au tour par tour
    └── include/      fichiers .h
├── assets/
│   ├── sprites/
│   ├── audio/
│   └── data/         JSON : dialogues, items, PNJ, quêtes
├── build/            non versionné
├── docs/             conventions, schémas JSON, conception
└── tests/
```

Des dossiers s'ajoutent avec les modules (inventaire, quêtes, interface).

## 16. Graphismes et audio

À décider tôt, car ça bloque le débutant :

- [ ] Taille des tiles (16x16 ou 32x32)
- [ ] Résolution de la fenêtre, plein écran ou fenêtré
- [ ] Style et palette (mini charte de 10 lignes)
- [ ] Outil de sprites (Aseprite, LibreSprite, Piskel, autre)
- [ ] Sons et musiques : assets libres (CC0) ou créations originales (garder la licence de chaque fichier dans `docs/`)

## 17. Planning et définition de terminé

Pas de date de rendu officielle. Pour ne pas s'essouffler, on se fixe un premier jalon :

1. Moteur jouable : déplacement, map, collisions, caméra. Objectif : 3 semaines. Si ça dépasse, on réévalue tout le reste.
2. Combat au tour par tour fonctionnel.
3. Quêtes et inventaire.
4. Contenu : dialogues, donjon, boss, équilibrage.

Une tâche est terminée quand :

- le code est commenté au format Doxygen ;
- il compile sans warning, y compris sur la GitHub Actions ;
- il a été relu via Pull Request ;
- il a été testé à la main selon un scénario écrit dans l'issue.
