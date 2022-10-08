#pragma once
#include <crails/cli/scaffold.hpp>
#include "../project_configuration.hpp"
#include "../scaffolds/controller.hpp"
#include "../scaffolds/odb_model.hpp"
#include "../scaffolds/metarecord_model.hpp"
#include "../scaffolds/view.hpp"
#include "../scaffolds/task.hpp"
#include "../scaffolds/resource.hpp"
#include "../scaffolds/layout.hpp"
#include "../scaffolds/module.hpp"
#include "../scaffolds/test.hpp"

class Scaffold : public Crails::Scaffold
{
  ProjectConfiguration configuration;
public:
  Scaffold()
  {
    try { configuration.initialize(); } catch (...) {}
  }

  Scaffolds available_scaffolds() const override
  {
    Scaffolds scaffolds{
      {"controller", []() { return new ControllerScaffold; }},
      {"view",       []() { return new ViewScaffold; }},
      {"task",       []() { return new TaskScaffold; }},
      {"resource",   []() { return new ResourceScaffold; }},
      {"layout",     []() { return new LayoutScaffold; }},
      {"module",     []() { return new ModuleScaffold; }},
      {"test",       []() { return new TestScaffold; }}
    };

    if (configuration.has_plugin("metarecord"))
      scaffolds.emplace("model", []() { return new MetaRecordModelScaffold; });
    else if (configuration.has_plugin("libcrails-odb"))
      scaffolds.emplace("model", []() { return new OdbModelScaffold; });
    return scaffolds;
  }
};
