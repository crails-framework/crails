#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <list>

class render_Build2RepositoriesManifest : public Crails::Template
{
public:
  render_Build2RepositoriesManifest(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    crails_version(Crails::cast<std::string>(vars, "crails_version")), 
    project_name(Crails::cast<std::string>(vars, "project_name")), 
    plugins(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "plugins"))), 
    git_organization_url( "https://github.com/crails-framework")
  {}

  void render()
  {
ecpp_stream << ": 1\nsummary: " << ( project_name );
  ecpp_stream << " project repository\n\n:\nrole: prerequisite\nlocation: https://pkg.cppget.org/1/stable\ntrust: 70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43\n\n:\nrole: prerequisite\nlocation: " << ( git_organization_url );
  ecpp_stream << "/libcrails-tests.git#" << ( crails_version );
  ecpp_stream << "\n";
 for (const std::string& plugin : plugins){
  ecpp_stream << "\n:\nrole: prerequisite\nlocation: " << ( git_organization_url );
  ecpp_stream << "/" << ( plugin );
  ecpp_stream << ".git#" << ( crails_version );
  ecpp_stream << "\n";
 };
  ecpp_stream << "";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string crails_version;
  std::string project_name;
  std::list<std::string>& plugins;
  std::string git_organization_url;
};

void render_build2_repositories_manifest(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2RepositoriesManifest(renderer, target, vars).render();
}