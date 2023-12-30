#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_OdbMigrateCpp : public Crails::Template
{
public:
  render_OdbMigrateCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <crails/odb/database.hpp>\n#include <odb/schema-catalog.hxx>\n#include \"config/databases.hpp\"\n\nusing namespace std;\n\nint help(char* arg_command)\n{\n  cout << \"Usage: \" << arg_command << \" [options] database_key\" << endl;\n  cout << \"Options:\" << endl;\n  cout << \"\\t-c: create the database before performing the migration\" << endl;\n  cout << \"\\t-d: drop the schema instead of performing a migration\" << endl;\n  return -1;\n}\n\nvoid create_database(const std::string& database_name)\n{\n  const auto& settings = Crails::Databases::Settings::singleton::require()\n    .at(Crails::environment)\n    .at(database_name);\n\n  // The second and third parameters are the user and password for the database,\n  // If left empty, it uses the identity specified in the database settings.\n  // Make sure to use the credentials of a user which can create users and databases.\n  Crails::Odb::Database::create_from_settings(settings, \"\", \"\");\n}\n\nint main(int argc, char** argv)\n{\n  if (argc < 2)\n    return help(argv[0]);\n  else\n  {\n    string database_name = argv[argc - 1];\n    string option        = argc > 2 ? string(argv[1]) : string(\"\");\n\n    if (option == \"-c\")\n      create_database(database_name);\n    {\n      Crails::Odb::Database& database = CRAILS_DATABASE(Crails::Odb, database_name);\n\n      if (option == \"-d\")\n        database.drop();\n      else\n        database.migrate();\n    }\n  }\n  return 0;\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_odb_migrate_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_OdbMigrateCpp(renderer, target, vars).render();
}