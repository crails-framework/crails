#pragma once
#include <crails/cli/scaffold_model.hpp>
#include <crails/cli/conventions.hpp>
#include "../file_renderer.hpp"
#include "../file_editor.hpp"
#include "../project_configuration.hpp"
#include <crails/utils/string.hpp>

class TestScaffold : public Crails::ScaffoldModel
{
  string test_name, target;
  FileRenderer renderer;
public:
  TestScaffold() : target("spec")
  {

  }

  void options_description(boost::program_options::options_description & desc) const override
  {
    desc.add_options()
      ("name,n", boost::program_options::value<std::string>(), "test's name")
      ("target,t", boost::program_options::value<std::string>(), "folder in which the test should be created (defaults to spec/)");
  }

  int create(boost::program_options::variables_map& options) override
  {
    string filename, filepath;

    test_name = options["name"].as<std::string>();
    if (options.count("target"))
      target = options["target"].as<std::string>();
    filename = Crails::naming_convention.filenames(test_name);
    filepath = target + '/' + filename;
    renderer.vars["classname"] = Crails::naming_convention.classnames(test_name + "_spec");
    renderer.vars["header"]    = filename + ".hpp";
    renderer.generate_file("scaffolds/spec/test.hpp", filepath + ".hpp");
    renderer.generate_file("scaffolds/spec/test.cpp", filepath + ".cpp");
    return 0;
  }
};
