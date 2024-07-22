# CustomLockScreen
 Program that generates a new lock screen for you to enjoy)
 Only supports windows

 Instalation:
 1. Download files
 2. Execute Setup.exe
 3. Enjoy)

 Setup flags:
 1. -r - skip setup of registry keys on windows (part that requires administrator rights)
 2. -w(number) and -h(number) - set custom width and height for the image respectevly
 3. -i - skip setup of stock image (use it if "Image.png" already exists and you don't want it to be regenerated) (part that requires administrator rights)
 4. -j - skip setup of json

 Creating own scripts:
 1. Script must have extension ".lua" or ".exe" and be located at 'scripts' folder in the same directory as Generate image
 2. In case of lua width and height passed as parametrs, and return value in form of byte-encoded string is expected
 3. In case of exe first is being passed width, then height and then full path to image. It is expected for exe file to generate .png image on it's own. (output from std::cout will be printed in the same console where Generate.exe is executed)
 

 Library dependencies:
 1. WinReg - https://github.com/GiovanniDicanio/WinReg
 2. stb_image_write.h - https://github.com/nothings/stb/blob/master/stb_image_write.h
 3. random - https://github.com/ilqvya/random
 4. nlohmann json - https://github.com/nlohmann/json
