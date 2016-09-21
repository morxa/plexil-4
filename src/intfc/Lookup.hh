/* Copyright (c) 2006-2015, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_LOOKUP_HH
#define PLEXIL_LOOKUP_HH

#include "ExpressionImpl.hh"
#include "NotifierImpl.hh"
#include "State.hh"

namespace PLEXIL
{

  // Forward references
  class CachedValue;
  class ExprVec;
  class StateCacheEntry;
  class ThresholdCache; // local to LookupOnChange

  // OPEN QUESTIONS -
  // - Registry for Lookup, Command param/return types
  //
  // FORMERLY OPEN QUESTIONS -
  // - Local cache for last value? (Yes, for now; revisit when we can profile)
  // - Access to Exec "globals":
  //  StateCacheMap instance - via singleton pattern
  //  cycle count (timestamp) - via g_exec global var

  //
  // Lookup use cases
  //
  // LookupNow
  //  - external i/f queried on demand synchronously
  //  - may be active for more than one Exec cycle, so updates possible
  //
  // LookupOnChange
  //  - grab from external i/f or state cache at initial activation
  //  - data updates triggered by interface
  //  - frequently active for many Exec cycles
  //

  class Lookup : public NotifierImpl
  {
  public:
    Lookup(Expression *stateName,
           bool stateNameIsGarbage,
           ValueType declaredType,
           ExprVec *paramVec = NULL);

    virtual ~Lookup();

    // Standard Expression API
    bool isAssignable() const;
    bool isConstant() const;
    virtual const char *exprName() const;
    void printValue(std::ostream &s) const;
    void printSubexpressions(std::ostream &s) const;

    // Wrap NotifierImpl method
    virtual void addListener(ExpressionListener *l);

    // Common behavior required by NotifierImpl
    void handleActivate();
    void handleDeactivate();
    void handleChange(Expression const *src);

    //
    // Value access
    //

    const ValueType valueType() const;

    // Delegated to the StateCacheEntry in every case
    bool isKnown() const;

    /**
     * @brief Retrieve the value of this Expression.
     * @param The appropriately typed place to put the result.
     * @return True if known, false if unknown or invalid.
     * @note The expression value is not copied if the return value is false.
     */

    virtual bool getValue(bool &) const;        // Boolean
    virtual bool getValue(double &) const;      // Real
    virtual bool getValue(uint16_t &) const;    // enumerations: State, Outcome, Failure, etc.
    virtual bool getValue(int32_t &) const;     // Integer
    virtual bool getValue(std::string &) const; // String

    /**
     * @brief Retrieve a pointer to the (const) value of this Expression.
     * @param ptr Reference to the pointer variable to receive the result.
     * @return True if known, false if unknown or invalid.
     * @note The pointer is not copied if the return value is false.
     */
    virtual bool getValuePointer(std::string const *&ptr) const;
    virtual bool getValuePointer(Array const *&ptr) const; // generic
    virtual bool getValuePointer(BooleanArray const *&ptr) const; // specific
    virtual bool getValuePointer(IntegerArray const *&ptr) const; //
    virtual bool getValuePointer(RealArray const *&ptr) const;    //
    virtual bool getValuePointer(StringArray const *&ptr) const;  //

    /**
     * @brief Get the value of this expression as a Value instance.
     * @return The Value instance.
     */
    virtual Value toValue() const;

    //
    // API to external interface
    //

    /**
     * @brief Notify this Lookup that its value has been updated.
     */
    virtual void valueChanged();

    /**
     * @brief Get this lookup's high and low thresholds.
     * @param high Place to store the high threshold value.
     * @param low Place to store the low threshold value.
     * @return True if this lookup has active thresholds, false otherwise.
     * @note The base class method always returns false.
     */
    virtual bool getThresholds(int32_t &high, int32_t &low);
    virtual bool getThresholds(double &high, double &low);

    // Utility

    /**
     * @brief Get the state for this Lookup, if known.
     * @param result The place to store the State.
     * @return True if fully known, false if not.
     */
    bool getState(State &result) const; 

  protected:

    // Behavior that needs to be augmented for LookupOnChange
    virtual void invalidateOldState(); // called before updating state to new value

    // Shared behavior needed by LookupOnChange
    bool handleChangeInternal(Expression const *src);
    void ensureRegistered();
    void unregister();
    
    // Member variables shared with implementation classes
    State m_cachedState;
    Expression *m_stateName;
    ExprVec *m_paramVec;
    StateCacheEntry* m_entry; // TODO opportunity to use refcounted ptr?
    ValueType m_declaredType;
    bool m_known;
    bool m_stateKnown;
    bool m_stateIsConstant; // allows early caching of state value
    bool m_stateNameIsGarbage;
    bool m_isRegistered;
  };

  class LookupOnChange : public Lookup
  {
  public:
    LookupOnChange(Expression *stateName,
                   bool stateNameIsGarbage,
                   ValueType declaredType,
                   Expression *tolerance,
                   bool toleranceIsGarbage = false,
                   ExprVec *paramVec = NULL);

    ~LookupOnChange();

    const char *exprName() const;

    // Wrappers around Lookup methods
    void handleActivate();
    void handleDeactivate();
    void handleChange(Expression const *exp);
    void valueChanged();

    bool getThresholds(int32_t &high, int32_t &low);
    bool getThresholds(double &high, double &low);

    /**
     * @brief Retrieve the value of this Expression.
     * @param The appropriately typed place to put the result.
     * @return True if known, false if unknown or invalid.
     * @note The expression value is not copied if the return value is false.
     */

    bool getValue(int32_t &) const;     // Integer
    bool getValue(double &) const;      // Real

    /**
     * @brief Get the value of this expression as a Value instance.
     * @return The Value instance.
     */
    Value toValue() const;

    // Wrap NotifierImpl method
    virtual void addListener(ExpressionListener *l);

  private:
    // Prohibit default, copy, assign
    LookupOnChange();
    LookupOnChange(const LookupOnChange &);
    LookupOnChange &operator=(const LookupOnChange &);

    // Wrapper for base class method
    virtual void invalidateOldState();

    // Internal helper
    bool updateInternal(bool valueChanged);

    // Unique member data
    ThresholdCache *m_thresholds;
    CachedValue *m_cachedValue;
    Expression *m_tolerance;
    bool m_toleranceIsGarbage;
  };

} // namespace PLEXIL

#endif // PLEXIL_LOOKUP_HH
