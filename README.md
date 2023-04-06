# Garden_Defender   

![Garden Defender Logo](https://user-images.githubusercontent.com/56697327/229754456-5e34f1b4-b3b8-435d-b763-7635a71a4984.png)

# Description
![WhatsApp Image 2023-04-06 at 18 43 52](https://user-images.githubusercontent.com/71940725/230478254-b407497c-fc81-4b7d-be0a-f5f599985f4d.jpg)

Protect your garden from unwanted animals with the garden defender. The garden defender is a robotic water pellet gun that deters unwanted animals by shooting at them with a water pellet. The robotic gun will continuously scan your garden with a webcam. With the combination of computer vision and a deep learning algorithm “YOLO”, the garden defender can detect and distinguish between a “friendly” and “unwanted guest” before shooting a water pellet. 

# Details

In most cases, a fence or a garden wall is not enough to prevent unwanted animals visiting your garden. This is particular true for gardeners who like to grow vegetables. Animals such rats and birds might see your vegetables as free food. Or your neighbourhood cat might have taken a liking to your lawn as its personal toilet. 

One solution to deter these “unwanted guest” is by shooting at them with a water gel pellet. The garden defender has a webcam that continuously scans your garden. When an “unwanted guest is detected”, the garden defender with aim and shoot. 

## Features

- Detect unwanted guests: OpenCV and a deep learning algorithm “YOLO” was used for object detection. 
    
- Aim and shoot: When an unwanted guest is detected will aim and shoot
    
- Garden surveillance - large field of view: Robot will continuously move web cam left and right to maximise the field of view when an unwanted guest not detected.
    
Please, watch our video:

[![Garden_Defender](https://img.youtube.com/vi/-97-if4tLNQ/0.jpg)](https://www.youtube.com/watch?v=-97-if4tLNQ)

# Documentation
For code documentation go to: 
https://ernestoflores27.github.io/Garden_Defender/html/index.html

# Build
For full building instructions go to:
[Full build guide](https://maeldorne.notion.site/Garden-Defender-f2392ed58dae446e874e89c7e9c0607c)

# Installation
**Required packages:**

- Raspberry Pi OS (previously called Raspbian): [https://www.raspberrypi.com/software/](https://www.raspberrypi.com/software/)
- Cmake: [https://cmake.org/download/](https://cmake.org/download/)
- Pigpio: [https://kalitut.com/control-gpio-pigpiod-commands/](https://kalitut.com/control-gpio-pigpiod-commands/)
- PCA9685: [https://www.adafruit.com/product/815](https://www.adafruit.com/product/815)
- OpenCV for C++ with DNN support: [https://opencv.org/](https://opencv.org/)

**Installing Raspberry Pi OS:**

1. Download package.
2. Install Raspberry Pi OS on your PC.
3. Insert SD card into your PC.
4. Open Raspberry Pi Imager.
5. In Operating system select: Raspberry Pi OS (32-bit)
6. In Storage select your SD card.
7. Select setting and add your WIFI name (SSID) and password.
8. Finally, click on Write.

**Installing Cmake on RPI:**

1.-Open terminal.

2.-Installation:

```bash
cd
sudo apt-get install cmake
```

3.-Verification:

```bash
cmake --version
```

**Installing Pigpio on RPI:**

1.-Open terminal.

2.-Installation:

```bash
cd
sudo apt-get install pigpio
```

3.- Verification:

```bash
pigpio --version
```

**Installing PCA9685 on RPI:**

1.Follow the installation as shown here: [https://github.com/vanvught/rpidmx512/tree/master/lib-pca9685](https://github.com/vanvught/rpidmx512/tree/master/lib-pca9685)

**Installing OpenCV on RPI:**

1.-Open terminal

2.-Download:

```bash
wget https://github.com/sol-prog/raspberry-pi-opencv/releases/download/opencv4rpi2.1/opencv-4.1.0-armhf.tar.bz2
```

3.-Extract:

```bash
tar xvf opencv-4.1.0-armhf.tar.bz2
```

4.-Move folder:

```bash
sudo mv opencv-4.1.0 /opt
```

5.-Remove folder:

```bash
rm opencv-4.1.0-armhf.tar.bz2
```

6.-Install requisites:

```bash
sudo apt install libtiff-dev zlib1g-dev
sudo apt install libjpeg-dev libpng-dev
sudo apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libxvidcore-dev libx264-dev
```

7.-Add OpenCV to the system library path:

```bash
cd ~
echo 'export LD_LIBRARY_PATH=/opt/opencv-4.1.0/lib:$LD_LIBRARY_PATH' >> .bashrc. .bashrc
```
8.-Clone config file:
```bash
cd ~
echo 'export LD_LIBRARY_PATH=/opt/opencv-4.1.0/lib:$LD_LIBRARY_PATH' >> .bashrc. .bashrc
```

Clone this repo using 
```bash
git clone https://github.com/Ernestoflores27/Garden_Defender.git
```
Build using cmake
```bash
cd Garden_Defender
make
```

# Run
sudo Garden_Defender/build/garden_defender

# Social Media
[<img src="https://camo.githubusercontent.com/451c9e00b3f8c9b6c15d4708d9c6293766e9766345a206114a135e4a597847b6/68747470733a2f2f7265732e636c6f7564696e6172792e636f6d2f6478737479337374362f696d6167652f75706c6f61642f76313634393339353835392f626c696e642d6e61762d73797374656d2f69636f6e73382d696e7374616772616d2d34385f6b356b7577692e706e67">](https://www.instagram.com/gardendefender_info/)
[<img src="https://camo.githubusercontent.com/e1aebb8e5b3c1fcaf7c9372d909c10dc1fe81b63f88c786ae15e78b65bd961dd/68747470733a2f2f7265732e636c6f7564696e6172792e636f6d2f6478737479337374362f696d6167652f75706c6f61642f76313634393339353835392f626c696e642d6e61762d73797374656d2f69636f6e73382d796f75747562652d34385f6b70616c6c392e706e67">](https://www.youtube.com/@gardendefender)
[<img src="https://camo.githubusercontent.com/25a57ba7684d613af975d1270cc992490103b9472e8ffaa85a28b10925d9ba7e/68747470733a2f2f7265732e636c6f7564696e6172792e636f6d2f6478737479337374362f696d6167652f75706c6f61642f76313634393339353835392f626c696e642d6e61762d73797374656d2f69636f6e73382d747769747465722d34385f70743469637a2e706e67">](https://twitter.com/garden_defender)
[<img src="https://user-images.githubusercontent.com/71940725/229294685-eee9c9a3-4d63-4785-b5a5-9edc586ef4b6.png">](https://www.tiktok.com/@gardendefenderinfo)
[<img src="https://user-images.githubusercontent.com/56697327/229308058-92dbd6a5-40c9-4143-af38-86583202f425.png">](https://www.facebook.com/profile.php?id=100089644931372)
