# Timer

![](screenshot.png)

![](screenshot-notification.png)

![](screenshot-tooltip.png)

A simple timer application written in C++/Qt for the GNU/Linux and MS Windows Operating System, licensed under the GPL V 3.0.  The current version is 1.3 released on 4/20/20.

## Features:
+ Rapid entry!  

  Just open timer:
	1. Type a number for minutes
	2. Press enter to advance to message field
	3. Type your message (optional)
	4. Press enter to start the timer
	
+ Full system tray support for GNU/Linux and MS Windows supporting:
  - Close program window minimizes to tray
  - Single click minimizes AND restores from tray
  - Cancel alarm from tray
  - Quit timer from tray
  - Tooltip on tray icon to easily identify a timer from multiple instances.
  - Tray icon changes color to green when running, and red when timer has expired.
 
+ System notifications that shows your custom message and allow you to cancel the alarm from the notification.
+ Start / Stop buttons change colors when hovering

## Installation:

### Windows:
1. Install the official Microsoft Redistributable C++ for Windows 64 bit 2015,2017 and 2019 from (https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).  Here is a direct link to their exe: [vc_redist.x64.exe](https://aka.ms/vs/16/release/vc_redist.x64.exe)
2. Download the timer zip and asc signature files from the releases folder here.
3. Place zip file anywhere you want
4. Verify signature (optional) * see how to below
5. Unzip
6. Double click the timer.exe

Don't forget you can right click the timer.exe to pin it to start menu, or taskbar, or put shortcut on your desktop.

Also, you can place a shortcut to timer.exe in your Start Menu with:

```
C:\Documents and Settings\<username>\Start Menu\Programs\Your Application
```

### GNU Linux:
1. You know what to do :) But if you don't contact me if you need a binary build for your distro and I'll gladly build one for you.

### Source Build:
1. GNU/Linux users: Easiest way is to open the pro file with QtCreator and run the build.  Be sure to set the build to 'release' so you your binary will be smaller.  This will auto build a shared library binary, so you only need the file 'timer' and put it anywhere on your system.  You'll probably be happiest if it's in your ~/bin directory.

### Bugs:
Report to the email address shown in section below.

## To verify the release signature:

Command line user can retrieve the key with:

```
    you@yourpc:~$ gpg --recv --keyserver hkp://keys.openpgp.org B608A617D6ED8A550BB3040582DDE559E5877342 
    gpg: key 82DDE559E5877342: public key "serverlinkdev <serverlinkdev@gmail.com>" imported
    gpg: Total number processed: 1
    gpg:               imported: 1

    you@yourpc:~$ gpg --fingerprint B608A617D6ED8A550BB3040582DDE559E5877342
    pub   rsa2048 2020-04-17 [SC]
          B608 A617 D6ED 8A55 0BB3  0405 82DD E559 E587 7342
    uid           [ unknown] serverlinkdev <serverlinkdev@gmail.com>
    sub   rsa2048 2020-04-17 [E]
```

Windows users can use: [Gpg4win.org](https://gpg4win.org/index.html) .  When fetching the key be sure to specify the keyserver:``` hkp://keys.openpgp.org```