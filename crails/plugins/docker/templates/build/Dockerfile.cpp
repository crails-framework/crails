#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class DockerDockerfile : public Crails::Template
{
public:
  DockerDockerfile(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    image(Crails::cast<string>(vars, "image",  "debian:sid")), 
    include_assets(Crails::cast<bool>(vars, "include_assets",  true)), 
    include_cheerp(Crails::cast<bool>(vars, "include_cheerp",  false)), 
    include_odb(Crails::cast<bool>(vars, "include_odb",  false)), 
    cheerp_repository(Crails::cast<string>(vars, "cheerp_repository",  "cheerp-nightly-ppa/ubuntu focal main")), 
    build2_fingerprint(Crails::cast<string>(vars, "build2_fingerprint"))
  {}

  std::string render()
  {
ecpp_stream << "FROM " << ( image );
  ecpp_stream << "\n\nENV LD_LIBRARY_PATH /usr/local/lib\nENV BUILD2_VERSION 0.15.0\nENV BUILD2_FINGERPRINT " << ( build2_fingerprint );
  ecpp_stream << "\n\nRUN apt-get -y --allow-unauthenticated update && \\n    apt-get -y --allow-unauthenticated upgrade && \\n    apt-get -y install curl \\n  cmake \\n  pkg-config \\n  build-essential \\n  libbz2-dev \\n  libssl-dev \\n  git\n\nRUN apt-get -y install \\n  libboost1.74-dev \\n  libboost-filesystem1.74-dev \\n  libboost-random1.74-dev \\n  libboost-program-options1.74-dev \\n  libboost-thread1.74-dev \\n  libboost-random1.74-dev \\n  libboost-system1.74-dev\n\nRUN curl -sSfo https://download.build2.org/$BUILD2_VERSION/build2-install-$BUILD2_VERSION.sh\nRUN chmod +x build2-install-$BUILD2_VERSION.sh\nRUN sh build2-install-$BUILD2_VERSION.sh --yes --trust \"$BUILD2_FINGERPRINT\"\n";
 if (include_assets){
  ecpp_stream << "\nRUN apt-get install -y nodejs npm\nRUN npm install -g node-sass";
 };
  ecpp_stream << "\n";
 if (include_odb){
  ecpp_stream << "\nRUN apt-get -y install libpq-dev libsqlite3-dev libmysqlclient-dev\nCOPY odb-compiler.sh odb-compiler.sh\nRUN bash odb-compiler.sh";
 };
  ecpp_stream << "\n";
 if (include_cheerp){
  ecpp_stream << "\nRUN echo \"deb http://ppa.launchpad.net/leaningtech-dev/" << ( cheerp_repository );
  ecpp_stream << "\" >> /etc/apt/sources.list\nRUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 84540D4B9BF457D5\nRUN apt-get -y --allow-unauthenticated update\nRUN apt-get -y install cheerp-core";
 };
  ecpp_stream << "\n\nCOPY build-environment.sh build-environment.sh\nCOPY build-crails.sh build-crails.sh\nRUN bash build-crails.sh\nCOPY build-comet.sh build-comet.sh\nRUN bash build-comet.sh\n\nRUN mkdir -p /opt/webapp\nWORKDIR /opt/webapp\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  string image;
  bool include_assets;
  bool include_cheerp;
  bool include_odb;
  string cheerp_repository;
  string build2_fingerprint;
};

std::string render_docker_dockerfile(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return DockerDockerfile(renderer, vars).render();
}