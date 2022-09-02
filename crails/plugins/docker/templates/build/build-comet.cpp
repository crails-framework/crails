#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class DockerBuildComet : public Crails::Template
{
public:
  DockerBuildComet(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#!/bin/bash -e\n\nCRAILS_VERSION=master\nCHEERP_PATH=/opt/cheerp\nCOMPILER_VERSION=`$CHEERP_PATH/bin/clang++ --version | grep Cheerp | cut -d' ' -f5`\nINSTALL_ROOT=/usr/local\nBUILD_DIR=\"build-cheerp-$COMPILER_VERSION\"\n\nsource ./build-environment.sh\n\necho \"+ creating package at $BUILD_DIR\"\nbpkg create -d \"$BUILD_DIR\" cc \\\n  config.cxx=\"$CHEERP_PATH/bin/clang++\" \\\n  config.cxx.poptions=\"-target cheerp-genericjs -D__CHEERP_CLIENT__ -I/usr/local/include\" \\\n  config.cxx.loptions=\"-target cheerp-genericjs\" \\\n  config.c{,xx}.version=$COMPILER_VERSION \\\n  config.install.root=$INSTALL_ROOT \\\n  config.install.lib=exec_root/lib/genericjs\n\ncd \"$BUILD_DIR\"\n\necho \"+ fetching dependencies\"\nfor package in ${comet_packages[@]} ; do\n  bpkg add \"https://github.com/crails-framework/$package.git#$CRAILS_VERSION\"\ndone\nbpkg fetch\n\nfor package in ${comet_packages[@]} ; do\n  echo \"+ building $package\"\n  bpkg build $package --yes\n  echo \"+ installing $package at $INSTALL_ROOT\"\n  bpkg install $package\ndone\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_docker_build_comet(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return DockerBuildComet(renderer, vars).render();
}