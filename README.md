# IntelligentQube
## Author
- Bradley Sullivan
- bradleysullivan@nevada.unr.edu
## Description
This game is a simple 2D clone of one of my favorite games from the PS1 era, Intelligent Qube. 
IQ is a puzzle game for the PlayStation 1 that was released in 1997 developed by G-Artists and published by Sony Computer Entertainment.

![alt text](https://github.com/Bradley-Sullivan/IntelligentQube/blob/main/blob/Intelligent_Qube.jpg?raw=true)

This program was written for the 2022 UNR 24-hr Hackathon competition.

The game is coded completely in C utilizing the Raylib framework.

## Tested Build/Development Environments
- Linux (Debian-based Distros)
    - gcc v11.2.0
    - Raylib v4.0.0
- Windows 10
    - mingw64 gcc v11.2.0
    - Raylib v4.0.0

## Build/Compile Instructions
- Windows 10
    - Download Source Files (Git or .zip Archive)
        - Clone the repository (Git)
            - Navigate to the directory where you want the game to reside
            - Execute 'git clone "https://"Git Username":"Git Auth Key"@github.com/Bradley-Sullivan/IntelligentQube.git' in CMD
        - Download and Extract .zip Archive from https://github.com/Bradley-Sullivan/IntelligentQube
    - Navigate to cloned or extracted code directory (most likely IntelligentQube/)
    - Execute '.\compile.ps1' in Windows CMD
- Linux (tested on Debian-based Distros)
    - Download Source Files (Git or .zip Archive)
        - Clone the repository (Git)
            - Navigate to the directory where you want the game to reside
            - Execute 'git clone "https://"Git Username":"Git Auth Key"@github.com/Bradley-Sullivan/IntelligentQube.git' in the Linux terminal
        - Download and Extract .zip Archive from https://github.com/Bradley-Sullivan/IntelligentQube
    - Navigate to cloned or extracted code directory
    - Execute 'chmod +x .\compileLinux.sh' to enable execution
    - Execute '.\compileLinux.sh' in Windows CMD

- Running the game
    - Linux
        - './iqLinux'
    - Windows 10
        - '.\iq'

## Game Rules
- Clear as many Normal and Advantage Qubes as you can without clearing any of the Forbidden Qubes.
    - Clearing a Forbidden Qube results in the loss of a row from the Stage.
    - Trapping/Clearing an Advantage Qube will mark its location on the Stage. It can later be detonated to clear a 3x3 grid of surrounding qubes.
- Avoid getting crushed/hit by the advancing Qubes. 
    - Getting crushed will reset the level
- If you let too many Normal or Advantage Qubes (target Qubes) fall off the edge of the stage, you will lose one or multiple rows (depending on how many target Qubes you miss).
- If you happen to stand on the last row as it gets removed (either from clearing a Forbidden Qube or letting too many target Qubes fall off) it's GAME OVER.
- Each time you clear one round of Qubes, more will begin rolling down the Stage. If the Stage become empty of Raised Qubes (inactive Qubes), you will advance to the next Stage.
    - There are 7 stages.

## Scoring
- Manually clearing a Qube (Normal, Advantage, or even Forbidden) will net you 100 points.
- For every Qube cleared by an Advantage Qube detonation, you will net 300 points.
    - This includes the detonated Advantage Qube itself as well as any cleared Forbidden Qubes.
- At the end of each stage, you will gain 1000 points for each remaining Stage row.
    - If you are able to clear a round of Qubes without losing a single row (a perfect level clear), you will gain 10,000 points.

## Controls
- Use the ARROW KEYS to move your the character.
- Press SPACE to set a Qube trap.
    - Pressing SPACE whilst a trap is set will activate the trap, clearing whatever Qube is on top of the trap.
- Press LEFT SHIFT to detonate all trapped Advantage Qubes.
- Press LEFT CONTROL to rapidly advance remaining Qubes.
- Press P to pause the game.
- Press ESCAPE to exit the game entirely and return to desktop.

## Game Preview/Comparison
![alt text](https://github.com/Bradley-Sullivan/IntelligentQube/blob/main/blob/IQ_Gameplay.png?raw=true)

![alt text](https://github.com/Bradley-Sullivan/IntelligentQube/blob/main/blob/IQ_Clone_Gameplay.png?raw=true)
