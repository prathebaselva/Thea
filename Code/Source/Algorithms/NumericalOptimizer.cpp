//============================================================================
//
// This file is part of the Thea project.
//
// This software is covered by the following BSD license, except for portions
// derived from other works which are covered by their respective licenses.
// For full licensing information including reproduction of these external
// licenses, see the file LICENSE.txt provided in the documentation.
//
// Copyright (C) 2011, Siddhartha Chaudhuri/Stanford University
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holders nor the names of contributors
// to this software may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//============================================================================

#include "NumericalOptimizer.hpp"

namespace Thea {
namespace Algorithms {

NumericalOptimizer::NumericalOptimizer()
: constraint_ndims(-1)
{
}

void
NumericalOptimizer::addConstraint(LinearConstraint::ConstPtr constraint)
{
  if (constraint)
  {
    if (constraint_ndims < 0)
      constraint_ndims = constraint->numDimensions();
    else
    {
      alwaysAssertM(constraint->numDimensions() == constraint_ndims,
                    std::string(getName()) + ": Constraint dimensions don't match existing constraints");
    }

    linear_constraints.push_back(constraint);
  }
}

void
NumericalOptimizer::addConstraint(NonlinearConstraint::ConstPtr constraint)
{
  if (constraint)
  {
    if (constraint_ndims < 0)
      constraint_ndims = constraint->numDimensions();
    else
    {
      alwaysAssertM(constraint->numDimensions() == constraint_ndims,
                    std::string(getName()) + ": Constraint dimensions don't match existing constraints");
    }

    nonlinear_constraints.push_back(constraint);
  }
}

void
NumericalOptimizer::clearConstraints()
{
  linear_constraints.clear();
  nonlinear_constraints.clear();
  constraint_ndims = -1;
}

NumericalOptimizerFactory *
NumericalOptimizerManager::getFactory(std::string const & type)
{
  FactoryMap::const_iterator installed = installed_factories.find(toLower(type));
  if (installed == installed_factories.end())
    throw Error("No factory for numerical optimizers of type '" + type + "' is installed");

  return installed->second;
}

bool
NumericalOptimizerManager::installFactory(std::string const & type, NumericalOptimizerFactory * factory)
{
  debugAssertM(factory, "NumericalOptimizerManager: Null factory cannot be installed");

  std::string type_lc = toLower(type);
  FactoryMap::const_iterator installed = installed_factories.find(type_lc);
  if (installed == installed_factories.end())
  {
    installed_factories[type_lc] = factory;
    return true;
  }
  else
    return false;
}

void
NumericalOptimizerManager::uninstallFactory(std::string const & type)
{
  installed_factories.erase(type);
}

} // namespace Algorithms
} // namespace Thea
