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

#include "Expression.hh"
#include "ExpressionFactory.hh"
#include "Node.hh"
#include "ParserException.hh"
#include "parser-utils.hh"
#include "PlexilSchema.hh"

#include "pugixml.hpp"

using pugi::xml_attribute;
using pugi::xml_document;
using pugi::xml_node;
using pugi::node_element;
using pugi::node_pcdata;

#include <cstring>

namespace PLEXIL
{
  static void parseGlobalDeclarations(pugi::xml_node declXml)
  {
    // NYI
  }

  static void parseVariableDeclarations(Node *node, pugi::xml_node decls)
  {
    xml_node decl = decls.first_child();
    while (decl) {
      // TODO
    }
  }

  static coid parseInterface(Node *node, pugi::xml_node iface)
  {
    // TODO
  }

  static void parseNodeBody(Node *node, pugi::xml_node bodyXml)
  {
    PlexilNodeType typ = node->getType();
    if (typ == NodeType_Empty) {
      checkParserExceptionWithLocation(!bodyXml,
                                       bodyXml,
                                       "Empty node with a NodeBody element");
      return;
    }

    checkHasChildElement(bodyXml);
    bodyXml = bodyXml.first_child(); // strip away NodeBody noise
    char const *tag = bodyXml.name();

    switch (typ) {
    case NodeType_Assignment:
      checkParserExceptionWithLocation(0 == strcmp(ASSIGNMENT_TAG, tag),
                                       bodyXml,
                                       "Invalid body element \"" << tag << "\" in Assignment node");
      // TODO
      return;

    case NodeType_Command:
      checkParserExceptionWithLocation(0 == strcmp(COMMAND_TAG, tag),
                                       bodyXml,
                                       "Illegal body element \"" << tag << "\" in Command node");
      // TODO
      return;

    case NodeType_Update:
      checkParserExceptionWithLocation(0 == strcmp(UPDATE_TAG, tag),
                                       bodyXml,
                                       "Illegal body element \"" << tag << "\" in Update node");
      // TODO
      return;

    case NodeType_List:
      checkParserExceptionWithLocation(0 == strcmp(NODELIST_TAG, tag),
                                       bodyXml,
                                       "Illegal body element \"" << tag << "\" in NodeList node");
      // TODO: Create children
      return;

    case NodeType_LibraryNodeCall:
      checkParserExceptionWithLocation(0 == strcmp(LIBRARYNODECALL_TAG, tag),
                                       bodyXml,
                                       "Illegal body element \"" << tag << "\" in LibraryNodeCall node");
      // TODO: Insert library expansion
      return;

    default:
      assertTrue_2(ALWAYS_FAIL, "parseNodeBody: Internal error: Unknown node type");
    }
  }

