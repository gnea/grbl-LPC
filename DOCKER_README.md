# Arm Embedded Docker Environment
Minimal docker environment for building arm-embedded projects

## Usage
Run interactively with ```docker run -it --rm -v `pwd`:/root ryankurte/docker-arm-embedded /bin/bash```.  
This will create a temporal instance (changes will be dropped on exit) with a binding from the current directory to the root user home directory.  

## Includes:
 - build-essential (native)
 - make, cmake
 - gawk, genromfs, ccache
 - arm-none-eabi from [launchpad.net](https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded)
 - [Yotta](http://yotta.mbed.com/)
 