# Setup

Follow the guide on [MCU On Eclipse](https://mcuoneclipse.com/2021/05/01/visual-studio-code-for-c-c-with-arm-cortex-m-part-1/) to set up and install required tools.

* [ARM Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* [CMake](https://cmake.org/download/)
* [Ninja](https://github.com/ninja-build/ninja/releases) (Place `ninja.exe` inside CMake `/bin` directory)
* [Segger](https://www.segger.com/downloads/jlink#J-LinkSoftwareAndDocumentationPack)

To install VS Code extensions run the following:

```
code --install-extension ms-vscode.cpptools
code --install-extension marus25.cortex-debug
code --install-extension twxs.cmake
code --install-extension dan-c-underwood.arm
code --install-extension zixuanwang.linkerscript
```

# Generating Build Files

1. Create a directory `/build`
1. Inside this build directory run the command: `cmake -DCMAKE_MAKE_PROGRAM=ninja.exe -G "Ninja" ..` (this creates the build files)
1. Run `ninja`, or use the VS Code build task to create build executables

# Debugging

VS Code Run/Debug should work out of the box