  static Node *parseNode(pugi::xml_node xml, Node *parent)
  {
    xml_attribute const typeAttr = xml.attribute(NODETYPE_ATTR);
    checkParserExceptionWithLocation(typeAttr,
                                     xml,
                                     "Node has no " << NODETYPE_ATTR << " attribute");
    PlexilNodeType nodeType = parseNodeType(typeAttr.value());
    checkParserExceptionWithLocation(nodeType >= NodeType_NodeList && nodeType <= NodeType_LibraryNodeCall,
                                     xml, // should be attribute
                                     "Invalid node type \"" << typeAttr.value << "\"");

    // Where to put the things we parse on the first pass
    xml_node id;
    xml_node prio;
    xml_node iface;
    xml_node varDecls;
    xml_node body;

    xml_node temp = xml.first_child();
    while (temp) {
      // TODO: check that temp is an element
      char const *tag = temp.name();
      size_t taglen = strlen(tag);
      switch (taglen) {
      case 6: // NodeId
        checkParserExceptionWithLocation(0 == strcmp(NODEID_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        checkParserExceptionWithLocation(!id,
                                         temp, 
                                         "Dupliicate " << tag << " element in Node");
        id = temp;
        break;

      case 7: // Comment
        checkParserExceptionWithLocation(0 == strcmp(COMMENT_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 8: // NodeBody, Priority
        if (0 == strcmp(BODY_TAG, tag)) {
          checkParserExceptionWithLocation(!body,
                                           temp, 
                                           "Dupliicate " << tag << " element in Node");
          body = temp;
          break;
        }
        else if (0 == strcmp(PRIORITY_TAG, tag)) {
          checkParserExceptionWithLocation(!prio,
                                           temp, 
                                           "Dupliicate " << tag << " element in Node");
          prio = temp;
          break;
        }
        checkParserExceptionWithLocation(ALWAYS_FAIL,
                                         temp,
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 9: // Interface
        checkParserExceptionWithLocation(0 == strcmp(INTERFACE_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        checkParserExceptionWithLocation(!iface,
                                         temp, 
                                         "Dupliicate " << tag << " element in Node");
        iface = temp;
        break;

      case 12: // EndCondition, PreCondition
        if (0 == strcmp(END_CONDITION_TAG, tag)
            || 0 == strcmp(PRE_CONDITION_TAG, tag)) {
          break;
        }
        checkParserExceptionWithLocation(ALWAYS_FAIL,
                                         temp,
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 13: // ExitCondition, PostCondition, SkipCondition
        if (0 == strcmp(EXIT_CONDITION_TAG, tag)
            || 0 == strcmp(POST_CONDITION_TAG, tag)
            || 0 == strcmp(SKIP_CONDITION_TAG, tag)) {
          break;
        }
        checkParserExceptionWithLocation(ALWAYS_FAIL,
                                         temp,
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 14: // StartCondition
        checkParserExceptionWithLocation(0 == strcmp(START_CONDITION_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 15: // RepeatCondition
        checkParserExceptionWithLocation(0 == strcmp(REPEAT_CONDITION_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 18: // InvariantCondition
        checkParserExceptionWithLocation(0 == strcmp(INVARIANT_CONDITION_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        break;

      case 20: // VariableDeclarations
        checkParserExceptionWithLocation(0 == strcmp(VAR_DECLS_TAG, tag),
                                         temp, 
                                         "Illegal element \"" << tag << "\" in Node");
        checkParserExceptionWithLocation(!varDecls,
                                         temp, 
                                         "Dupliicate " << tag << " element in Node");
        varDecls = temp;
        break;

      default:
        checkParserExceptionWithLocation(ALWAYS_FAIL,
                                         temp,
                                         "Illegal element \"" << tag << "\" in Node");
        break;
      }

      temp = temp.next_sibling();
    }

    checkParserExceptionWithLocation(id,
                                     xml,
                                     "Node has no " << NODEID_TAG << " element");
    checkNotEmpty(id);
    char const *name = id.first_child().value();

    // TODO: check for duplicate conditions (?)

    Node *result = NodeFactory::createNode(nodeType, name, parent);

    try {
      // Populate local variables
      if (varDecls)
        parseVariableDeclarations(node, varDecls);

      // Get interface variables
      if (iface)
        parseInterface(node, iface);

      // TODO: create body (if not empty node)

    }
    catch (std::exception const & exc) {
      delete result;
      throw;
    }
    return result;
  }

  // XML has already been checked for gross errors
  // We only need to worry about conditions
  static void postInitNode(Node *node, xml_node const &xml)
  {
    // create conditions
    xml_node cond = xml.first_child();
    while (cond) {
      if (testTagSuffix(CONDITION_SUFFIX, cond)) {
        checkHasChildElement(cond);
        // Check that condition name is valid, get index
        Node::ConditionIndex which = Node::getConditionIndex(std::string(cond.name()));
        checkParserExceptionWithLocation(which >= Node::skip_idx && which <= Node::repeatIdx,
                                         "Node " << node->getNodeId()
                                         << ": Illegal condition name \"" << cond.name() << "\"");
        bool garbage;
        Expression *cond = createExpression(cond.first_child(),
                                            node,
                                            garbage);
        checkParserExceptionWithLocation(cond->valueType() == BOOLEAN_TYPE || cond->valueType() == UNKNOWN_TYPE,
                                         cond.first_child(),
                                         "Node " << node->getNodeId() << ": Expression for "
                                         << cond.name() << " is not Boolean");
        node->addUserCondition(which, cond, garbage);
      }
      cond = cond.next_sibling();
    }

    // finalize conditions
    node->finalizeConditions();

    // recurse on children (?)
  }

  static void postInitPlan(Node *node, xml_node const &xml)
  {
  }

  Node *parsePlan(xml_node const &xml)
    throw(ParserException)
  {
    checkTag(PLEXIL_PLAN_TAG, xml);
    checkHasChildElement(xml);

    xml_node elt = xml.first_child();

    // Handle global declarations
    if (testTag(GLOBAL_DECLARATIONS_TAG, elt)) {
      parseGlobalDeclarations(elt);
      elt = elt.next_sibling();
    }

    checkTag(NODE_TAG, elt);
    Node *result = parseNode(xml, NULL);
    postInitPlan(result, xml);
    return result;
  }
}