# GLAD

Header-only GLAD 2.0.8 for OpenGL 4.6 Core.

Define `GLAD_GL_IMPLEMENTATION` in exactly one source file before including
`<glad/gl.h>`.

Regenerate with:

```sh
glad --api 'gl:core=4.6' --extensions='' --out-path third_party/glad --reproducible c --header-only
```
