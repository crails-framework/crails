#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <vector>

class render_DockerBuildCrails : public Crails::Template
{
public:
  render_DockerBuildCrails(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    crails_version(Crails::cast< std::string >(vars, "crails_version",  "master")), 
    build2_fingerprint(Crails::cast< std::string >(vars, "build2_fingerprint")), 
    with_odb(Crails::cast< bool >(vars, "with_odb",  false)), 
    with_comet(Crails::cast< bool >(vars, "with_comet",  false)), 
    prebuild_patches( {"fix-broken-build2-packages"}), 
    postbuild_patches( {"fix-boost-pc"})
  {}

  void render()
  {
ecpp_stream << "#!/bin/bash -e\n\nif [ -z \"$CPP_COMPILER\" ] ; then\n  CPP_COMPILER=\"g++\"\nfi\n\nCRAILS_VERSION=" << ( crails_version );
  ecpp_stream << "\nCPPGET_FINGERPRINT=\"" << ( build2_fingerprint );
  ecpp_stream << "\"\nINSTALL_ROOT=/usr/local\nBUILD_DIR=\"build-$CPP_COMPILER\"\nBPKG_OPTS=\"--yes --sys-no-query\"\n\nsource ./build-environment.sh\n\n##\n## Build libraries\n##\necho \"+ creating package at $BUILD_DIR\"\nbpkg create -d \"$BUILD_DIR\" cc \\\n  config.cxx=$CPP_COMPILER \\\n  config.cc.poptions=-I/usr/local/include \\\n  config.cc.coptions=-O3 \\\n  config.bin.rpath=\"$INSTALL_ROOT/lib\" \\\n  config.install.root=\"$INSTALL_ROOT\"\n\ncd \"$BUILD_DIR\"\n\necho \"+ fetching dependencies\"\nbpkg add https://pkg.cppget.org/1/beta --trust \"$CPPGET_FINGERPRINT\"\nfor package in crails comet.cpp libcrails ${crails_packages[@]} ; do\n  bpkg add \"https://github.com/crails-framework/$package.git#$CRAILS_VERSION\"\ndone\nbpkg fetch --trust \"$CPPGET_FINGERPRINT\"\n";
 if (prebuild_patches.size() > 0){
  ecpp_stream << "\necho \"+ applying patches\"";
   for (const std::string& patch : prebuild_patches){
  ecpp_stream << "\nsh <(curl -s \"https://raw.githubusercontent.com/crails-framework/crails/master/" << ( patch );
  ecpp_stream << ".sh\")";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n\necho \"+ building core components\"\nbpkg build crails    $BPKG_OPTS ${system_packages[@]}\nbpkg build libcrails $BPKG_OPTS ${system_packages[@]}";
 if (with_comet){
  ecpp_stream << "\nbpkg build comet     $BPKG_OPTS ${system_packages[@]}";
 };
  ecpp_stream << "\n";
 if (with_odb){
  ecpp_stream << "\nif [ -z ${sql_backends[@]} ] ; then\n  echo \"+ no sql backend selected, skipping libcrails-odb\"\nelse\n  echo \"+ building libcrails-odb\"\n  config_file=\"libcrails-odb-2.0.0/build/config.build\"\n  bpkg build libodb $BPKG_OPTS\n  bpkg install libodb\n  for backend in ${sql_backends[@]} ; do\n    bpkg build libodb-$backend $BPKG_OPTS ${system_packages[@]}\n    bpkg install libodb-$backend\n  done\n  bpkg build libcrails-odb $BPKG_OPTS --configure-only ${system_packages[@]}\n  for backend in sqlite pgsql mysql oracle ; do\n    echo ${sql_backends[@]} | grep $backend \\\n      && echo \"config.libcrails_odb.with_$backend = true\"  >> $config_file \\\n      || echo \"config.libcrails_odb.with_$backend = false\" >> $config_file\n  done\n  bpkg build libcrails-odb ${system_packages[@]}\nfi";
 };
  ecpp_stream << "\n\nfor package in ${crails_packages[@]} ; do\n  echo \"+ building $package\"\n  bpkg build $package $BPKG_OPTS ${system_packages[@]}\ndone\n\necho \"+ installing at $INSTALL_ROOT\"\nbpkg install --all --recursive\n";
 if (postbuild_patches.size() > 0){
  ecpp_stream << "\necho \"+ applying patches\"";
   for (const std::string& patch : postbuild_patches){
  ecpp_stream << "\nsh <(curl -s \"https://raw.githubusercontent.com/crails-framework/crails/master/" << ( patch );
  ecpp_stream << ".sh\")";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string crails_version;
  std::string build2_fingerprint;
  bool with_odb;
  bool with_comet;
  std::vector<std::string> prebuild_patches;
  std::vector<std::string> postbuild_patches;
};

void render_docker_build_crails(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_DockerBuildCrails(renderer, target, vars).render();
}