06.10.2016 || dmix90
============================================================
#Windows Store application launcher for Steam
============================================================

You need to generate app executables using launcher itself or you will have to find application id's manually.

#How to generate executables using StoreAppLauncher.
1. Run StoreAppLauncher_xXX.exe with administrator rights( Right Click -> Run as Administrator )
2. Check or uncheck 'Filter' checkbox( when it's active it should filter out various built in and service apps )
3. Click 'Generate" button in a dialog box, then _Applications folder should open where you will find generated executables you can use in Steam or anywhere else. There also should be _Applications.txt file which can provide a hint to an actual application name.

#How to manually find Application ID:
1. Launch 'Go to Applications Folder' shortcut from archive you downloaded.
2. Press the F10 key on the keyboard. The window now should get a menu if there wasn't one already, click View -> Choose Details.
3. In the next dialog, tick the AppUserModelId checkbox.
4. Switch folder view mode to "Details".
5. Find application you need and look at AppUserModelID column it will show ID you looking for.
6. Enter this id as a parameter to StoreAppLauncher_xXX.exe or rename launcher executable to this id( for example it should look like this -> Microsoft.WindowsCalculator_8wekyb3d8bbwe!App.exe )

============================================================

#How to use launcher:
1. Copy -> Paste StoreAppLauncher_xXX to a folder( example D:\Games\WindowsStore\_Games\ ) where you 
want to keep your app launchers.
2. Generate executables by using launcher itself or by manual method mentioned above.
3. Add executable you just generated as a Non-Steam Game using Steam UI.

#OPTIONAL:
4. Find what you added in your library -> Right Click -> Properties.
5. Click -> Set Launch Options

Only four additional parameters are supported at this moment:

#"-bigpicture":
Alt+Tab -> press Guide. You should hear "Beep" and get back to Steam Big Picture Interface.
Alt+F4  -> press Guide + Back. You should hear "Beep" , app should close and you should be back to Big Picture interface.
Win+G 	-> press RightThumb + Back, You should hear "Beep" and Windows 10 Game Bar should popup.

NOTE: To make Guide button work in this scenario you need to disable Guide button functionality in Xbox Game Bar options.

#"-bigpicture0":
Alt+Tab -> press Back + Start + LeftTrigger. You should hear "Beep" and get back to Steam Big Picture Interface.
Alt+F4  -> press Back + Start + LeftTrigger + RightTrigger. You should hear "Beep" , app should close and you should be back to Big Picture interface.

#"-downsampling":
Attempts to switch to maximum available screen resolution for primary display and switch back to previous one when application is closed.

#"-maxres_XXXXxXXXX":
This parameter only works when paired with "-downsampling" parameter. It's purpose is to set maximum desired downsampling resolution using this parameter( example: -downsampling -maxres_2560x1440 ). This resolution must be available in Windows Display properties.

==============================================================================================================
#Steam launch options example: -bigpicture -downsampling -maxres_2560x1440
#Windows shortcut example: ..\_Games\Microsoft.SpartaUWP_8wekyb3d8bbwe!GearGameShippingPublic.exe -downsampling
==============================================================================================================

=============
#VERSION 1.0.0
=============
Features:
1. Launch Windows Store apps through Steam with displayed status while app is running.
2. If explorer is not running launcher will attempt to start if first and then launch app itself, when app is closed it launcher will try to close explorer as well. This is needed since Windows Store apps do not work without explorer. Should be useful to all 5 people in the world( hey bros! ) who use Steam Big Picture as their default Windows startup Shell :D
3. Automatic Windows Desktop resolution switch before launching app and restore previous resolution when app is closed. Should be useful for certain Windows Store games that do not properly support internal resolution scaling( Quantum Break, Killer Instinct, Forza 6 Apex, Forza Horizon 3 etc ). To use this add "-downsampling" parameter to your launch options. Limit maximum downsampling resolution with "-maxres_XXXXxXXXX" parameter.
4. Use "-bigpicture" parameter if you want to play Windows Store games using Steam Big Picture interface. Without this parameter you will control BP interface in background when you click Guide button during gameplay. With this parameter Guide button will be used as Alt+Tab keyboard shortcut. This parameter might affect controller input a little since when it's enabled it checks for button presses in background at the same time with game. More information on this parameter above.
