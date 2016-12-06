/* Copyright (c) 2006-2016, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_ALIAS_HH
#define PLEXIL_ALIAS_HH

#include "Assignable.hh"
#include "NotifierImpl.hh"

namespace PLEXIL
{
  // Forward declaration
  class Value;

  /**
   * @class Alias
   * @brief A read-only proxy for another expression.
   * @note Most commonly used in library nodes, but also anywhere
   *       read-only access to a mutable expression is needed.
   */
  class Alias :
    public NotifierImpl
  {
  public:
    Alias(NodeConnector *node, // *** is this needed?? ***
          char const *name,
          Expression *original = NULL,
          bool garbage = false);
    virtual ~Alias();

    //
    // Expression API
    //

    char const *getName() const;
    char const *exprName() const;
    ValueType valueType() const;
    bool isKnown() const;
    virtual bool isAssignable() const;
    bool isConstant() const;
    Expression *getBaseExpression();
    Expression const *getBaseExpression() const;

    void printValue(std::ostream &s) const;

    /**
     * @brief Retrieve the value of this Expression.
     * @param The appropriately typed place to put the result.
     * @return True if known, false if unknown or invalid.
     */
    bool getValue(Boolean &var) const;
    bool getValue(uint16_t &var) const;
    bool getValue(Integer &var) const;
    bool getValue(Real &var) const;
    bool getValue(String &var) const;

    /**
     * @brief Retrieve a pointer to the (const) value of this Expression.
     * @param ptr Reference to the pointer variable to receive the result.
     * @return True if known, false if unknown or invalid.
     */
    bool getValuePointer(String const *&ptr) const;
    bool getValuePointer(Array const *&ptr) const;
    bool getValuePointer(BooleanArray const *&ptr) const;
    bool getValuePointer(IntegerArray const *&ptr) const;
    bool getValuePointer(RealArray const *&ptr) const;
    bool getValuePointer(StringArray const *&ptr) const;

    /**
     * @brief Get the value of this expression as a Value instance.
     * @return The Value instance.
     */
    Value toValue() const;

    // Wrap NotifierImpl method
    virtual void addListener(ExpressionListener *l);

  protected:

    // The expression being aliased.
    Expression *m_exp;
    // Parent node
    NodeConnector *m_node;
    // Name in the parent node
    char const *m_name;

  private:

    bool m_garbage;

    // Disallow default, copy, assign
    Alias();
    Alias(const Alias &);
    Alias &operator=(const Alias &);

  };

} // namespace PLEXIL

#endif // PLEXIL_ALIAS_HH
