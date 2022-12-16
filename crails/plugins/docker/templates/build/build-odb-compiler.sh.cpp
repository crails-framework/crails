#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_DockerBuildOdbCompiler : public Crails::Template
{
public:
  render_DockerBuildOdbCompiler(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    build2_fingerprint(Crails::cast<std::string>(vars, "build2_fingerprint",  "70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43"))
  {}

  void render()
  {
ecpp_stream << "#!/bin/sh -ex\n\nSYSTEM_GCC=`gcc --version | grep gcc | awk '{print $3}' | cut -d. -f1`\nif [ -z \"$ODB_COMPILER_GCC\" ] ; then\n  ODB_COMPILER_GCC=10\nfi\nFINGERPRINT=\"" << ( build2_fingerprint );
  ecpp_stream << "\"\n\napt-get -y install \\\n  gcc-$ODB_COMPILER_GCC \\\n  g++-$ODB_COMPILER_GCC \\\n  gcc-$ODB_COMPILER_GCC-plugin-dev\n\nbpkg create -d odb-gcc-$ODB_COMPILER_GCC cc \\\n  config.cxx=g++-$ODB_COMPILER_GCC \\\n  config.cc.coptions=-O3 \\\n  config.bin.rpath=/usr/local/lib \\\n  config.install.root=/usr/local\n\ncd odb-gcc-$ODB_COMPILER_GCC\n\nbpkg build odb@https://pkg.cppget.org/1/beta --yes --trust \"$FINGERPRINT\"\nbpkg install odb\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string build2_fingerprint;
};

void render_docker_build_odb_compiler(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_DockerBuildOdbCompiler(renderer, target, vars).render();
}