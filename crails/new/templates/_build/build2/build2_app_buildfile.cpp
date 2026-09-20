#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <list>

class render_Build2AppBuildfile : public Crails::Template
{
public:
  render_Build2AppBuildfile(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    project_name(Crails::cast< std::string >(vars, "project_name")), 
    plugins(reinterpret_cast<std::list<std::string>&>(*Crails::cast< std::list<std::string>* >(vars, "plugins")))
  {}

  void render()
  {
    ecpp_stream.reserve(2562);
    // BEGIN TEMPLATE BODY
ecpp_stream << "";
 for (const std::string& plugin : plugins){
  ecpp_stream << "\nimport intf_libs += " << ( plugin );
  ecpp_stream << "%lib{" << ( plugin.substr(3) );
  ecpp_stream << "}";
 };
  ecpp_stream << "\nlib{" << ( project_name );
  ecpp_stream << "}: {hxx ixx txx cxx}{**} $impl_libs $intf_libs\n\nhxx{export}@./: cxx.importable = false\n\ncxx.poptions =+ \"-I$out_root\" \"-I$src_root\" \"-I$src_root/app\"\ncxx.poptions =+ \"-Wall\" \"-Wno-unknown-pragmas\"\ncxx.poptions =+ \"-pedantic\"\n\n{hbmia obja}{*}: cxx.poptions += -DLIBCRAILS_STATIC_BUILD\n{hbmia objs}{*}: cxx.poptions += -DLIBCRAILS_SHARED_BUILD\n\nlib{" << ( project_name );
  ecpp_stream << "}:\n{\n  cxx.export.poptions = \"-I$out_root/app\" \"-I$src_root/app\"\n  cxx.export.libs = $intf_libs\n}\n\nliba{" << ( project_name );
  ecpp_stream << "}: cxx.export.poptions += -DLIBCRAILS_STATIC\nlibs{" << ( project_name );
  ecpp_stream << "}: cxx.export.poptions += -DLIBCRAILS_SHARED\n\nif $version.pre_release\n  lib{" << ( project_name );
  ecpp_stream << "}: bin.lib.version = \"-$version.project_id\"\nelse\n  lib{" << ( project_name );
  ecpp_stream << "}: bin.lib.version = \"-$version.major.$version.minor\"\n\n{hxx ixx txx}{*}:\n{\n  install         = include/\n  install.subdirs = true\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string project_name;
  std::list<std::string>& plugins;
};

void render_build2_app_buildfile(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2AppBuildfile(renderer, target, vars).render();
}