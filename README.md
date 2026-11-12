# UIN-Campus

## Versions des outils
```
g++ (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0
Ubuntu clang version 14.0.0-1ubuntu1.1
cmake version 4.4.4
1.10.1
GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1
git version 2.34.1
```

## Construction
```
cmake --preset debug
cmake --build --preset debug
./build/debug/uin-campus
```

## Partie C : débogage et sanitizers
- GDB : à i = 3, notes[3] est lu hors limites (condition `<=` au lieu de `<`).
- ASan/UBSan : heap-buffer-overflow à bug.cpp:8, corrigé, rapport disparu.
- LeakSanitizer et Valgrind : fuite de 40 octets à fuite.cpp:3, détectée par les deux.

## Partie B : éditeur
VS Code avec clangd, CMake Tools et CodeLLDB. clangd souligne une erreur volontaire (`int x = "texte";`) pendant la frappe grâce à compile_commands.json.

## Partie C : débogage et sanitizers
- GDB : à i = 3, notes[3] est lu hors limites (condition `<=` au lieu de `<`).
- ASan/UBSan : heap-buffer-overflow à bug.cpp:8, corrigé, rapport disparu.
- LeakSanitizer et Valgrind : fuite de 40 octets à fuite.cpp:3, détectée par les deux.

## Remarque
Compilé en C++20 (g++ 11.4) car g++ 14 n'est pas disponible sur Ubuntu 22.04 : `std::println` (C++23) n'est pas utilisable ici.
