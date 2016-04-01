# Windows Store application launcher for STEAM
# OLD
___________________________
How to find Application ID:

1. Launch Go to Applications Folder link.
2. Press the F10 key on the keyboard. The window will get a menu, choose View -> Choose details.
3. In the next dialog, tick the AppUserModelId checkbox.
4. Switch folder view mode to "Details".
5. Find application you need and look at AppUserModelID column it will show ID you looking for.
6. Enter this id as a parameter to StoreAppLauncher_xXX.exe
________________________
Usage example:

Add StoreAppLauncher_xXX.exe to Steam as a non-Steam Game then Right Click->Properties->Set Launch Options, enter ID there.
You will need separate StoreAppLauncher.exe for each game you will add to Steam as it will share launch parameters between identical .exe files, so just create a copy of launcher.
________________________
Known Issues:

1. If you press guide button on your controller while you running windows app steam will switch focus to itself and you will navigate big picture interface 

Workaround: Add additional "bp"(without quotes) parameter to parameter list. After that you will be able to use implemented Alt+Tab functionality which you can access from gamepad.
Press and hold for ~500ms Back + Start + LeftTrigger( Previous version )
You will switch to previous window, in my case it was always Steam Big Picture interface. Repeat to get back to game.

Example: D:\Games\WindowsStore\StoreAppLauncher_x64.exe Microsoft.WindowsCalculator_8wekyb3d8bbwe!App bp
