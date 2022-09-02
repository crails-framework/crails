#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class DockerBuildCrails : public Crails::Template
{
public:
  DockerBuildCrails(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    crails_version(Crails::cast<std::string>(vars, "crails_version",  "master")), 
    build2_fingerprint(Crails::cast<std::string>(vars, "build2_fingerprint")), 
    with_odb(Crails::cast<bool>(vars, "with_odb",  false)), 
    with_comet(Crails::cast<bool>(vars, "with_comet",  false))
  {}

  std::string render()
  {
ecpp_stream << "#!/bin/bash -e\n\nCRAILS_VERSION=" << ( crails_version );
  ecpp_stream << "\nCOMPILER_VERSION=`gcc --version | grep gcc | awk '{print $3}' | cut -d. -f1`\nCPPGET_FINGERPRINT=\"" << ( build2_fingerprint );
  ecpp_stream << "\"\nINSTALL_ROOT=/usr/local\nBUILD_DIR=\"build-gcc-$COMPILER_VERSION\"\n\nsource ./build-environment.sh\n\n##\n## Build libraries\n##\necho \"+ creating package at $BUILD_DIR\"\nbpkg create -d \"$BUILD_DIR\" cc \\\n  config.cxx=g++ \\\n  config.cc.coptions=-O3 \\\n  config.bin.rpath=\"$INSTALL_ROOT/lib\" \\\n  config.install.root=\"$INSTALL_ROOT\"\n\ncd \"$BUILD_DIR\"\n\necho \"+ fetching dependencies\"\nbpkg add https://pkg.cppget.org/1/beta --trust \"$CPPGET_FINGERPRINT\"\nfor package in crails comet.cpp libcrails ${crails_packages[@]} ; do\n  bpkg add \"https://github.com/crails-framework/$package.git#$CRAILS_VERSION\"\ndone\nbpkg fetch --trust \"$CPPGET_FINGERPRINT\"\n\necho \"+ building core components\"\nbpkg build crails    --yes ${system_packages[@]}\nbpkg build libcrails --yes ${system_packages[@]}";
 if (with_comet){
  ecpp_stream << "\nbpkg build comet     --yes ${system_packages[@]}";
 };
  ecpp_stream << "\n";
 if (with_odb){
  ecpp_stream << "\nif [ -z ${sql_backends[@]} ] ; then\n  echo \"+ no sql backend selected, skipping libcrails-odb\"\nelse\n  echo \"+ building libcrails-odb\"\n  config_file=\"libcrails-odb-2.0.0/build/config.build\"\n  bpkg build libodb\n  bpkg build libcrails-odb --yes --configure-only ${system_packages[@]}\n  for backend in sqlite pgsql mysql oracle ; do\n    echo ${sql_backends[@]} | grep $backend \\\n      && echo \"config.libcrails_odb.with_$backend = true\"  >> $config_file \\\n      || echo \"config.libcrails_odb.with_$backend = false\" >> $config_file\n  done\n  bpkg install libodb\n  for backend in ${sql_backends[@]} ; do\n    bpkg build libodb-$backend ${system_packages[@]}\n    bpkg install libodb-$backend\n  done\n  bpkg build libcrails-odb ${system_packages[@]}\nfi";
 };
  ecpp_stream << "\n\nfor package in ${crails_packages[@]} ; do\n  echo \"+ building $package\"\n  bpkg build $package --yes ${system_packages[@]}\ndone\n\necho \"+ installing at $INSTALL_ROOT\"\nbpkg install --all --recursive\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string crails_version;
  std::string build2_fingerprint;
  bool with_odb;
  bool with_comet;
};

std::string render_docker_build_crails(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return DockerBuildCrails(renderer, vars).render();
}