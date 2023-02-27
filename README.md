# Phonk: a 4Coder custom layer

Phonk should be used with Ryan Fleury's custom layer (https://github.com/Dion-Systems/4coder_fleury).

### Setup

#### Where to put this repo
- `cd to/4coder/custom/4coder_fleury/`
- `git clone` this repo

#### How to include it in custom layer
- Goto the file `4coder_fleury.cpp` 
- Add `#include "phonk/4coder_phonk.cpp"` below all the `#includes`'s
- Within the `custom_layer_init` function replace or `#if 0` all the lines that set the bindings and dyanmic bindings with `PK_SetupBindings(app);`

#### How to build
- `build.bat` in `custom/4coder_fleury`