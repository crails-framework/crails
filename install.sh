#!/usr/bin/env -S bash -e

DEFAULT_CRAILS_VERSION=master
DEFAULT_COMPILER=none
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
  if   which clang++ > /dev/null 2>&1 ; then export DEFAULT_COMPILER=clang++
  elif which g++     > /dev/null 2>&1 ; then export DEFAULT_COMPILER=g++
  fi
  echo -n "> Which compiler do you wish to use (default: $DEFAULT_COMPILER): "
  read COMPILER
  if [ -z "$COMPILER" ] ; then COMPILER="$DEFAULT_COMPILER" ; fi
  if [ "$COMPILER" = "none" ] ; then
    echo "(!) C++ compiler not found."
    exit -1
  fi
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

touch "$INSTALL_ROOT/toto" && rm "$INSTALL_ROOT/toto" && USE_SUDO=n || USE_SUDO=y
if [ "$USE_SUDO" = "y" ] ; then
  SUDO_OPTION="config.install.sudo=sudo"
fi

COMPILER_VERSION=`$COMPILER --version | sed -n 1p | awk 'match($0, /[0-9]+.[0-9]+.[0-9]+/) { print substr($0, RSTART, RLENGTH) }' | cut -d. -f1`
BUILD_DIR="build-$COMPILER-$COMPILER_VERSION"

crails_packages=(
  libcrails-action
  libcrails-archive
  libcrails-attachment
  libcrails-cli
  libcrails-controllers
  libcrails-crud
  libcrails-cookies
  libcrails-design-patterns
  libcrails-encrypt
  libcrails-fonticons
  libcrails-form-parser
  libcrails-i18n
  libcrails-multipart-parser
  libcrails-url-parser
  libcrails-xml-parser
  libcrails-json-parser
  libcrails-json-views
  libcrails-html-views
  libcrails-databases
  libcrails-http-client
  libcrails-mail
  libcrails-oauth
  libcrails-paginator
  libcrails-selenium
  libcrails-sentry
  libcrails-sidekix
  libcrails-signin
  libcrails-sync
  libcrails-xmlrpc
  libcrails-proxy
  libcrails-redis
  libcrails-tests
  libcrails-templates
)

system_packages=()
boost_packages=(
  ?sys:libboost-any
  ?sys:libboost-asio
  ?sys:libboost-beast
  ?sys:libboost-config
  ?sys:libboost-date-time
  ?sys:libboost-filesystem
  ?sys:libboost-fusion
  ?sys:libboost-json
  ?sys:libboost-lexical-cast
  ?sys:libboost-lockfree
  ?sys:libboost-program-options
  ?sys:libboost-property-tree
  ?sys:libboost-process
  ?sys:libboost-random
)

if [ "$use_system_libraries" = "y" ] ; then
  system_packages+=(
    ${boost_packages[@]}
    ?sys:libssl
    ?sys:libcrypto
  )
  for backend in $sql_backends ; do
    case $backend in
      sqlite)
        system_packages+=(?sys:libsqlite3)
        ;;
      pgsql)
        #system_packages+=(?sys:libpq)
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
## Lookup optional dependencies which are not available in cppget repositories
##
if [ -f /usr/include/libssh/libssh.h ] || [ -f /usr/local/include/libssh/libssh.h ] ; then
  echo "+ Detected libssh: adding libcrails-ssh and crails-deploy"
  crails_packages+=(libcrails-ssh)
  export WITH_CRAILS_DEPLOY="y"
fi

