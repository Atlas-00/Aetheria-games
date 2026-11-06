Voilà, juste la section 18, prête à coller dans un fichier `INSTRUCTION.md` à la racine de ton repo:

# INSTRUCTION.md — Comment travailler dans le projet Aetheria

C'est le guide à suivre à la lettre pour rejoindre le projet, coder une tâche et la faire valider. Si tu bloques ici, tu ne codes pas encore.

## 1. Ce qu'il faut installer avant de commencer

Installer une seule fois, sur ta machine:

**Sur macOS:**

- Xcode Command Line Tools: `xcode-select --install`
- CMake: `brew install cmake`
- Git: déjà installé avec Xcode

**Sur Windows:**

- Visual Studio Code : <https://code.visualstudio.com/>
    **Avec les extension suivantes**
  - C/C++ Extensions Pack
  - Error lens
  - Doxygen Documentation Generator
  - Fluent Icons
  - Material Icon Theme
  - Makefile Tools
  - markdownlint
  - JSON Crack
  - Git Graph
  - GitLens
- CMake: <https://cmake.org/download/>
- Git: <https://git-scm.com>

**Sur Linux (Ubuntu/Debian):**

```bash
sudo apt install build-essential cmake git

Vérifier que tout est là:

git --version
cmake --version
```

## 2. Cloner le projet (1ère fois)

*Ne clone pas sans les submodules, sinon `third_party/` sera vide.*

En une seule commande:

```bash
git clone --recurse-submodules <https://github.com/Atlas-00/Aetheria-games.git>
cd Aetheria
```

*Si tu as déjà cloné sans `--recurse-submodules`:*

git submodule update --init --recursive

Tu dois voir du contenu dans `third_party/raylib` et `third_party/cJSON`. Si ces dossiers sont vides, tu t'es trompé d'étape, refais celle-ci.

## 3. Compiler et lancer (la même commande pour tout le monde)

Depuis la racine du projet `Aetheria/`:

```bash
cmake -B build
cmake --build build
```

Lancer le jeu:

### macOS / Linux

```bash
./build/Aetheria
```

### Windows (PowerShell)

```bahs
.\build\Aetheria.exe
```

*Règles:*

- On ne tape jamais `make`, `gcc main.c`, ou `cmake..` à la main. C'est CMake qui choisit le compilateur de ta machine.
- Le dossier `build/` n'est jamais commit. Il est dans le `.gitignore`.
- Si tu changes de branche ou que ça ne compile plus, repars de zéro:

```bash
rm -rf build
cmake -B build
cmake --build build
```

# Sur Windows PowerShell: Remove-Item -Recurse -Force build

## 4. Mettre à jour ton projet chaque jour

Avant de commencer à coder, toujours:

```bash
git checkout main
git pull --recurse-submodules
```

Ça te met à jour le code + les versions de Raylib / cJSON.

## 5. Travailler sur une tâche (le workflow obligatoire)

On ne code jamais directement sur `main`. Le cycle est toujours le même:

## 5.1 Choisir une Issue sur GitHub

Chaque tâche est une Issue. Pas d'Issue = pas de code. Tu prends une Issue qui correspond à ton niveau.

## 5.2 Créer une branche

En anglais, avec le préfixe + le nom de la tâche:

```bash
git checkout main
git pull --recurse-submodules
git checkout -b feat/player-movement
```

# Autres exemples: fix/mana-gauge-overflow, docs/readme-setup

## 5.3 Coder

- Tu ne touches que les fichiers de ta tâche.
- Tu respectes les conventions: `snake_case`, types en `_t`, Doxygen sur chaque fonction, zéro warning.
- Tu testes à la main avec le scénario écrit dans l'Issue.

## 5.4 Committer

En anglais, avec le préfixe:

```bash
git add src/player.c
git commit -m "feat: add player movement with collision"
```

Préfixes autorisés: `feat`, `fix`, `docs`, `refactor`, `test`, `chore`.
Interdit: `commit "ça marche"`, `commit "update"`, `commit "fix bug"`.

## 5.5 Pousser et ouvrir une Pull Request

```bash
git push -u origin feat/player-movement

Puis sur GitHub: *Compare & pull request* -> vers `main`, avec la description: ce que tu as fait + comment tester.
```

## 5.6 Relecture

- Ta PR doit compiler sur GitHub Actions (Windows, macOS, Linux). Si elle est rouge, tu corriges, on ne la relit pas.
- Elle doit être relue par au moins 1 personne.
- Si le relecteur ne comprend pas ton code, c'est à toi d'expliquer, pas à lui de deviner.
- Une fois approuvée, c'est toi qui fais *Squash & Merge* ou le relecteur, pas de merge direct sur `main`.

## 6. Ajouter un nouveau fichier / module

1. Crée ton `.c` dans `src/` et ton `.h` dans `src/include/`.
2. Si tu crées un nouveau dossier (ex. `src/inventory/`), préviens un avancé: il faut vérifier que le `CMakeLists.txt` le récupère bien.
3. Si tu ajoutes un asset, mets-le dans `assets/` (sprites, audio, data JSON) et note sa licence dans `docs/` si ce n'est pas toi qui l'as créé.
4. Ne commit jamais `build/`, `_.o`, `_.exe`, `.vscode/`.

## 7. Problèmes fréquents

Problème Solution
`third_party/raylib` est vide `git submodule update --init --recursive`
`CMake Error... Compatibility with CMake < 3.5` Tu as une vieille Raylib, fais `git pull --recurse-submodules` pour récupérer la version fixée
Ça compile chez toi mais pas sur GitHub Actions Tu as un warning en local que tu as ignoré, ou un chemin en absolu. Corrige et re-push
`Aetheria.exe` introuvable sur Windows Il est dans `build/Aetheria.exe` (pas dans `build/Debug/`), grâce au `CMakeLists.txt`
Conflit sur `git pull` Ne fais pas de `git push --force`. Demande à ton binôme, on résout ensemble
