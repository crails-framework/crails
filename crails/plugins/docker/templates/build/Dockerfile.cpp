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
    with_comet(Crails::cast<bool>(vars, "with_comet",  false)), 
    with_odb(Crails::cast<bool>(vars, "with_odb",  false)), 
    with_metarecord(Crails::cast<bool>(vars, "with_metarecord",  false)), 
    sass_backend( with_metarecord ? "ruby" : "nodejs"), 
    cheerp_repository(Crails::cast<string>(vars, "cheerp_repository",  "cheerp-nightly-ppa/ubuntu focal main"))
  {}

  std::string render()
  {
ecpp_stream << "FROM " << ( image );
  ecpp_stream << "\n\nWORKDIR /tmp\nENV LD_LIBRARY_PATH /usr/local/lib\n\nRUN apt-get -y --allow-unauthenticated update && \\\n    apt-get -y --allow-unauthenticated upgrade && \\\n    apt-get -y install curl \\\n  cmake \\\n  pkg-config \\\n  build-essential \\\n  libbz2-dev \\\n  libssl-dev \\\n  git\n\nRUN apt-get -y install \\\n  libboost1.74-dev \\\n  libboost-filesystem1.74-dev \\\n  libboost-random1.74-dev \\\n  libboost-program-options1.74-dev \\\n  libboost-thread1.74-dev \\\n  libboost-random1.74-dev \\\n  libboost-system1.74-dev\n\nCOPY build-build2.sh build-build2.sh\nRUN bash build-build2.sh\n";
 if (with_odb){
  ecpp_stream << "\nRUN apt-get -y install libpq-dev libsqlite3-dev libmysqlclient-dev\nCOPY build-odb-compiler.sh build-odb-compiler.sh\nRUN bash build-odb-compiler.sh";
 };
  ecpp_stream << "\n";
 if (with_comet){
  ecpp_stream << "\nRUN echo \"deb http://ppa.launchpad.net/leaningtech-dev/" << ( cheerp_repository );
  ecpp_stream << "\" >> /etc/apt/sources.list\nRUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 84540D4B9BF457D5\nRUN apt-get -y --allow-unauthenticated update\nRUN apt-get -y install cheerp-core";
 };
  ecpp_stream << "\n";
 if (with_metarecord){
  ecpp_stream << "\nRUN apt-get install -y ruby\nRUN gem install meta-record";
 };
  ecpp_stream << "\n";
 if (include_assets){
  ecpp_stream << "";
   if (sass_backend == "nodejs"){
  ecpp_stream << "\nRUN apt-get install -y nodejs npm\nRUN npm install -g node-sass";
 }else{
  ecpp_stream << "\nRUN gem install sass";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n\nCOPY build-environment.sh build-environment.sh\nCOPY build-crails.sh build-crails.sh\nRUN bash build-crails.sh";
 if (with_comet){
  ecpp_stream << "\nCOPY build-comet.sh build-comet.sh\nRUN bash build-comet.sh";
 };
  ecpp_stream << "\n\nRUN mkdir -p /opt/application\nWORKDIR /opt/application\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  string image;
  bool include_assets;
  bool with_comet;
  bool with_odb;
  bool with_metarecord;
  string sass_backend;
  string cheerp_repository;
};

std::string render_docker_dockerfile(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return DockerDockerfile(renderer, vars).render();
}