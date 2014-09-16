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

#include "ArrayLiteralFactory.hh"

#include "Constant.hh"
#include "Error.hh"
#include "expression-schema.hh"
#include "NodeConnector.hh"
#include "parser-utils.hh"

#include "pugixml.hpp"

#include <cstdlib>

namespace PLEXIL
{
  ArrayLiteralFactory::ArrayLiteralFactory(std::string const &name)
    : ExpressionFactory(name)
  {
  }

  ArrayLiteralFactory::~ArrayLiteralFactory()
  {
  }

  // *** DELETE ME ***
  Expression *ArrayLiteralFactory::allocate(PlexilExpr const * /* expr */,
                                            NodeConnector * /* node */,
                                            bool & /* wasCreated */) const
  {
    assertTrue_2(ALWAYS_FAIL, "Nothing should ever call this method!");
    return NULL;
  }

  template <typename T>
  Expression *createArrayLiteral(char const *eltTypeName, pugi::xml_node const &expr)
  {
    // gather elements
    std::vector<T> values;

    pugi::xml_node thisElement = expr.first_child();
    size_t i = 0;
    std::vector<size_t> unknowns;
    while (thisElement) {
      checkTagSuffix(VAL_TAG, thisElement);
      // Check type
      const char* thisElementTag = thisElement.name();
      checkParserExceptionWithLocation(0 == strncmp(thisElementTag, 
                                                    eltTypeName, 
                                                    strlen(thisElementTag) - strlen(VAL_TAG)),
                                       thisElement,
                                       "Type mismatch: element " << thisElementTag
                                       << " in array value of type " << eltTypeName);

      // Get array element value
      checkNotEmpty(thisElement);
      const char* thisElementValue = thisElement.first_child().value();
      T temp;
      if (parseValue<T>(thisElementValue, temp)) // will throw if format error
        values.push_back(temp);
      else {
        unknowns.push_back(i);
        values.push_back(T()); // push a placeholder for unknown
      }
      thisElement = thisElement.next_sibling();
      ++i;
    }

    // Handle unknowns here
    ArrayImpl<T> initVals(values);
    for (std::vector<size_t>::const_iterator it = unknowns.begin();
         it != unknowns.end();
         ++it)
      initVals.setElementUnknown(*it);

    return new Constant<ArrayImpl<T> >(initVals);
  }

  template <>
  Expression *createArrayLiteral<std::string>(char const *eltTypeName, pugi::xml_node const &expr)
  {
    // gather elements
    std::vector<std::string> values;

    pugi::xml_node thisElement = expr.first_child();
    size_t i = 0;
    while (thisElement) {
      checkTagSuffix(VAL_TAG, thisElement);
      // Check type
      const char* thisElementTag = thisElement.name();
      checkParserExceptionWithLocation(0 == strncmp(thisElementTag, 
                                                    eltTypeName, 
                                                    strlen(thisElementTag) - strlen(VAL_TAG)),
                                       thisElement,
                                       "Type mismatch: element " << thisElementTag
                                       << " in array value of type \"" << eltTypeName);
      values.push_back(std::string(thisElement.first_child().value()));
      thisElement = thisElement.next_sibling();
      ++i;
    }
    return new Constant<ArrayImpl<std::string> >(ArrayImpl<std::string>(values));
  }

  Expression *ArrayLiteralFactory::allocate(pugi::xml_node const &expr,
                                            NodeConnector * /* node */,
                                            bool &wasCreated) const
  {
    // confirm that we have an array value
    checkTag(ARRAY_VAL_TAG, expr);

    // confirm that we have an element type
    checkAttr(TYPE_TAG, expr);
    const char* valueType = expr.attribute(TYPE_TAG).value();
    ValueType valtyp = parseValueType(valueType);
    checkParserExceptionWithLocation(valtyp != UNKNOWN_TYPE,
                                     expr, // should be attribute
                                     "Unknown array element Type value \"" << valueType << "\"");

    wasCreated = true;

    switch (valtyp) {
    case BOOLEAN_TYPE:
      return createArrayLiteral<bool>(valueType, expr);

    case INTEGER_TYPE:
      return createArrayLiteral<int32_t>(valueType, expr);

    case REAL_TYPE:
      return createArrayLiteral<double>(valueType, expr);

    case STRING_TYPE:
      return createArrayLiteral<std::string>(valueType, expr);

    default:
      checkParserExceptionWithLocation(ALWAYS_FAIL,
                                       expr, // should be attribute
                                       "Invalid or unimplemented array element Type value \"" << valueType << "\"");
      return NULL;
    }
  }



} // namespace PLEXIL
