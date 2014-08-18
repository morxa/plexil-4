/* Copyright (c) 2006-2013, Universities Space Research Association (USRA).
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

#ifndef POSIX_TIME_ADAPTER_H
#define POSIX_TIME_ADAPTER_H

#include "TimeAdapter.hh"

#include <time.h> // *** better be POSIX time.h! ***

namespace PLEXIL
{

  /**
   * @brief An interface adapter using standard POSIX time facilities
   *        to implement LookupNow and LookupOnChange.
   */
  class PosixTimeAdapter : public TimeAdapter
  {
  public:
    /**
     * @brief Constructor.
     * @param execInterface Reference to the parent AdapterExecInterface object.
     */
    PosixTimeAdapter(AdapterExecInterface& execInterface);

    /**
     * @brief Constructor from configuration XML.
     * @param execInterface Reference to the parent AdapterExecInterface object.
     * @param xml A const reference to the XML element describing this adapter
     * @note The instance maintains a shared pointer to the XML.
     */
    PosixTimeAdapter(AdapterExecInterface& execInterface, 
                     const pugi::xml_node& xml);

    /**
     * @brief Destructor.
     */
    virtual ~PosixTimeAdapter();

    /**
     * @brief Stops the adapter.  
     * @return true if successful, false otherwise.
     */
    bool stop();

    /**
     * @brief Get the current time from the operating system.
     * @return A double representing the current time.
     */
    double getCurrentTime();

  protected:

    /**
     * @brief Initialize signal handling for the process.
     * @return True if successful, false otherwise.
     */
    virtual bool configureSignalHandling();

    /**
     * @brief Construct and initialize the timer as required.
     * @return True if successful, false otherwise.
     */
    virtual bool initializeTimer();

    /**
     * @brief Shut down and delete the timer as required.
     * @return True if successful, false otherwise.
     */
    virtual bool deleteTimer();

    /**
     * @brief Set the timer.
     * @param date The Unix-epoch wakeup time, as a double.
     * @return True if the timer was set, false if clock time had already passed the wakeup time.
     */
    virtual bool setTimer(double date);

    /**
     * @brief Stop the timer. 
     * @return True if successful, false otherwise.
     */
    virtual bool stopTimer();

    /**
     * @brief Initialize the wait thread signal mask.
     * @return True if successful, false otherwise.
     */
    virtual bool configureWaitThreadSigmask(sigset_t* mask);

    /**
     * @brief Initialize the sigwait mask.
     * @param Pointer to the mask.
     * @return True if successful, false otherwise.
     */
    virtual bool initializeSigwaitMask(sigset_t* mask);

  private:

    // Deliberately unimplemented
    PosixTimeAdapter();
    PosixTimeAdapter(const PosixTimeAdapter &);
    PosixTimeAdapter & operator=(const PosixTimeAdapter &);

    //
    // Member variables
    //

    sigevent m_sigevent;
    timer_t m_timer;
  };

}

#endif // POSIX_TIME_ADAPTER_H
