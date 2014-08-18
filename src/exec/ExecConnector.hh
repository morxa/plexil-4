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

#ifndef EXEC_CONNECTOR_HH
#define EXEC_CONNECTOR_HH

#include "NodeConstants.hh"

namespace PLEXIL
{
  // Forward references
  class Assignment;
  class ExecListenerHub;
  class ExternalInterface;
  class Node;
  class PlexilNode;

  /**
   * @brief Abstract class representing the key API of the PlexilExec. Facilitates testing.
   */
  class ExecConnector {
  public:
    ExecConnector() {}
    virtual ~ExecConnector() {}
    virtual void notifyNodeConditionChanged(Node *node) = 0;
    virtual void handleConditionsChanged(Node *node, NodeState newState) = 0;

    /**
     * @brief Schedule this assignment for execution.
     */
    virtual void enqueueAssignment(Assignment *assign) = 0;

    /**
     * @brief Schedule this assignment for execution.
     */
    virtual void enqueueAssignmentForRetraction(Assignment *assign) = 0;

    /**
     * @brief Mark node as finished and no longer eligible for execution.
     */
    virtual void markRootNodeFinished(Node *node) = 0;

    // Needed by TestExternalInterface

    /**
     * @brief Add the plan under the node named by the parent.
     * @param plan The intermediate representation of the plan.
     * @return True if succesful, false otherwise.
     */
    virtual bool addPlan(PlexilNode *plan) = 0;

    /**
     * @brief Add the node to the known libraries.
     * @param plan The intermediate representation of the node.
     */
    virtual void addLibraryNode(PlexilNode *lib) = 0;

    /**
     * @brief Get the named library.
     */
    virtual PlexilNode const *getLibrary(std::string const &name) const = 0;

    /**
     * @brief Begins a single "macro step" i.e. the entire quiescence cycle.
     */
    virtual void step(double startTime) = 0; // *** FIXME: use real time type ***

    /**
     * @brief Returns true if the Exec needs to be stepped.
     */
    virtual bool needsStep() const = 0;

    /**
     * @brief Set the ExecListenerHub instance.
     */
    virtual void setExecListenerHub(ExecListenerHub *hub) = 0;

    // Needed by ExecApplication

    virtual void deleteFinishedPlans() = 0;

    virtual bool allPlansFinished() const = 0;
  };

  // Global pointer to the exec instance
  extern ExecConnector *g_exec;

}

#endif // EXEC_CONNECTOR_HH
