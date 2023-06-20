# crails

Command line client for the Crails framework.

# How to install

Crails relies on the amazing `build2` build system. If you haven't installed it yet, the following lines will do the trick :
```
curl -sSfO https://download.build2.org/0.15.0/build2-install-0.15.0.sh
chmod +x build2-install-0.15.0.sh
sh build2-install-0.15.0.sh
```

And here's how to build crails from scratch:
```
bpkg create -d crails-build-gcc cc config.cxx=g++
cd crails-build-gcc
bpkg add 'https://github.com/crails-framework/crails.git#master'
bpkg fetch
bpkg build crails '?sys:libssl'
bpkg install --all --recursive config.install.root=/usr/local config.install.sudo=sudo
```

## Use system libraries
If you already have the Boost libraries on your system, you can also edit the build line as such:
```
bpkg build crails \
  '?sys:libboost-random' \
  '?sys:libboost-program-options' \
  '?sys:libboost-beast' \
  '?sys:libboost-thread' \
  '?sys:libboost-random' \
  '?sys:libboost-system' \
  '?sys:libssl'
```
