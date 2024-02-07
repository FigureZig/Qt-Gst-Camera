
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="qdarkstyle/icon.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Qt-Gst-Camera</h3>

  <p align="center">
    Simple pet-project
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

[![example][product-example]]

This project is a Qt-based application for video processing that integrates with GStreamer for video capture and manipulation. Designed for [АО "НКБ ВС"](https://nkbvs.ru/)


### Built With

* [![c++][c++-shield]][c++-url]
* [![Qt][qt-shield]][qt-url]
* [![Gstreamer][gstreamer-shield]][gstreamer-url]

<!-- GETTING STARTED -->
## Getting Started

To run program, follod these simple steps:

### Prerequisites

To build and run program, you'll need to install:
* Qt 6.6.0
* GStreamer

### Installation

For install programm: 

1. Clone the repo
   ```sh
   git clone https://github.com/FigureZig/Qt-Gst-Camera.git
   ```
2. In gstreamerhandler.cpp change device-index on desired one
   ```c++
   const char* pipelineStr = "mfvideosrc device-index=YOUR_NUBMER ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink";
   ```

<!--BUILD-->
## Build

To build the project you need:

1. Change Cache Variable in build.bat
  ```sh
  cmake ../. -B .build_msvc -DQt6_DIR="YOUR_DESTANATION"
  ```
2. Run configure.bat
3. Run build.bat

<!--MARKDOWN-->
[linkedin-shield]: https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white
[linkedin-url]: https://www.linkedin.com/in/figurezig

[qt-shield]: https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white
[qt-url]: https://www.qt.io/

[c++-shield]:https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white
[c++-url]: https://isocpp.org/

[gstreamer-shield]: https://img.shields.io/static/v1?style=for-the-badge&message=GStreamer&color=FF3131&logo=GStreamer&logoColor=FFFFFF&label=
[gstreamer-url]: https://gstreamer.freedesktop.org/

[product-logo]: qdarkstyle/icon.png
[product-example]: Example.png
