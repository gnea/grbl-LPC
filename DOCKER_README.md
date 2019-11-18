# Arm Embedded Docker Environment
Minimal docker environment for building arm-embedded projects

## Usage
Run interactively with ```docker run -it --rm -v `pwd`:/root ryankurte/docker-arm-embedded /bin/bash```.  
This will create a temporal instance (changes will be dropped on exit) with a binding from the current directory to the root user home directory.  

If you are using selinux you may need to add a ```:z``` to the end of the -v parameter to indicate the mount is shared.

```docker run -it --rm -v `pwd`:/root:z ryankurte/docker-arm-embedded /bin/bash```

https://docs.docker.com/storage/bind-mounts/

### Building grbl-LPC
Update the grbl/config.h and uncomment the CPU_MAP define statement for the board you are compiling for

```
git submodule init
git submodule update
make
```

The resulting firmware file is build/firmware.bin

## Includes:
 - build-essential (native)
 - make, cmake
 - gawk, genromfs, ccache
 - arm-none-eabi from [launchpad.net](https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded)
 - [Yotta](http://yotta.mbed.com/)

 
