# dazum

dazum is a zoom application that aims to be a nice zoom tool for any OS. Inspired by [boomer](https://github.com/tsoding/boomer) and made with [raylib](https://github.com/raysan5/raylib).

## Preview

[preview.webm](https://github.com/user-attachments/assets/470907d9-ae95-44d1-b0a6-733742cf12b5)

## Requirements

- git
- C compiler (clang, gcc, cc)
- make
- [raylib 5.0](https://github.com/raysan5/raylib)
- [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

On Linux (Debian/Ubuntu) running this should be enough:
```bash
sudo apt install git build-essential libx11-dev
```

## Installation

Clone the repository and build the program

```bash
git clone https://github.com/SrVariable/dazum
cd dazum
make
```

It will create `dazum`, you can run it with
```bash
./dazum
```

## Features

|Control|Description|
|-|-|
|<kbd>0</kbd>|Reset camera|
|<kbd>Mouse Button Left</kbd>|Drag camera|
|<kbd>esc</kbd> or <kbd>q</kbd>|Close dazum|
|<kbd>f</kbd>|Toggle flashlight mode|
|<kbd>Mouse Wheel Up</kbd>|Zoom in|
|<kbd>Mouse Wheel Down</kbd>|Zoom out|
|<kbd>Ctrl</kbd> + <kbd>Mouse Button Up</kbd>|Increase flashlight radius|
|<kbd>Ctrl</kbd> + <kbd>Mouse Button Down</kbd>|Decrease flashlight radius|

## Contributing

If you find any bug or have any suggestion, please open an issue [here](https://github.com/SrVariable/dazum/issues).
