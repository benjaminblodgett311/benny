# Setting up Visual Studio Project for C++ with GLFW and ImGui

Follow these instructions to set up a Visual Studio project for C++ development with GLFW and ImGui libraries.

## Step 1: Clone Repository

Clone the repository:

```bash
git clone git@github.com:benjaminblodgett311/benny.git
```

## Step 2: Create Visual Studio Project

1. Open Visual Studio.
2. On the right, under "Create a new project", select "Continue without code".
3. Select `File` >> `New` >> `Project from Existing Code`.
4. Choose a Visual C++ project.
5. Select the location of the newly cloned repository and set the desired name.
6. Select "Next".
7. For project type, switch to "Console Application Project".
8. Hit "Finish".
9. Under the "Source Files" filter open main.cpp.
10. At the top of the Solution Explorer window, enable "Show All Files"

## Step 3: Configure Project Properties

1. Right-click the project in the Solution Explorer and go to "Properties".
2. Under "Debugging", change "Working Directory" to `$(ProjectDir)res`.
3. Under "VC++ Directories", change "Include Directories" to `$(ProjectDir)libraries\include;$(IncludePath)` and "Library Directories" to `$(ProjectDir)libraries\lib;$(LibraryPath)`.
4. Under "Linker" >> "Input", change "Additional Dependencies" to `glfw3.lib;opengl32.lib;%(AdditionalDependencies)`.
5. Hit "Apply".

## Step 4: Add Submodules

Open a terminal under "View" or press Ctrl+`.

```bash
git submodule update --init
```

## Step 5: Include Files in Project

To include a file or the contents of a directory recursively into the project, right-click it in the Solution Explorer and select "Include In Project" from the drop-down menu.

Either include the entire contents of `libraries/include` or just the `ImGui` folder and the `glad.c` file inside the `libraries/include/glad` folder.

## Step 6: Finished

The project should now build with no errors and only warnings from GLFW.
