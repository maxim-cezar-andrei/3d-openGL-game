# Proiect: [GLRunner]

## Concept
Joc 3D first-person inspirat de "Instant Death" de pe Steam.
Precision platformer — jucatorul navigheaza niveluri evitand 
obiecte rosii. Daca atinge rosu, moare si reincepe nivelul.
Fara checkpoint-uri, fara shooting, fara inamici cu AI.
Dificultatea vine din reflexe si precizie.

## Stil vizual
Minimalist:
- Alb/gri: geometria statica (podea, pereti)
- Rosu: orice e mortal
- Verde: platforme safe
- Albastru: spawn point (optional)
- Maxim 4-5 culori in tot jocul

## Stack tehnic
OpenGL 3.3 Core, GLFW, GLAD, GLM, ImGui, C++20

## Build
Exclusiv **Visual Studio 2022** prin `GLRunner.slnx`. Configuratie activa: `Debug|x64`.
Nu exista build pe linie de comanda.

## Structura folderelor
- `imgui/` — biblioteca ImGui, vendored complet (nu modifica)
- `Dependencies/include/` si `Dependencies/lib/` — GLFW, GLAD, GLM (nu modifica)
- `shader.vert` / `shader.frag` — citite de pe disk la runtime, trebuie sa fie langa executabil

## Stilul meu de lucru cu tine
Sunt incepator in OpenGL — primul proiect serios.
NU-mi scrie cod direct decat daca cer explicit.
Comportamentul dorit:
1. Explica-mi conceptele si pasii
2. Spune-mi ce fisiere sa modific si in ce ordine
3. Lasa-ma sa incerc singur
4. Daca spun "sunt blocat la X" sau "scrie tu", atunci scrie cod
5. Cand scrii cod, adauga comentarii si explica-mi deciziile
6. Nu adauga dependinte/biblioteci noi fara sa ma intrebi
7. Nu rescrie portiuni mari de cod — fa modificari minime, focalizate

## Arhitectura actuala
Tot codul de joc se afla intr-un singur fisier `main.cpp`.

State machine cu 3 stari: `STATE_MAIN_MENU` → `STATE_PLAYING` → `STATE_GAME_OVER`.

- Camera FPS cu WASD pe plan orizontal (miscarea pe Y e blocata intentionat)
- Collision detection AABB simpla cu obstacolele (stop on collision)
- Cuburi statice ca obstacole — pozitii definite in array-ul `hartaObstacole[]`
- Acelasi VAO/VBO (un cub unitar) e reutilizat pentru toate obiectele din scena; distinctia se face prin matricea `model` si uniformul `objectColor` trimise la shader

## Roadmap (in ordinea prioritatii)
1. Jump + gravitate + ground detection
2. Collision detection extinsa pe Y (pentru platforme/podea)
3. Categorii de obiecte: safe vs deadly vs trigger(finish)
4. Sistem de death & respawn
5. Spawn point per nivel
6. Obstacole mobile (platforme miscatoare, pendule)
7. Meniu + tranzitii intre niveluri
8. Polish: sunet, muzica, efecte de death

## Decizii luate
- Fara Bullet Physics sau alte engines — totul custom
- Fara shadere complicate initial — lighting basic
- Fara texturi complicate — culori solide pe primitive
- Minimalism intentionat (estetic + simplicitate tehnica)

## Ce NU vreau
- Features care nu sunt in roadmap fara sa discutam
- Biblioteci externe adaugate fara intrebare
- Refactorizari mari nesolicitate
- Cod fara comentarii explicative