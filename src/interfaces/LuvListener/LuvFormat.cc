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

#include "LuvFormat.hh"

#include "Assignable.hh"
#include "Error.hh"
#include "Node.hh"

#include <iostream>

namespace PLEXIL {

  //
  // Local utilities
  //

  inline void simpleStartTag(std::ostream& s, const char* val) {
    s << '<' << val << ">";
  }

  inline void openStartTag(std::ostream& s, const char* val) {
    s << '<' << val << ' ';
  }

  inline void closeTag(std::ostream& s) {
    s << ">";
  }

  inline void attribute(std::ostream& s, const char* name, const char* val) {
    s << name << "=\"" << val << "\" ";
  }

  inline void endTag(std::ostream& s, const char* val) {
    s << "</" << val << "> ";
  }

  inline void emptyElement(std::ostream& s, const char* val) {
    s << '<' << val << " /> ";
  }

  /**
   * @brief Generate a simple XML element containing some text.
   * @param s The stream to write the element to.
   * @param tag The tag for the element.
   * @param text The text for the element.
   */
  void simpleTextElement(std::ostream& s,
                         const char* tag,
                         const char* text) {
    simpleStartTag(s, tag);
    s << text;
    endTag(s, tag);
  }

  //* Internal function for formatNodePath
  void formatNodePathInternal(std::ostream& s, 
                              Node const *node) {
    // Fill in parents recursively
    if (node->getParent())
      formatNodePathInternal(s, node->getParent());
    // Put ours at the end
    simpleTextElement(s, LuvFormat::NODE_ID_TAG(), node->getNodeId().c_str());
  }

  /**
   * @brief Generate the XML representation of the path to the node.
   * @param s The stream to write the XML to.
   * @param node The plan node whose path is being constructed.
   */
  void formatNodePath(std::ostream& s, 
                      Node const *node) {
    simpleStartTag(s, LuvFormat::NODE_PATH_TAG());
    formatNodePathInternal(s, node);
    endTag(s, LuvFormat::NODE_PATH_TAG());
  }

  /**
   * @brief Generate the XML representation of the current values of the node's conditions.
   * @param s The stream to write the XML to.
   * @param node The node whose conditions are being extracted.
   */
  void formatConditions(std::ostream& s, 
                        Node *node) 
  {
    simpleStartTag(s, LuvFormat::CONDITIONS_TAG());

    const std::vector<std::string>& allConditions = node->ALL_CONDITIONS();
    for (std::vector<std::string>::const_iterator conditionName = allConditions.begin();
         conditionName != allConditions.end();
         ++conditionName) {
      Expression const *cond = node->getCondition(*conditionName);
      if (cond) {
        simpleTextElement(s, 
                          conditionName->c_str(), 
                          cond->valueString().c_str());
      }
    }

    endTag(s, LuvFormat::CONDITIONS_TAG());
  }

  /**
   * @brief Construct the PlanInfo header XML.
   * @param s The stream to write the XML to.
   * @param block Whether the viewer should block.
   */
  void LuvFormat::formatPlanInfo(std::ostream& s, 
                                 bool block) {
    simpleStartTag(s, PLAN_INFO_TAG());
    simpleTextElement(s, 
                      VIEWER_BLOCKS_TAG(),
                      (block ? TRUE_STR() : FALSE_STR()));
    endTag(s, PLAN_INFO_TAG());
  }



  /**
   * @brief Construct the node state transition XML.
   * @param s The stream to write the XML to.
   * @param prevState The state from which the node is transitioning.
   * @param node The node.
   */
  void LuvFormat::formatTransition(std::ostream& s, 
                                   NodeState /* prevState */,
                                   Node *node) {

    simpleStartTag(s, NODE_STATE_UPDATE_TAG());

    // add state
    simpleTextElement(s, NODE_STATE_TAG(), node->getStateName().c_str());

    // add outcome
    simpleTextElement(s, NODE_OUTCOME_TAG(), outcomeName(node->getOutcome()).c_str());

    // add failure type
    simpleTextElement(s, NODE_FAILURE_TYPE_TAG(), failureTypeName(node->getFailureType()).c_str());
      
    // add the condition states
    formatConditions(s, node);

    // add the path
    formatNodePath(s, node);

    endTag(s, NODE_STATE_UPDATE_TAG());
  }

  /**
   * @brief Construct the assignment XML.
   * @param s The stream to write the XML to.
   * @param dest The expression being assigned to.
   * @param destName The variable name of the expression.
   * @param value The internal representation of the new value.
   */
  void LuvFormat::formatAssignment(std::ostream& s, 
                                   Expression const *dest,
                                   std::string const &destName,
                                   Value const &value) {
    simpleStartTag(s, ASSIGNMENT_TAG());

    // format variable name
    simpleStartTag(s, VARIABLE_TAG());
    // get path to node, if any
    Node const *node = dynamic_cast<Node const *>(dest->asAssignable()->getNode());
    if (node) 
      formatNodePath(s, node);

    // get variable name
    // *** TODO: enhance for array reference ***
    simpleTextElement(s, VARIABLE_NAME_TAG(), destName.c_str());
    endTag(s, VARIABLE_TAG());

    // format variable value
    simpleTextElement(s,
                      VARIABLE_VALUE_TAG(), 
                      value.valueToString().c_str());
    
    endTag(s, ASSIGNMENT_TAG());
  }

  /**
   * @brief Format the message representing a new plan.
   * @param s The stream to write the XML to.
   * @param plan The intermediate representation of the new plan.
   */
  void LuvFormat::formatPlan(std::ostream& s, 
                             pugi::xml_node const plan) {
    assertTrue_2(ALWAYS_FAIL, "Not yet implemented");
    // create a PLEXIL Plan wrapper and stick the plan in it
  }

  /**
   * @brief Construct the message representing a new library node.
   * @param s The stream to write the XML to.
   * @param plan The intermediate representation of the library node.
   */
  void LuvFormat::formatLibrary(std::ostream& s, 
                                pugi::xml_node const libNode) {
    // create a PLEXIL Library wrapper and stick the library node in it
    assertTrue_2(ALWAYS_FAIL, "Not yet implemented");
    simpleStartTag(s, PLEXIL_LIBRARY_TAG());
    // pugi::xml_document* libXml = toXml(libNode);
    // libXml->save(s, " ", PUGI_FORMAT_OPTIONS());
    // delete libXml;
    endTag(s, PLEXIL_LIBRARY_TAG());
  }

}
