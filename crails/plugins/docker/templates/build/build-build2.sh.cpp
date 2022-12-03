#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class DockerBuildBuild2 : public Crails::Template
{
public:
  DockerBuildBuild2(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    build2_version(Crails::cast<string>(vars, "build2_version",  "0.15.0")), 
    build2_fingerprint(Crails::cast<string>(vars, "build2_fingerprint",  "70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43"))
  {}

  void render()
  {
ecpp_stream << "#!/bin/sh -ex\n\nexport BUILD2_VERSION=\"" << ( build2_version );
  ecpp_stream << "\"\nexport BUILD2_FINGERPRINT=\"" << ( build2_fingerprint );
  ecpp_stream << "\"\n\ncurl -sSfO https://download.build2.org/$BUILD2_VERSION/build2-install-$BUILD2_VERSION.sh\nchmod +x build2-install-$BUILD2_VERSION.sh\nsh build2-install-$BUILD2_VERSION.sh --yes --trust \"$BUILD2_FINGERPRINT\"\n\n\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string build2_version;
  string build2_fingerprint;
};

void render_docker_build_build2(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  DockerBuildBuild2(renderer, target, vars).render();
}