# GLRunner - Joc 3D First-Person (C++, OpenGL) - in lucru

Joc 3D de tip platformer cu perspectiva first-person, construit
cu OpenGL. Navigheaza nivelurile fara sa atingi obiectele rosii.
Fara checkpoints. Doar reflexe si precizie.

## Tehnologii folosite
- C++
- OpenGL 3.3 
- GLFW (fereastra si input)
- GLAD (OpenGL loader)
- GLM (matematica 3D)
- ImGui (meniuri si overlay)

## Functionalitati implementate
- Camera FPS (mouse look)
- Miscare WASD 
- Saritura, gravitatie si detectie sol
- Detectie coliziuni AABB (obstacole orizontale)
- Coliziuni verticale (aterizare pe platforme)
- Categorii de obiecte: SAFE (verde) si UNSAFE (rosu)
- Flux moarte si restart (ecran Game Over cu Home / Run Again)
- Meniu principal (Space pentru start)

## Controale
- WASD - miscare
- Mouse - privire
- Space - saritura
- R - restart dupa moarte
- Escape - iesire
