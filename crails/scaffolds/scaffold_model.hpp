#pragma once
#include <boost/program_options.hpp>

class ScaffoldModel
{
public:
  virtual void options_description(boost::program_options::options_description& desc) const = 0;
  virtual int create(boost::program_options::variables_map&) = 0;
};
