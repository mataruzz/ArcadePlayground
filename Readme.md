# Arcade Platform Game

Welcome to the Arcade Platform! This project is built using Qt and C++. The idea of this project was mainly for me to learn the Qt library and understand its working principles. The platform hosts a variety of classic games, with the initial release featuring Tic Tac Toe and Tetris. Might include future releases including more exciting games.


## Install from source
In case you want to build and use this project on your own machine (instead of simple playing downloading the executable), you have first to install [Qt6.7](https://doc.qt.io/qt-6/get-and-install-qt.html). Once all the environment has been set.

1. **Clone the Repository**
   ```bash
   git clone https://github.com/mataruzz/ArcadePlayground.git
   cd ArcadePlayground

2. **Build the project with qmake**
    ```bash
    mkdir build && cd build
    export PATH=/path/to/your/Qt/bin:/$PATH # might not be needed
    qmake ../arcade_playground.pro -config release
    make

### Running the project
Run the executable we built in the previous step 

```
./arcade_playground
```

### Application Overview
<p align="center">
    <img src="https://github.com/mataruzz/ArcadePlayground/blob/main/Images/TicTacToe/Samples/TicTacToe.gif" height="260">
    <img src="https://github.com/mataruzz/ArcadePlayground/blob/main/Images/Tetris/Samples/Tetris.gif" height="260">
</p>

### Creating the AppImage
As reference on how the AppImage has been obtained, I used the tool [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy) with the attached [qt plugin](https://github.com/linuxdeploy/linuxdeploy-plugin-qt). Download the 2 AppImages and run:

```
./linuxdeploy-x86_64.AppImage --appdir AppDir --executable ./path/to/your/executable --output appimage --plugin qt
```



