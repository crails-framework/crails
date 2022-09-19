#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixConfigSidekixCpp : public Crails::Template
{
public:
  SidekixConfigSidekixCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <crails/sidekix/database.hpp>\n#include <crails/sidekix/file_database.hpp>\n#include <crails/sidekix/test_database.hpp>\n#include <filesystem>\n\nusing namespace Sidekix;\nusing namespace std;\n\nstatic const string task_store_directory = \".pending-tasks\";\n\nSideTaskDatabase* SideTaskDatabase::initialize()\n{\n  if (Crails::environment == Crails::Test)\n    return new Tests::Database();\n  std::filesystem::create_directories(task_store_directory);\n  return new FileDatabase(task_store_directory);\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_sidekix_config_sidekix_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return SidekixConfigSidekixCpp(renderer, vars).render();
}