#Windows Store application launcher for STEAM
________________________________________________________________________________________________

You need to find application id first.
Take a look at AppID_List.txt, there you will find some id's for "big" games from Windows Store.

#How to find Application ID:
1. Launch Go to Applications Folder link from archive you downloaded.
2. Press the F10 key on the keyboard. The window will get a menu, click View -> Choose Details.
3. In the next dialog, tick the AppUserModelId checkbox.
4. Switch folder view mode to "Details".
5. Find application you need and look at AppUserModelID column it will show ID you looking for.
6. Enter this id as a parameter to StoreAppLauncher_xXX.exe
________________________________________________________________________________________________

#How to use launcher:
1. Copy->Paste StoreAppLauncher_xXX to a folder( example D:\Games\WindowsStore\_Games\ ) where you 
want to keep your game launchers. You need a copy with unique name for each application you want 
to launch from Steam because it will overwrite launch parameters if you have single executable 
for multiple applications in your library.
2. Add executable you just created as a Non-Steam Game using steam interface.
3. Find what you added in your library -> RightClick -> Properties.
4. Click Set Launch Options.
5. There you should enter/paste Windows Store application id -> Click OK and you ready to go.
6(Optional). Add additional parameter to your launch options if you want to quit or Alt+Tab from game using controller.
It will only switch between two last windows so be careful with that.

Only two additional parameters are supported at this moment:

"bp":

Alt+Tab -> press Back + Start + LeftTrigger. You should hear "Beep" and get back to Steam Big Picture Interface.

Alt+F4  -> press Back + Start + LeftTrigger + RightTrigger. You should hear "Beep" , game should close and you should be back to Big Picture interface.

"bp1":

Alt+Tab -> press Guide. You should hear "Beep" and get back to Steam Big Picture Interface.

Alt+F4  -> press Guide + Back. You should hear "Beep" , game should close and you should be back to Big Picture interface.
________________________________________________________________________________________________

Launch Options example: Microsoft.WindowsCalculator_8wekyb3d8bbwe!App bp1
________________________________________________________________________________________________


#NEW: 
1. If you used previous version of a launcher and have a lot of copies already you can try to use Update function. 
Just copy new version to a directory where you keep other launchers and launch it without any parameters through explorer. 
It will ask if you want to update current directory executable files. 
IT WILL TRY TO OVERWRITE ANY .exe FILES IT HAS BEEN ABLE TO FIND IN CURRENT DIRECTORY, SO BE CAREFUL! There should be a list of files in console window.

2. Now launcher will try to open explorer.exe if there is currently no such process running and close it after you quit game. Should be useful if you using Steam as startup shell for windows because UWA's wont launch if there is no explorer shell running -_-

3. Added launch parameter "bp1". Now you can use Guide button on your controller to Alt+Tab to Steam Big Picture interface.
4. Alt+F4 functionality by pressing Guide + Back if you using new "bp1" parameter or Back+Start+LeftTrigger+RightTrigger if you using old "bp" parameter.
5. Executable size is greatly reduced. I used uncompressed icon in previous version, lol. Sorry -_-
6. Now launcher runs in two threads( one for checking game/app status other for checking controller input ).
Game checking thread is updated every ~1.5 seconds while controller thread is updated every ~150ms. 
There might be problems with that but i don't think i noticed any. If there is no "bp" or "bp1" parameter present app will run in single thread mode like it was in previous version. Pretty sure CPU usage stayed the same compared to previous version ( ~0.01% or something like that on 6700k at 4.5ghz )
7. Now you should not be able to open launcher if it is already running. -_-
