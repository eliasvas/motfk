Current engine I'm working on. </br>
I want a nice and stable game framework, </br>
OpenGL ES 3.0 is used as the cross-platform gfx API right now, and SDL2 for everything else. </br>

### support
| Windows  | Linux | WASM |
| :-------------: | :-------------: | :-------------: |
| ✅ | ✅ | 🚧 |
### building
#### Linux
```sh
sudo apt-get install build-essential libsdl2-dev libsdl2-2.0-0 libasan6 libgles2-mesa-dev -y
git clone https://github.com/eliasvas/engine
cd engine && git submodule update --init && mkdir build
cd build && cmake .. && make --build . -j10
```
#### WASM (Emscripten)
```sh
sudo apt-get install build-essential libsdl2-dev libsdl2-2.0-0 libasan6 libgles2-mesa-dev -y
sudo apt install clang emscripten -y
git clone https://github.com/eliasvas/engine
cd engine && git submodule update --init && mkdir -p build/web
set EMCC_DEBUG=1
emcmake cmake ../..
cmake --build . -j10
emrun game.html
-sMEMORY64
```