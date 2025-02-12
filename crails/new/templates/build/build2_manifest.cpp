#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <list>

class render_Build2Manifest : public Crails::Template
{
public:
  render_Build2Manifest(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    crails_version(Crails::cast<std::string>(vars, "crails_version")), 
    project_name(Crails::cast<std::string>(vars, "project_name")), 
    configuration_type(Crails::cast<std::string>(vars, "configuration_type",  "full")), 
    cpp_version(Crails::cast<std::string>(vars, "cpp_version",  "c++17")), 
    formats(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "formats"))), 
    plugins(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "plugins")))
  {}

  void render()
  {
ecpp_stream << ": 1\nname: " << ( project_name );
  ecpp_stream << "\nversion: 0.1.0\nlanguage: c++\nsummary: Crails Application\nlicense: other: proprietary ; Not free/open-source.\ndescription-file: README.md\nurl: https://example.org/bpkgtest2\nemail: example@mail.com\n#build-error-mail: example@mail.com\ndepends: * build2 >= 0.16.0\ndepends: * bpkg >= 0.16.0";
 for (const std::string& plugin : plugins){
  ecpp_stream << "\ndepends: " << ( plugin );
  ecpp_stream << " ^" << ( crails_version );
  ecpp_stream << "";
 };
  ecpp_stream << "\ndepends: libcrails-tests ^" << ( crails_version );
  ecpp_stream << "\n#depends: libcrails-selenium ^" << ( crails_version );
  ecpp_stream << "\n# Custom dependencies (do not modify this line)\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string crails_version;
  std::string project_name;
  std::string configuration_type;
  std::string cpp_version;
  std::list<std::string>& formats;
  std::list<std::string>& plugins;
};

void render_build2_manifest(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2Manifest(renderer, target, vars).render();
}