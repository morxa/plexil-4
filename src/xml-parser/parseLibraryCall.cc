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

#include "Debug.hh"
#include "Error.hh"
#include "ExpressionFactory.hh"
#include "LibraryCallNode.hh"
#include "parseNode.hh"
#include "parsePlan.hh" // for getLibraryNode
#include "ParserException.hh"
#include "parser-utils.hh"
#include "PlexilSchema.hh"

#include "pugixml.hpp"

using pugi::xml_node;
using pugi::node_element;
using pugi::node_pcdata;

namespace PLEXIL
{

  //
  // First pass
  //

  static void checkAlias(LibraryCallNode *node, xml_node aliasXml)
    throw (ParserException)
  {
    checkTag(ALIAS_TAG, aliasXml);
    xml_node nameXml = aliasXml.first_child();
    checkTag(NODE_PARAMETER_TAG, nameXml);
    checkNotEmpty(nameXml);
    std::string name(nameXml.child_value());
    // Basic checks to see that we have something that could be an expression
    xml_node temp = nameXml.next_sibling();
    checkParserExceptionWithLocation(temp,
                                     aliasXml,
                                     "Alias " << name << " missing value expression in LibraryNodeCall node");
    checkParserExceptionWithLocation(temp.type() == node_element && temp.first_child(),
                                     temp,
                                     "Alias " << name << " has malformed value expression in LibraryNodeCall node");
  }

  // *** TODO: Invalidate library node if parser error expanding call ***
  void constructLibraryCall(Node *node, xml_node callXml)
    throw (ParserException)
  {
    LibraryCallNode *callNode = dynamic_cast<LibraryCallNode *>(node);
    assertTrue_2(callNode,
                 "constructLibraryCall: Not a LibraryCallNode");
    xml_node temp = callXml.first_child();
    checkTag(NODEID_TAG, temp);
    char const *name = temp.child_value();
    checkParserExceptionWithLocation(*name,
                                     temp,
                                     "Empty NodeId in LibraryNodeCall node");
    temp = temp.next_sibling();

    // Check (but don't populate) aliases
    while (temp) {
      checkAlias(callNode, temp);
      temp = temp.next_sibling();
    }

    // Construct call
    xml_node templ = getLibraryNode(name);
    checkParserExceptionWithLocation(templ,
                                     callXml,
                                     "Library node " << name << " not found");
    callNode->addChild(parseNode(templ, callNode));
  }

  // Second pass
  static void finalizeAlias(LibraryCallNode *node, xml_node aliasXml)
    throw (ParserException)
  {
    // Construct the actual alias expression
    xml_node nameXml = aliasXml.first_child();
    std::string name(nameXml.child_value());

    debugMsg("finalizeAlias",
             " caller " << node->getNodeId() << ", constructing alias " << name);
             
    // Add the alias
    checkParserExceptionWithLocation(!node->findVariable(name, true),
                                     aliasXml,
                                     "Duplicate alias name " << name << " in LibraryNodeCall node");
    bool isGarbage = false;
    Expression *exp = createExpression(nameXml.next_sibling(), node, isGarbage);
    node->addAlias(name, exp, isGarbage);
  }

  // Second pass
  void finalizeLibraryCall(Node *node, xml_node callXml)
    throw (ParserException)
  {
    debugMsg("finalizeLibraryCall", " caller " << node->getNodeId());
    LibraryCallNode *callNode = dynamic_cast<LibraryCallNode *>(node);
    assertTrue_2(callNode, "Internal error: Used to be a LibraryCallNode, but not now");
    xml_node temp = callXml.first_child();
    char const *calleeName = temp.child_value();
    temp = temp.next_sibling();
    // Initialize aliases
    while (temp) {
      debugMsg("finalizeLibraryCall", " finalizing alias");
      finalizeAlias(callNode, temp);
      temp = temp.next_sibling();
    }
    // Descend into called node
    assertTrue_2(!node->getChildren().empty(),
                 "Internal error: LibraryNodeCall node missing called node");
    Node *callee = node->getChildren().front();
    xml_node calleeXml = getLibraryNode(std::string(calleeName));
    assertTrue_2(calleeXml,
                 "Internal error: LibraryNodeCall can't find XML for called node");
    finalizeNode(callee, calleeXml);
  }

} // namespace PLEXIL
