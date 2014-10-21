/* Copyright (c) 2006-2014, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "Debug.hh"
#include "Error.hh"
#include "Expressions.hh"
#include "Node.hh"
#include "lifecycle-utils.h"
#include "parsePlan.hh"
#include "pugixml.hpp"
#include "test/TransitionExternalInterface.hh"

void loadPlanBenchmark(std::string const &planFile)
{
  // Load the XML
  pugi::xml_document *doc = PLEXIL::loadXmlFile(planFile);
  checkParserException(doc, "File " << planFile << " not found");
  
  PLEXIL::Node *root = PLEXIL::parsePlan(doc->document_element());
  checkParserException(root, "parsePlan returned NULL");

  delete root;
  delete doc;
}

void usage()
{
  std::cout << "Usage: benchmark [options] <plan file>\n"
            << " Options:\n"
            << "  -h               Display this message and exit\n"
            << "  -d <debug file>  Use debug-file as debug message config (default Debug.cfg)\n"
            << "  -n <number>      Number of times to load the plan (default 1)\n"
            << std::endl;
}

int main(int argc, char *argv[])
{
  std::string debugConfig("Debug.cfg");
  std::string planFile;
  unsigned int n = 1;

  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-d"))
      debugConfig = std::string(argv[++i]);
    else if (!strcmp(argv[i], "-h")) {
      usage();
      return 0;
    }
    else if (!strcmp(argv[i], "-n")) {
      n = atoi(argv[++i]);
      if (n <= 0) {
        std::cerr << "-n option value out of range or invalid" << std::endl;
        usage();
        return 1;
      }
    }
    else {
      if (!planFile.empty()) {
        std::cerr << "Multiple plan files specified" << std::endl;
        usage();
        return 1;
      }
      planFile = argv[i];
    }
  }

  if (planFile.empty()) {
    std::cerr << "No plan file specified" << std::endl;
    usage();
    return 1;
  }
  
  std::ifstream config(debugConfig.c_str());
  if (config.good()) {
     readDebugConfigStream(config);
     std::cout << "Reading configuration file " << debugConfig.c_str() << "\n";
  }
  else
     std::cerr << "Unable to read configuration file " << debugConfig << " - continuing\n";
  
  std::cout << "Loading plan file " << planFile << ' ' << n << " times..." << std::endl;

  try {
    // Initialize infrastructure
    Error::doThrowExceptions();
    PLEXIL::initializeExpressions();
    PLEXIL::TransitionExternalInterface intfc;
    PLEXIL::g_interface = &intfc;

    // TODO: time before & after
    for (unsigned int i = 0; i < n; ++i)
      loadPlanBenchmark(planFile);

    PLEXIL::g_interface = NULL;
    runFinalizers();
  }
  catch (PLEXIL::ParserException const &e) {
    std::cerr << "Aborting benchmark due to parser exception:\n" << e.what() << std::endl;
    std::cout << "Aborted." << std::endl;
    return 1;
  }
  catch (Error const &e) {
    std::cerr << "Aborting benchmark due to error:\n" << e << std::endl;
    std::cout << "Aborted." << std::endl;
    return 1;
  }
  std::cout << "Done." << std::endl;
  return 0;
}
