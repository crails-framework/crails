#!/bin/bash -e

DEFAULT_CRAILS_VERSION=master
DEFAULT_COMPILER=clang++
DEFAULT_INSTALL_ROOT=/usr/local
CPPGET_FINGERPRINT="70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43"

echo ""
echo "  ██████╗██████╗  █████╗ ██╗██╗     ███████╗"
echo " ██╔════╝██╔══██╗██╔══██╗██║██║     ██╔════╝"
echo " ██║     ██████╔╝███████║██║██║     ███████╗"
echo " ██║     ██╔══██╗██╔══██║██║██║     ╚════██║"
echo " ╚██████╗██║  ██║██║  ██║██║███████╗███████║"
echo "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝╚══════╝"
echo ""

if [ -z "$CRAILS_VERSION" ] ; then
  echo -n "> Which version of crails would you like to install (default: $DEFAULT_CRAILS_VERSION): "
  read CRAILS_VERSION
  if [ -z "$CRAILS_VERSION" ] ; then CRAILS_VERSION="$DEFAULT_CRAILS_VERSION" ; fi
fi

if [ -z "$COMPILER" ] ; then
  echo -n "> Which compiler do you wish to use (default: $DEFAULT_COMPILER): "
  read COMPILER
  if [ -z "$COMPILER" ] ; then COMPILER="$DEFAULT_COMPILER" ; fi
fi

if [ -z "$sql_backends" ] ; then
  echo "(i) Available database backends are: sqlite pgsql mysql oracle"
  echo -n "> Which database backend(s) do you want to use (separated with spaces): "
  read sql_backends
fi

if [ ! -z "$sql_backends" ] ; then
  if [ -z "$WITH_ODB_COMPILER" ] && ! which odb > /dev/null ; then
    echo "(i) The ODB compiler is needed for SQL support. This installer will build ODB unless told otherwise."
    if ! find /usr/lib/gcc -name gcc-plugin.h | grep gcc-plugin.h > /dev/null ; then
      echo '(!) gcc-plugin.h was not found. If you want to build the ODB compiler, make sure you have the gcc-plugin development package installed in your system.'
    else
      echo '(i) gcc-plugin.h found, the ODB compiler should build successfully.'
    fi
    echo -n "> Build the ODB compiler (y/n): "
    read WITH_ODB_COMPILER
    if [ -z "$WITH_ODB_COMPILER" ] ; then WITH_ODB_COMPILER="y" ; fi
  fi
fi

if [ -z "$WITH_COMET" ] ; then
  echo -n "> Build comet.cpp (https://github.com/crails-framework/comet.cpp) (y/n): "
  read WITH_COMET
fi

if [ -z "$use_system_libraries" ] ; then
  echo -n "> Should we use system libraries when available (y/n): "
  read use_system_libraries
  if [ -z "$use_system_libraries" ] ; then use_system_libraries="n" ; fi
fi

echo -n "> Install path (default: $DEFAULT_INSTALL_ROOT): "
read INSTALL_ROOT
if [ -z "$INSTALL_ROOT" ] ; then INSTALL_ROOT="$DEFAULT_INSTALL_ROOT" ; fi

COMPILER_VERSION=`$COMPILER --version | sed -n 1p | awk '{print $3}' | cut -d. -f1`
BUILD_DIR="build-$COMPILER-$COMPILER_VERSION"

crails_packages=(
  libcrails-templates
  libcrails-action
  libcrails-controllers
  libcrails-crud
  libcrails-cookies
  libcrails-form-parser
  libcrails-multipart-parser
  libcrails-url-parser
  libcrails-xml-parser
  libcrails-json-parser
  libcrails-json-views
  libcrails-html-views
  libcrails-databases
  libcrails-http-client
  libcrails-mail
  libcrails-signin
  libcrails-sync
  libcrails-xmlrpc
  libcrails-proxy
  libcrails-tests
)

system_packages=()

