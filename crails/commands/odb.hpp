#pragma once
#include "../command.hpp"
#include <boost/filesystem.hpp>
#include <list>

class BuildOdb : public Crails::Command
{
public:
  typedef std::vector<boost::filesystem::path> FileList;

  BuildOdb()
  {
    custom_includes = {"crails/safe_ptr.hpp", "crails/odb/traits.hpp", "config/odb.hpp"};
  }

  int              run() override;
  std::string_view description() const { return "runs the odb compiler to generate your models queries and schema"; }
  void             options_description(boost::program_options::options_description&) const override;

private:
  bool        compile_models(const FileList& files);
  bool        generate_schema(const FileList& files);
  FileList    collect_files();
  std::string odb_command();
  std::string hxx_prologue();

private:
  std::string              odb_compiler = "odb";
  std::string              output_dir;
  std::list<std::string>   input_dirs;
  std::list<std::string>   custom_includes;
  std::list<std::string>   backends;
  std::string              hxx_prologue_, cxx_prologue;
  std::string              default_pointer;
  std::string              input_name;
  std::string              table_prefix;
  bool                     use_session = true;
  bool                     at_once = false;
};
