# # Procedural Lockscreen and Desktop Wallpaper
 Program that generates a new lock screen for you to enjoy)
 Only supports windows for now

 ### How it works
 In order to set custom lock screen image you have to craete key 'LockScreenImagePath' in folder 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PersonalizationCSP'. After that you can just change image in that path to change lockscreen image.
 Also to generate image on every login program sets up task in task scheduler with trigger 'OnWorkstationUnlock' with scrript Generate.exe to execute
 Generate.exe chooses random script from folder scripts and parses it if it's lua or just executes it if it's .exe file.


 ### Instalation:
 1. Download files
 2. Extract them in any folder
 3. Execute Setup.exe
 4. Enjoy)


 ### Setup flags:
 <b>General</b>:
 - '-help' - show this list
 <b>Image</b>:
 - '-width=(number)' and '-height=(number)' - set custom width and height for the image respectevly
 <b>Skips:
 - '-skip-integrity' - skip integrity check
 - '-skip-json' - skip setup of json (use it if you made some changes in json and don't want it to be overwritten)
 - '-skip-registry' - skip setup of registry keys on windows (part that requires administrator rights)
 - '-skip-generate' - skip setup of initial call of Generate.exe (use it if 'Image.png' already exists and you don't want it to be regenerated)
 - '-skip-task' - skip setup of task in task scheduler (part that requires administrator rights)
 <b>Clearing</b> (when any of this flags are present, nothing is being set up):
 - '-clear-all' or '-clear' - remove files, registry keys and task in task scheduler
 - '-clear-files' - remove files (props.json and Image.png)
 - '-clear-registry' - remove registry keys
 - '-clear-task' - remove task from taskscheduler (if custom task name was set specify it using apropriate flag '-task_name=(task name)'. You can also specify custom powershell path)
 <b>Task</b>:
 - '-user-name=(user name)' - set user name who's login will change lock screen image (if empty login of any user will generate new image)
 - '-powershell-path=(path to powershell.exe)' - set custom path to powershell executable, default path is 'C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe' (currently it is required to setup task in task scheduler)
 - '-task-name=(task name)' - set custom name for the task in task scheduler (I personally don't know who will use this feature, but for people who use task scheduler it might be useful. By default task name is 'Generate Wallpaper')

 ### Generate flags:
 <b>General</b>:
 - '-help' - show this list
 <b>Image</b>:
 - '-width=(number)' and '-height=(number)' - set custom width and height for the image respectevly


 ### Add/Remove/Disable scripts
 To add script just copy .exe or .lua file into scripts folder. You can use any subdirectories inside it.
 If you want to disable script rename it's extension.


 ### Creating own scripts:
 - Script must have extension '.lua' or '.exe' and be located at 'scripts' folder in the same directory as Generate.exe
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
 - [x] Add clean up functions (remove registry keys, clear all files, remove task from task scheduler)
 - [x] Full support of wide chars
 - [ ] Function specifying custom image location
 - [ ] Gui interface
 - [x] Help functions
 - [ ] Add/Remove functions
 - [ ] <b>Site for scripts</b>
 - [ ] More default scripts!
