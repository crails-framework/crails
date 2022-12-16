#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_DockerBuildComet : public Crails::Template
{
public:
  render_DockerBuildComet(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#!/bin/bash -e\n\nCRAILS_VERSION=master\nCHEERP_PATH=/opt/cheerp\nCOMPILER_VERSION=`$CHEERP_PATH/bin/clang++ --version | grep Cheerp | cut -d' ' -f5`\nINSTALL_ROOT=/usr/local\nBUILD_DIR=\"build-cheerp-$COMPILER_VERSION\"\n\nsource ./build-environment.sh\n\necho \"+ creating package at $BUILD_DIR\"\nbpkg create -d \"$BUILD_DIR\" cc \\\n  config.cxx=\"$CHEERP_PATH/bin/clang++\" \\\n  config.cxx.poptions=\"-target cheerp-genericjs -fexceptions -D__CHEERP_CLIENT__ -I/usr/local/include\" \\\n  config.cxx.loptions=\"-target cheerp-genericjs\" \\\n  config.c{,xx}.version=$COMPILER_VERSION \\\n  config.install.root=$INSTALL_ROOT \\\n  config.install.lib=exec_root/lib/genericjs\n\ncd \"$BUILD_DIR\"\n\necho \"+ fetching dependencies\"\nfor package in ${comet_packages[@]} ; do\n  bpkg add \"https://github.com/crails-framework/$package.git#$CRAILS_VERSION\"\ndone\nbpkg fetch\n\nfor package in ${comet_packages[@]} ; do\n  echo \"+ building $package\"\n  bpkg build $package --yes\n  echo \"+ installing $package at $INSTALL_ROOT\"\n  bpkg install $package\ndone\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_docker_build_comet(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_DockerBuildComet(renderer, target, vars).render();
}