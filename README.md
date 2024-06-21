# LZK-Rework in C++20
---
### Setup (Github) codespace
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install g++-13

into the args .vscode/tasks.json file
```json
"-std=c++20", // C++20
"-O3", // Higher compile time but better performance
"-fdiagnostics-color=always",
"-g",
"${file}",
"-o",
"${fileDirname}/${fileBasenameNoExtension}"
```


### Notes/Code archive (ignore)