if pkg-config Magick++ ; then
  echo "+ Detected Magick++: adding libcrails-image"
  crails_packages+=(libcrails-image)
  system_packages+=(?sys:ImageMagick++/*)
fi

if [ -d /usr/include/mongocxx/v_noabi ] || [ -d /usr/local/mongocxx/include/v_noabi ] ; then
  echo "+ Detected mongocxx"
  if [ -d /usr/include/bsoncxx/v_noabi ] || [ -d /usr/local/bsoncxx/include/v_noabi ] ; then
    echo "+ Detected bsoncxx: adding libcrails-mongodb"
    crails_packages+=(libcrails-mongodb)
  fi
fi

##
## Prepare build2
##
if ! which bpkg ; then
  echo "+ bpkg does not appear to be installed. Installing build2:"
  BUILD2_VERSION="0.15.0"
  curl -sSfO https://download.build2.org/$BUILD2_VERSION/build2-install-$BUILD2_VERSION.sh
  chmod +x build2-install-$BUILD2_VERSION.sh
  sh build2-install-$BUILD2_VERSION.sh
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
  config.bin.rpath="$INSTALL_ROOT/lib" \
  config.install.root="$INSTALL_ROOT"  \
  $SUDO_OPTION

cd "$BUILD_DIR"

if [ ! "$use_system_libraries" = "y" ] ; then
  echo "+ applying patches"
  sh <(curl -s "https://raw.githubusercontent.com/crails-framework/crails/master/fix-boost-property-tree.sh")
  sh <(curl -s "https://raw.githubusercontent.com/crails-framework/crails/master/fix-broken-build2-packages.sh")
fi

echo "+ fetching dependencies"
bpkg add https://pkg.cppget.org/1/beta --trust "$CPPGET_FINGERPRINT"
for package in crails crails-deploy comet.cpp libcrails ${crails_packages[@]} ; do
  bpkg add "https://github.com/crails-framework/$package.git#$CRAILS_VERSION"
done
bpkg fetch --trust "$CPPGET_FINGERPRINT"

if [ "$use_system_libraries" = "y" ] ; then
  echo "+ fixing libboost-process not being properly imported when using system libraries"
  if ! bpkg build libcrails-cli --yes --configure-only ${system_packages[@]} ; then
    echo "(i) libcrails-cli failed to configure (just according to keikaku)"
    tail -n +2 libcrails-cli-2.0.0/libcrails-cli/buildfile > /tmp/tmpfile
    mv /tmp/tmpfile libcrails-cli-2.0.0/libcrails-cli/buildfile
  else
    echo "(!) libcrails-cli configuration was supposed to fail, but it didn't."
  fi
fi

if [ ! "$use_system_libraries" = "y" ] ; then
  # Patch compiling issue with boost 1.78 and 1.79, see https://github.com/boostorg/process/issues/235
  bpkg build libboost-process --configure-only --yes || echo "failed with success"
  version=`ls -d libboost-* | sed -n 1p | awk 'match($0, /[0-9]+.[0-9]+.[0-9]+/) { print substr($0, RSTART, RLENGTH) }'`
  if [ $version = "1.78.0" ] || [ $version = "1.79.0" ] ; then
    monkeypatch_target="libboost-process-$version/include/boost/process/detail/posix/executor.hpp"
    sed '156s/len + 1/static_cast<int>(len + 1)/' "$monkeypatch_target" > monkeypatch
    mv monkeypatch "$monkeypatch_target"
  fi
fi

echo "+ building core components"
bpkg build crails    --yes ${system_packages[@]}
bpkg build libcrails --yes ${system_packages[@]}

if [ "$WITH_CRAILS_DEPLOY" = "y" ] ; then
  bpkg build crails-deploy --yes ${system_packages[@]}
fi

if [ "$WITH_COMET" = "y" ] ; then
  bpkg build comet   --yes ${system_packages[@]}
fi

if [ -z ${sql_backends} ] ; then
  echo "+ no sql backend selected, skipping libcrails-odb"
else
  echo "+ building libcrails-odb"
  config_file="libcrails-odb-2.0.0/build/config.build"
  build_file="libcrails-odb-2.0.0/libcrails-odb/buildfile"
  bpkg build libodb --yes
  bpkg build libcrails-odb --yes --configure-only ${system_packages[@]}
  for backend in sqlite pgsql mysql oracle ; do
    if echo ${sql_backends} | grep $backend ; then
      echo "config.libcrails_odb.with_$backend = true"  >> $config_file
      awk '!'"/with_$backend/" "$build_file" > .tmpfile
      mv .tmpfile "$build_file"
    else
      echo "config.libcrails_odb.with_$backend = false" >> $config_file
    fi
  done
  for backend in ${sql_backends} ; do
    bpkg build libodb-$backend --yes ${system_packages[@]}
  done
  bpkg build libcrails-odb --yes ${system_packages[@]}
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
  bpkg install --all --recursive

  # patch pkgconfig files containing invalid linking options
  patch_pc_script="fix-boost-pc.sh"
  if [ -z "$SUDO_OPTION" ] ; then
    sh <(curl -s "https://raw.githubusercontent.com/crails-framework/crails/master/$patch_pc_script")
  else
    curl -sSO "https://raw.githubusercontent.com/crails-framework/crails/master/$patch_pc_script"
    chmod +x "$patch_pc_script"
    sudo "./$patch_pc_script"
    rm "$patch_pc_script"
  fi
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
    $SUDO_OPTION
  cd odb-gcc
  bpkg build odb@https://pkg.cppget.org/1/beta --yes --trust "$CPPGET_FINGERPRINT"
  bpkg install odb
fi

##
## Build Comet.cpp
##
if [ "$WITH_COMET" = "y" ] ; then
  cd ..
  export INSTALL_CLI="n"
  bash <(curl -s "https://raw.githubusercontent.com/crails-framework/comet.cpp/master/install.sh")
fi
