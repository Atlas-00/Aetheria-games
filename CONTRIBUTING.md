# CONTRIBUTING.md — Comment travailler dans le projet Aetheria

## 1. Ce qu'il faut installer avant de commencer

À faire une seule fois, sur ta machine.

### macOS

- Xcode Command Line Tools : `xcode-select --install`
- CMake : `brew install cmake`
- Git : déjà installé avec Xcode

### Windows

Compilateur de référence : **MSVC** (c'est celui que teste la CI sur Windows).

- Visual Studio Build Tools : <https://visualstudio.microsoft.com/downloads/> (section « Build Tools »). Dans l'installeur, coche la charge de travail **Desktop development with C++**.
- CMake : <https://cmake.org/download/> (coche « Add CMake to the system PATH » pendant l'installation).
- Git : <https://git-scm.com>
- Visual Studio Code : <https://code.visualstudio.com/>, avec ces extensions :
  - C/C++ Extension Pack
  - CMake Tools
  - Error Lens
  - Doxygen Documentation Generator
  - markdownlint
  - JSON Crack
  - Git Graph
  - GitLens

Les thèmes d'icônes (Fluent Icons, Material Icon Theme) sont facultatifs.

### Linux (Ubuntu / Debian)

Personne dans l'équipe ne travaille sous Linux, mais la CI compile sur Ubuntu. Si tu veux compiler en local :

```bash
sudo apt install build-essential cmake git libasound2-dev libx11-dev libxrandr-dev \
  libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
  libwayland-dev libxkbcommon-dev
```

### Vérifier que tout est là

```bash
git --version
cmake --version
```

## 2. Cloner le projet (première fois)

Ne clone pas sans les sous-modules, sinon `third_party/` sera vide.

```bash
git clone --recurse-submodules https://github.com/Atlas-00/Aetheria-games.git
cd Aetheria-games
```

Si tu as déjà cloné sans `--recurse-submodules` :

```bash
git submodule update --init --recursive
```

Tu dois voir du contenu dans `third_party/raylib` et `third_party/cJSON`. S'ils sont vides, refais cette étape.

## 3. Compiler et lancer (même commande pour tout le monde)

Depuis la racine du projet :

```bash
cmake -B build
cmake --build build
```

Lancer le jeu :

macOS / Linux :

```bash
./build/Aetheria
```

Windows (PowerShell) :

```powershell
.\build\Aetheria.exe
```

Règles :

- On ne tape jamais `make` ni `gcc main.c` à la main. CMake choisit le compilateur de ta machine.
- Le dossier `build/` n'est jamais commité (il est dans `.gitignore`).
- Si tu changes de branche ou que ça ne compile plus, repars de zéro :

```bash
rm -rf build
cmake -B build
cmake --build build
```

Sous Windows PowerShell, remplace la première ligne par `Remove-Item -Recurse -Force build`.

## 4. Mettre à jour ton projet chaque jour

Avant de commencer à coder :

```bash
git checkout main
git pull --recurse-submodules
```

Ça met à jour le code et les versions de raylib / cJSON.

## 5. Travailler sur une tâche (workflow obligatoire)

On ne code jamais directement sur `main`. Le cycle est toujours le même.

### 5.1 Choisir une issue

Chaque tâche est une issue GitHub. Pas d'issue, pas de code. Prends une issue qui correspond à ton niveau.

### 5.2 Créer une branche

Nom en anglais : préfixe + nom de la tâche.

```bash
git checkout main
git pull --recurse-submodules
git checkout -b feat/player-movement
```

Autres exemples : `fix/mana-gauge-overflow`, `docs/readme-setup`, `chore/ci-workflow`.

### 5.3 Coder

- Tu ne touches que les fichiers de ta tâche.
- Conventions : `snake_case`, types en `_t`, Doxygen sur chaque fonction, zéro warning.
- Tu testes à la main avec le scénario écrit dans l'issue.

### 5.4 Committer

Message en anglais, avec préfixe :

```bash
git add src/player.c
git commit -m "feat: add player movement with collision"
```

Préfixes autorisés : `feat`, `fix`, `docs`, `refactor`, `test`, `chore`.
Interdit : `"ça marche"`, `"update"`, `"fix bug"`.

### 5.5 Pousser et ouvrir une Pull Request

```bash
git push -u origin feat/player-movement
```

Puis sur GitHub : « Compare & pull request » vers `main`. Dans la description : ce que tu as fait, et comment le tester.

Le titre de la PR suit la même convention que les commits (`feat: ...`, `fix: ...`). Avec « Squash & Merge », c'est en général lui qui devient le commit sur `main`.

### 5.6 Relecture

- La PR doit compiler sur GitHub Actions (Windows, macOS, Linux). Si elle est rouge, tu corriges, on ne la relit pas.
- Elle doit être relue par au moins 1 personne.
- Si le relecteur ne comprend pas ton code, c'est à toi de l'expliquer.
- Une fois approuvée, on fait « Squash & Merge ». Jamais de push direct sur `main`.

## 6. Ajouter un fichier ou un module

1. Crée ton `.c` dans `src/` (dans le sous-dossier du module) et ton `.h` dans `src/include/`.
2. Tu n'as rien à modifier dans `CMakeLists.txt` : tous les `.c` de `src/` sont récupérés automatiquement. Si ton nouveau fichier n'est pas pris en compte, refais `cmake -B build`.
3. Un asset va dans `assets/` (sprites, audio, données JSON). Si tu ne l'as pas créé toi-même, note sa licence dans `docs/`.
4. Ne commite jamais `build/`, les fichiers `*.o`, `*.exe` ou `.vscode/`.

## 7. Problèmes fréquents

| Problème | Solution |
| --- | --- |
| `third_party/raylib` est vide | `git submodule update --init --recursive` |
| `CMake Error ... Compatibility with CMake < 3.5` | Tu n'as pas le dernier `CMakeLists.txt` : `git pull --recurse-submodules` |
| Ça compile chez toi mais pas sur GitHub Actions | Cherche : un nom de fichier dont la casse diffère (Linux la respecte), un fichier ignoré par `.gitignore` donc jamais poussé, un chemin absolu, un sous-module non initialisé. Lis le log du job rouge |
| `Aetheria.exe` introuvable sur Windows | Il est dans `build/Aetheria.exe` (pas dans `build/Debug/`), grâce au `CMakeLists.txt` |
| Conflit sur `git pull` | Ne fais jamais `git push --force`. Demande à ton binôme, on résout ensemble |
