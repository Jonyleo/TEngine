# CGJ Project

### Group 1
João Oliveira 93728
Giulio Camasso 105147

## Instalation

### Dependencies

glfw3 (https://www.glfw.org/)
- copy glfw3.lib and glfw3dll.lib to dependencies/lib 
- copy the contents of the include folder to dependencies/include

glew (https://glew.sourceforge.net/)
- copy glew32.dll and glew32.lib to dependencies/lib
- copy the contents of the include folder to dependencies/include

glm (https://github.com/g-truc/glm/releases/tag/0.9.9.8)
- copy the contents of the first glm folder to dependencies/include

The dependencies' folder structure should be the following

![Folder structure](FolderStructure.PNG)

### Building

Navigate to tangram/tangram.cpp and open the file
Run

The executable will be accessible in TEngine\out\build\x64-Debug\tangram but it must be in the
same directory as the assets folder and glew32.dll.