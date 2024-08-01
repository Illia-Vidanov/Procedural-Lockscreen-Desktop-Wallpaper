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
 - '-r' - skip setup of registry keys on windows (part that requires administrator rights)
 - '-g' - skip setup of initial call of Generate.exe (use it if "Image.png" already exists and you don't want it to be regenerated)
 - '-j' - skip setup of json (use it if you made some changes in json and don't want it to be overwritten)
 - '-i' - skip integrity check
 - '-t' - skip setup of task in task scheduler (part that requires administrator rights)
 clearing:
 task scheduler:
 - '-user_name=(user name)' - set user name who's login will change lock screen image (if empty login of any user will generate new image)
 - '-powershell_path=(path to powershell.exe)' - set custom path to powershell executable (currently it is required to setup task in task scheduler)
 - '-task_name=(task name)' - set custom name for the task in task scheduler (I personally don't know who will use this feature, but for people who use task scheduler it might be useful. By default task name is "Generate Lock Screen Image")


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
 - [x] Automatically add task to task schduler
 - [ ] Add clean up functions (remove registry keys, clear all files, remove task from task scheduler)
 - [ ] More default scripts!
