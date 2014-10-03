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

#include "ExpressionFactory.hh"

#include "ArrayReference.hh"
#include "ConcreteExpressionFactory.hh"
#include "Debug.hh"
#include "Error.hh"
#include "expression-schema.hh"
#include "NodeConnector.hh"
#include "ParserException.hh"
#include "parser-utils.hh"
#include "pugixml.hpp"

#include <map>

namespace PLEXIL
{

  static std::map<std::string, ExpressionFactory*>& expressionFactoryMap()
  {
    static std::map<std::string, ExpressionFactory*> sl_map;
    return sl_map;
  }

  static void registerExpressionFactory(const std::string& name,
                                        ExpressionFactory* factory) 
  {
    check_error_1(factory != NULL);
    checkError(expressionFactoryMap().find(name) == expressionFactoryMap().end(),
               "Error:  Attempted to register a factory for name \"" << name <<
               "\" twice.");
    expressionFactoryMap()[name] = factory;
    debugMsg("ExpressionFactory:registerFactory",
             "Registered factory for name \"" << name << "\"");
  }

  ExpressionFactory::ExpressionFactory(const std::string& name)
    : m_name(name)
  {
    registerExpressionFactory(m_name, this);
  }

  ExpressionFactory::~ExpressionFactory()
  {
  }

  const std::string& ExpressionFactory::getName() const
  {
    return m_name;
  }

  Expression *createExpression(pugi::xml_node const expr,
                               NodeConnector *node)
    throw (ParserException)
  {
    bool dummy;
    return createExpression(expr, node, dummy);
  }

  Expression *createExpression(pugi::xml_node const expr,
                               NodeConnector *node,
                               bool& wasCreated)
    throw (ParserException)
  {
    checkParserExceptionWithLocation(expr.type() == pugi::node_element,
                                     expr,
                                     "createExpression: argument is not an XML element");
    std::string const name = expr.name();
    // Delegate to factory
    debugMsg("createExpression", " name = " << name);
    std::map<std::string, ExpressionFactory*>::const_iterator it =
      expressionFactoryMap().find(name);
    checkParserException(it != expressionFactoryMap().end(),
                         "createExpression: No factory registered for name \"" << name << "\".");
    Expression *retval = it->second->allocate(expr, node, wasCreated);
    debugMsg("createExpression",
             " Created " << (wasCreated ? "" : "reference to ") << retval->toString());
    return retval;
  }

  //
  // createAssignable
  //

  Assignable *createAssignable(pugi::xml_node const expr,
                               NodeConnector *node,
                               bool& wasCreated)
    throw (ParserException)
  {
    checkParserExceptionWithLocation(expr.type() == pugi::node_element,
                                     expr,
                                     "Not an XML element");
    assertTrue_2(node, "createAssignable: Internal error: Null node argument");
    Expression *resultExpr = NULL;
    if (testTagSuffix(VAR_TAG, expr))
      resultExpr = createExpression(expr, node, wasCreated);
    else if (testTag(ARRAYELEMENT_TAG, expr))
      resultExpr = createMutableArrayReference(expr, node, wasCreated);
    else
      checkParserExceptionWithLocation(ALWAYS_FAIL,
                                       expr,
                                       "Invalid Assignment or InOut alias target");
    assertTrue_2(resultExpr, "createAssignable: Internal error: Null expression")
    checkParserExceptionWithLocation(resultExpr->isAssignable(),
                                     expr,
                                     "Expression is not assignable");
    return resultExpr->asAssignable();
  }

  void purgeExpressionFactories()
  {
    for (std::map<std::string, ExpressionFactory*>::iterator it =
           expressionFactoryMap().begin();
         it != expressionFactoryMap().end();
         ++it) {
      ExpressionFactory* tmp = it->second;
      it->second = NULL;
      delete tmp;
    }
    expressionFactoryMap().clear();
  }

}
