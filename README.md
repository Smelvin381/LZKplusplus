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

// #include <memory> // Smart pointers (unused)
// Secondary functions
// template<typename... Args>
// void STOP_PROGRAM(std::unique_ptr<Args>&... args) {
//   // Gracefully stop the program

//   std::cout << "\033[31m \u001b[1m ";
//   std::cout << " @ STOPPED without destructing objects.";
//   std::cout << " \033[0m" << std::endl;

//   PROGRAM_RUNNING = false;

//   // Delete the dynamically allocated objects
//   // Unique pointer made like this: std::unique_ptr<cslib::ThreadParty> threadPool(new cslib::ThreadParty());
//   (args.reset(), ...);

//   // Run std::exit(0) in a separate thread
//   auto asyncResult = std::async(std::launch::async, [](){ std::exit(0); });
//   asyncResult.wait(); // Wait for the async operation to complete
// }