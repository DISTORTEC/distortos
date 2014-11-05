/**
 * \file
 * \brief MutexPriorityProtocolTestCase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-05
 */

#ifndef TEST_MUTEX_MUTEXPRIORITYPROTOCOLTESTCASE_HPP_
#define TEST_MUTEX_MUTEXPRIORITYPROTOCOLTESTCASE_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Executes typical "priority inversion" scenario using mutexes with PriorityProtect protocol.
 *
 * 5 threads are executed and they are expected to execute and preempt each other in exact sequence.
 *
 * Legend for timing diagrams:
 * - Lxx - lock of mutex xx
 * - Uxx - unlock of mutex xx
 * - x12 - operation on mutex shared between T1 and T2
 * - x13 - operation on mutex shared between T1 and T3
 * - x23 - operation on mutex shared between T2 and T3
 * - x35 - operation on mutex shared between T3 and T5
 * - solid outline with background - thread is running
 * - dashed outline without background - thread is preempted
 * - gray background - thread doesn't hold any mutexes
 * - orange background - thread holds one mutex
 * - red background - thread holds two mutexes
 * - pink background - thread holds three mutexes
 * - arrows - priority changes due to operation of PriorityProtect protocol
 *
 * Vertical scale - priority, T1 - lowest, T5 - highest
 * Horizontal scale - time
 *
 * \image html without-priority-protect-scenario.png "Without \"PriorityProtect\" protocol enabled"
 *
 * This scenario demonstrates typical priority inversion problem - highest priority thread T5 is delayed because lower
 * priority thread T4 executes and doesn't allow other lower priority threads (which hold mutex required by thread T5)
 * to continue. This is what happens in each marked time point:
 * - at t0 thread T1 starts executing
 * - at t1 thread T1 successfully locks mutex "13"
 * - at t2 thread T1 successfully locks mutex "12"
 * - at t3 thread T2 starts executing and preempts thread T1
 * - at t4 thread T2 successfully locks mutex "23"
 * - at t5 thread T2 tries to lock mutex "12" which is currently held by thread T1, thread T1 is allowed to continue,
 * - at t6 thread T3 starts executing and preempts thread T1
 * - at t7 thread T3 successfully locks mutex "35"
 * - at t8 thread T2 tries to lock mutex "23" which is currently held by thread T2, thread T1 is allowed to continue (as
 * thread T2 is blocked on mutex "12")
 * - at t9 thread T4 starts executing and preempts thread T1
 * - at t10 thread T5 starts executing and preempts thread T4
 * - at t11 thread T5 tries to lock mutex "35" which is currently held by thread T3, thread T4 is allowed to continue as
 * a thread with highest priority that is "runnable"
 * - at t12 thread T4 terminates and allows thread T1 to continue (as thread T5 is blocked on mutex "35", thread T3 - on
 * mutex "23" and thread T2 - on mutex "12")
 * - at t13 thread T1 unlocks mutex "13"
 * - at t14 thread T1 unlocks mutex "12", ownership of the mutex is passed to thread T2, which is allowed to continue
 * - at t15 thread T2 unlocks mutex "23", ownership of the mutex is passed to thread T3, which is allowed to continue
 * - at t16 thread T3 successfully locks mutex "13"
 * - at t17 thread T3 unlocks mutex "35", ownership of the mutex is passed to thread T5, which is allowed to continue
 * - at t18 thread T5 unlocks mutex "35"
 * - at t19 thread T5 terminates and allows thread T3 to continue
 * - at t20 thread T3 unlocks mutex "23"
 * - at t21 thread T3 unlocks mutex "13"
 * - at t22 thread T3 terminates and allows thread T2 to continue
 * - at t23 thread T2 unlocks mutex "12"
 * - at t24 thread T2 terminates and allows thread T1 to continue
 * - at t25 thread T1 terminates
 *
 * \image html with-priority-protect-scenario.png "With \"PriorityProtect\" protocol enabled"
 *
 * This is the same situation as in previous scenario, but now all mutexes have PriorityProtect protocol enabled.
 * Changes of priority due to this protocol allow highest priority thread T5 to acquire ownership of the mutex much
 * sooner than previously. Additionally the most important critical sections (where mutex with high priority ceiling is
 * used) are as short as possible. This scenario has 9 context switches, which is much less than 16 in scenario without
 * PriorityProtect protocol. This is what happens in each marked time point:
 * - at t0 thread T1 starts executing
 * - at t1 thread T1 successfully locks mutex "13" and it's priority is boosted to the priority ceiling of this mutex,
 * which is equal to priority of thread T3
 * - at t2 thread T1 successfully locks mutex "12", there is no priority boost as priority ceiling of mutex "12" is
 * lower than priority ceiling of mutex "13"
 * - at t3 thread T2 becomes "runnable", but is not allowed to run because effective priority of thread T1 is higher
 * - at t4 thread T3 becomes "runnable", but is not allowed to run because effective priority of thread T1 is equal and
 * thread T1 was first
 * - at t5 thread T1 unlocks mutex "13" and it's priority is lowered to the priority ceiling of another mutex it holds
 * ("12"), which is equal to priority of thread T2, thread T1 is preempted and thread T3 is allowed to continue
 * - at t6 thread T3 successfully locks mutex "35" and it's priority is boosted to the priority ceiling of this mutex,
 * which is equal to priority of thread T5
 * - at t7 thread T3 successfully locks mutex "23", there is no priority boost as priority ceiling of mutex "23" is
 * lower than priority ceiling of mutex "35"
 * - at t8 thread T3 successfully locks mutex "13", there is no priority boost as priority ceiling of mutex "13" is
 * lower than priority ceiling of mutex "35", thread T4 becomes "runnable", but is not allowed to run because effective
 * priority of thread T3 is higher
 * - at t9 thread T5 becomes "runnable", but is not allowed to run because effective priority of thread T3 is equal and
 * thread T3 was first
 * - at t10 thread T3 unlocks mutex "35" and it's priority is lowered to its original value (other mutexes it currently
 * holds - "23" and "13" - have priority ceiling equal to priority of thread T3), thread T3 is preempted and thread T5
 * is allowed to continue
 * - at t11 thread T5 successfully locks mutex "35", there is no priority boost as priority ceiling of mutex "35" is
 * equal to original priority of thread T5
 * - at t12 thread T5 unlocks mutex "35"
 * - at t13 thread T5 terminates and allows thread T4 to continue
 * - at t14 thread T4 terminates and allows thread T3 to continue
 * - at t15 thread T3 unlocks mutex "23"
 * - at t16 thread T3 unlocks mutex "13"
 * - at t17 thread T3 terminates and allows thread T1 (its effective priority is equal to priority of thread T2, but
 * thread T1 was first) to continue
 * - at t18 thread T1 unlocks mutex "12" and it's priority is lowered to its original value, thread T1 is preempted and
 * thread T2 is allowed to continue
 * - at t19 thread T2 successfully locks mutex "23" and it's priority is boosted to the priority ceiling of this mutex,
 * which is equal to priority of thread T3
 * - at t20 thread T2 successfully locks mutex "12", there is no priority boost as priority ceiling of mutex "12" is
 * lower than priority ceiling of mutex "23"
 * - at t21 thread T2 unlocks mutex "23" and it's priority is lowered to its original value
 * - at t22 thread T2 unlocks mutex "12"
 * - at t23 thread T2 terminates and allows thread T1 to continue
 * - at t24 thread T1 terminates
 */

class MutexPriorityProtocolTestCase : public TestCase
{
private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	virtual bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_MUTEX_MUTEXPRIORITYPROTOCOLTESTCASE_HPP_
