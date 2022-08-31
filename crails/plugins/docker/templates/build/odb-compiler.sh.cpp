#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class DockerOdbCompiler : public Crails::Template
{
public:
  DockerOdbCompiler(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    build2_fingerprint(Crails::cast<std::string>(vars, "build2_fingerprint",  "70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43"))
  {}

  std::string render()
  {
ecpp_stream << "#!/bin/sh -ex\n\nSYSTEM_GCC=`gcc --version | grep gcc | awk '{print $3}' | cut -d. -f1`\nODB_COMPILER_GCC=10\nFINGERPRINT=\"" << ( build2_fingerprint );
  ecpp_stream << "\"\n\napt-get -y install \\n  gcc-$ODB_COMPILER_GCC \\n  g++-$ODB_COMPILER_GCC \\n  gcc-$ODB_COMPILER_GCC-plugin-dev\n\nbpkg create -d odb-gcc-$ODB_COMPILER_GCC cc \\n  config.cxx=g++-$ODB_COMPILER_GCC \\n  config.cc.coptions=-O3 \\n  config.bin.rpath=/usr/local/lib \\n  config.install.root=/usr/local\n\ncd odb-gcc-$ODB_COMPILER_GCC\n\nbpkg build odb@https://pkg.cppget.org/1/beta --yes --trust \"$FINGERPRINT\"\nbpkg install odb\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string build2_fingerprint;
};

std::string render_docker_odb_compiler(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return DockerOdbCompiler(renderer, vars).render();
}