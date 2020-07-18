# How to compile

## clion
- Start folder in clion
- Right click on project folder and press "reload CMake project""

## vs studio
- open vs studio 2019
- make sure it uses cmake
- vs studio doesnt respect the settings of cmake for 32 bit (which it needs to be), you can set this up @ the settings somewhere

## other editor's
- Prepare the 
```cmake.exe -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - NMake Makefiles" C:\path\to\folder\D2PacketBased```

# After compiling
- When running the newly created executable, you need to tell the path of the game, `-D c:\games\Diablo113D` (only works with 1.13d currently)
- Make sure that 1.13d version can run without the cd
- The executable expects there is a `data` folder (in the repo `data`), in case of clion copy the data to `cmake-build-debug` as that is where the executable is compiled at and ran


# What does it do?
Currently not too much,
 
To get some basic testing call the program with `-testFile ..\Hooks\GameFiles\5-42-181-35-4000-1594805931745-sc.log` as that loads the test file
 
This is a file that is the received data from a battle net game, not the data that is send. Just what is received
  