if [ "$use_system_libraries" = "y" ] ; then
  system_packages+=(
    ?sys:libboost-any
    ?sys:libboost-asio
    ?sys:libboost-beast
    ?sys:libboost-config
    ?sys:libboost-filesystem
    ?sys:libboost-fusion
    ?sys:libboost-json
    ?sys:libboost-lexical-cast
    ?sys:libboost-program-options
    ?sys:libboost-property-tree
    ?sys:libboost-process
    ?sys:libboost-random
    ?sys:libssl
  )
  for backend in $sql_backends ; do
    case $backend in
      sqlite)
        system_packages+=(?sys:libsqlite3)
        ;;
      pgsql)
        system_packages+=(?sys:libpq)
        ;;
      mysql)
        system_packages+=(?sys:libmysqlclient)
        ;;
    esac
  done
  echo "+ Using system packages: ${system_packages[@]}"
fi

if [ ! -z "$sql_backends" ] ; then
  crails_packages+=(libcrails-odb)
fi

##
## Build libraries
##
echo "+ creating package at $BUILD_DIR"

if [ -d "$BUILD_DIR" ] ; then
  echo "++ removing previously existing package at $BUILD_DIR"
  rm -rf "$BUILD_DIR"
fi

bpkg create -d "$BUILD_DIR" cc \
  config.cxx=$COMPILER \
  config.cc.coptions=-O3 \
  config.bin.rpath="$INSTALL_ROOT/lib"

cd "$BUILD_DIR"

echo "+ fetching dependencies"
bpkg add https://pkg.cppget.org/1/beta --trust "$CPPGET_FINGERPRINT"
for package in crails comet.cpp libcrails ${crails_packages[@]} ; do
  bpkg add "https://github.com/crails-framework/$package.git#$CRAILS_VERSION"
done
bpkg fetch --trust "$CPPGET_FINGERPRINT"

echo "+ building core components"
bpkg build crails    --yes ${system_packages[@]}
bpkg build libcrails --yes ${system_packages[@]}

if [ "$WITH_COMET" = "y" ] ; then
  bpkg build comet   --yes ${system_packages[@]}
fi

if [ -z ${sql_backends} ] ; then
  echo "+ no sql backend selected, skipping libcrails-odb"
else
  echo "+ building libcrails-odb"
  config_file="libcrails-odb-2.0.0/build/config.build"
  bpkg build libodb
  bpkg build libcrails-odb --yes --configure-only ${system_packages[@]}
  for backend in sqlite pgsql mysql oracle ; do
    echo ${sql_backends} | grep $backend \
      && echo "config.libcrails_odb.with_$backend = true"  >> $config_file \
      || echo "config.libcrails_odb.with_$backend = false" >> $config_file
  done
  for backend in ${sql_backends} ; do
    bpkg build libodb-$backend ${system_packages[@]}
  done
  bpkg build libcrails-odb ${system_packages[@]}
fi

for package in ${crails_packages[@]} ; do
  echo "+ building $package"
  bpkg build $package --yes ${system_packages[@]}
done

if [ -z "$install_confirmed" ] ; then
  echo -n "> Install to $INSTALL_ROOT (y/n): "
  read install_confirmed
fi
  
if [ "$install_confirmed" = "y" ] ; then
  bpkg install --all --recursive \
    config.install.root="$INSTALL_ROOT" \
    config.install.sudo=sudo
fi

##
## Build ODB compiler
##
if [ "$WITH_ODB_COMPILER" = "y" ] ; then
  cd ..
  bpkg create -d odb-gcc cc \
    config.cxx=g++ \
    config.cc.coptions=-O3 \
    config.bin.rpath="$INSTALL_ROOT/lib" \
    config.install.root="$INSTALL_ROOT"  \
    config.install.sudo=sudo
  cd odb-gcc
  bpkg build odb@https://pkg.cppget.org/1/beta --yes --trust "$CPPGET_FINGERPRINT"
  bpkg install odb
fi

##
## Build Comet.cpp
##
if [ "$WITH_COMET" = "y" ] ; then
  cd ..
  bash <(curl -s "https://raw.githubusercontent.com/crails-framework/comet.cpp/master/install.sh")
fi
