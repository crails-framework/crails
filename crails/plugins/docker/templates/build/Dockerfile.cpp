#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_DockerDockerfile : public Crails::Template
{
public:
  render_DockerDockerfile(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    image(Crails::cast<string>(vars, "image",  "debian:sid")), 
    include_assets(Crails::cast<bool>(vars, "include_assets",  true)), 
    with_comet(Crails::cast<bool>(vars, "with_comet",  false)), 
    with_odb(Crails::cast<bool>(vars, "with_odb",  false)), 
    with_metarecord(Crails::cast<bool>(vars, "with_metarecord",  false)), 
    with_ssh(Crails::cast<bool>(vars, "with_ssh",  false)), 
    with_imagemagick(Crails::cast<bool>(vars, "with_imagemagick",  false)), 
    sass_backend( with_metarecord ? "ruby" : "nodejs"), 
    cheerp_repository(Crails::cast<string>(vars, "cheerp_repository",  "cheerp-nightly-ppa/ubuntu focal main")), 
    script_path(Crails::cast<string>(vars, "script_path",  "base/"))
  {}

  void render()
  {
ecpp_stream << "FROM " << ( image );
  ecpp_stream << "\n\nWORKDIR /tmp\nENV LD_LIBRARY_PATH /usr/local/lib\nENV DEBIAN_FRONTEND \"noninteractive\"\nENV TZ \"Europe/London\"\n\nRUN apt-get -y --allow-unauthenticated update && \\\n    apt-get -y --allow-unauthenticated upgrade && \\\n    apt-get -y install curl \\\n  cmake \\\n  pkg-config \\\n  build-essential \\\n  libbz2-dev \\\n  libssl-dev \\\n  git\n\nCOPY " << ( script_path );
  ecpp_stream << "build-build2.sh build-build2.sh\nRUN bash build-build2.sh\n";
 if (with_odb){
  ecpp_stream << "\nRUN apt-get -y install libpq-dev libsqlite3-dev libmysqlclient-dev\nCOPY " << ( script_path );
  ecpp_stream << "build-odb-compiler.sh build-odb-compiler.sh\nRUN bash build-odb-compiler.sh";
 };
  ecpp_stream << "\n";
 if (with_comet){
  ecpp_stream << "\nRUN echo \"deb http://ppa.launchpad.net/leaningtech-dev/" << ( cheerp_repository );
  ecpp_stream << "\" >> /etc/apt/sources.list\nRUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 84540D4B9BF457D5\nRUN apt-get -y --allow-unauthenticated update\nRUN apt-get -y install cheerp-core";
 };
  ecpp_stream << "\n";
 if (with_ssh){
  ecpp_stream << "\nRUN apt-get -y install libssh-dev";
 };
  ecpp_stream << "\n";
 if (with_imagemagick){
  ecpp_stream << "\nRUN apt-get -y install imagemagick";
 };
  ecpp_stream << "\n";
 if (with_metarecord){
  ecpp_stream << "\nRUN apt-get install -y ruby ruby-dev\nRUN gem install meta-record";
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
  ecpp_stream << "\n\nCOPY " << ( script_path );
  ecpp_stream << "build-environment.sh build-environment.sh\nCOPY " << ( script_path );
  ecpp_stream << "build-crails.sh build-crails.sh\nRUN bash build-crails.sh";
 if (with_comet){
  ecpp_stream << "\nCOPY " << ( script_path );
  ecpp_stream << "build-comet.sh build-comet.sh\nRUN bash build-comet.sh";
 };
  ecpp_stream << "\n\nRUN mkdir -p /opt/application\nWORKDIR /opt/application\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string image;
  bool include_assets;
  bool with_comet;
  bool with_odb;
  bool with_metarecord;
  bool with_ssh;
  bool with_imagemagick;
  string sass_backend;
  string cheerp_repository;
  string script_path;
};

void render_docker_dockerfile(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_DockerDockerfile(renderer, target, vars).render();
}