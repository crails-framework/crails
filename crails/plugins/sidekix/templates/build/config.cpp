#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SidekixConfigSidekixCpp : public Crails::Template
{
public:
  render_SidekixConfigSidekixCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(1072);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include <crails/sidekix/database.hpp>\n#include <crails/sidekix/file_database.hpp>\n#include <crails/sidekix/test_database.hpp>\n#include <filesystem>\n\nusing namespace Sidekix;\nusing namespace std;\n\nstatic const string task_store_directory = \".pending-tasks\";\n\nSideTaskDatabase* SideTaskDatabase::initialize()\n{\n  if (Crails::environment == Crails::Test)\n    return new Tests::Database();\n  std::filesystem::create_directories(task_store_directory);\n  return new FileDatabase(task_store_directory);\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_sidekix_config_sidekix_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixConfigSidekixCpp(renderer, target, vars).render();
}