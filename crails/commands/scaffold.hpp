#pragma once
#include "../command.hpp"
#include "../scaffolds/controller.hpp"
#include "../scaffolds/odb_model.hpp"
#include "../scaffolds/view.hpp"
#include "../scaffolds/task.hpp"
#include "../scaffolds/resource.hpp"
#include "../scaffolds/layout.hpp"

class Scaffold : public Crails::Command
{
  std::string type;
  ScaffoldModel* model = nullptr;
public:
  std::string_view description() const override { return "Generates basic files forming the basic structure of Crails."; }

  bool initialize(int argc, const char** argv) override
  {
    configuration.initialize();
    if (argc > 0)
    {
      type = argv[1];
      if (type == "controller")
        model = new ControllerScaffold;
      else if ((type == "model" || type == "model-odb") && configuration.has_module("libcrails-odb"))
        model = new OdbModelScaffold;
      else if (type == "view")
        model = new ViewScaffold;
      else if (type == "task")
        model = new TaskScaffold;
      else if (type == "resource")
        model = new ResourceScaffold;
      else if (type == "layout")
        model = new LayoutScaffold;
      else
      {
        std::cerr << "Unknown scaffold type " << type << std::endl;
        return false;
      }
      return Command::initialize(argc - 1, &argv[1]);
    }
    return false;
  }

  void options_description(boost::program_options::options_description& desc) const override
  {
    if (model)
      model->options_description(desc);
  }

  int run()
  {
    model->create(options);
    return 0;
  }

private:
};
