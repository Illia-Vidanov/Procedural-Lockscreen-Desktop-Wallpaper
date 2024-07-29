# CustomLockScreen
 Program that generates a new lock screen for you to enjoy)
 Only supports windows for now

 ### Instalation:
 1. Download files
 2. Extract them in any folder
 3. Execute Setup.exe
 4. Enjoy)

 ### Setup flags:
 image:
 - '-w=(number)' and '-h=(number)' - set custom width and height for the image respectevly
 skips:
 2. '-r' - skip setup of registry keys on windows (part that requires administrator rights)
 3. '-g' - skip setup of initial call of Generate.exe (use it if "Image.png" already exists and you don't want it to be regenerated)
 4. '-j' - skip setup of json (use it if you made some changes in json and don't want it to be overwritten)
 5. '-b' - skip setup of batch file (use it if you made some changes in batch file and don't want it to be overwritten)
 6. '-i' - skip integrity check
 7. '-t' - skip setup of task in task scheduler ()
 clearing:

 ### Creating own scripts:
 - Script must have extension ".lua" or ".exe" and be located at 'scripts' folder in the same directory as Generate.exe
 Lua:
 width and height passed as parametrs, and return is expected to be in a form of a byte-encoded string
 Exe:
 width height and path are passed as parametrs (width - first, height - second, path - third) and it is expected for exe file to generate .png image on it's own.
 If an error is occured you can exit with non-zero exit code and Generate.exe will terminate (output from std::cout will be printed in the same console where Generate.exe is executed)
 

 ### Library dependencies:
 - [WinReg](https://github.com/GiovanniDicanio/WinReg)
 - [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)
 - [random](https://github.com/ilqvya/random)
 - [nlohmann json](https://github.com/nlohmann/json)

 ### TODO:
 - [x] Add embeded manifest
 - [ ] Automatically add task to task schduler
 - [ ] Add clean up functions (remove registry keys, clear all files, remove task from task scheduler)
 - [ ] More default scripts!
