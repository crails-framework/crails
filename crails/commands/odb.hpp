#pragma once
#include "../command.hpp"
#include <filesystem>
#include <list>

class BuildOdb : public ::Command
{
public:
  typedef std::vector<std::filesystem::path> FileList;

  BuildOdb()
  {
    custom_includes = {"crails/safe_ptr.hpp", "crails/odb/traits.hpp", "config/odb.hpp"};
  }

  int              run() override;
  std::string_view description() const override { return "runs the odb compiler to generate your models queries and schema"; }
  void             options_description(boost::program_options::options_description&) const override;

private:
  bool        increment_schema_version();
  bool        compile_models_at_once(const FileList& files);
  bool        compile_models_one_by_one(const FileList& files);
  bool        compile_models(const FileList& files);
  bool        generate_schema(const FileList& files);
  FileList    collect_files();
  std::string odb_command(const std::string& output_dir = "");
  std::string hxx_prologue();
  void        apply_new_version();
  void        clear_empty_changesets();

  bool at_once_fix_include_paths(const FileList& files) const;

private:
  std::string              odb_compiler = "odb";
  std::string              temporary_dir;
  std::string              output_dir;
  std::list<std::string>   input_dirs;
  std::list<std::string>   custom_includes;
  std::list<std::string>   backends;
  std::string              hxx_prologue_, cxx_prologue;
  std::string              default_pointer;
  std::string              input_name = "application";
  std::string              table_prefix;
  bool                     use_session = true;
  bool                     at_once = true;
};
