/**
 * \file
 * \brief SoftwareTimerFunctionTypesTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SoftwareTimerFunctionTypesTestCase.hpp"

#include "distortos/DynamicSoftwareTimer.hpp"

#include <malloc.h>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Regular function for a software timer - arguments are passed by reference and by value.
 *
 * \param [out] sharedVariable is a reference to shared variable
 * \param [in] magicValue is the value which will be assigned to shared variable
 */

void regularFunction(uint32_t& sharedVariable, const uint32_t magicValue)
{
	sharedVariable = magicValue;
}

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// Functor class is a state-less functor
class Functor
{
public:

	/**
	 * \brief Functor's function for a software timer - arguments are passed by reference and by value.
	 *
	 * \param [out] sharedVariable is a reference to shared variable
	 * \param [in] magicValue is the value which will be assigned to shared variable
	 */

	void operator()(uint32_t& sharedVariable, const uint32_t magicValue) const
	{
		sharedVariable = magicValue;
	}
};

/// Object class is a class with state
class Object
{
public:

	/**
	 * \brief Object's constructor.
	 *
	 * \param [in] magicValue is the value which will be assigned to contained variable variable
	 */

	constexpr explicit Object(const uint32_t magicValue) :
			variable_{},
			magicValue_{magicValue}
	{

	}

	/**
	 * \brief Object's function for a software timer - arguments are passed in the object instance.
	 */

	void function()
	{
		variable_ = magicValue_;
	}

	/// \return contained variable
	uint32_t getVariable() const
	{
		return variable_;
	}

private:

	/// contained variable - used to return result
	uint32_t variable_;

	/// value which will be assigned to contained variable - used as argument
	const uint32_t magicValue_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SoftwareTimerFunctionTypesTestCase::run_() const
{
	constexpr auto singleDuration = TickClock::duration{1};

	const auto allocatedMemory = mallinfo().uordblks;

	// software timer with regular function
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0xd16807d9};

		auto regularFunctionSoftwareTimer = makeDynamicSoftwareTimer(regularFunction, std::ref(sharedVariable),
				magicValue);
		regularFunctionSoftwareTimer.start(singleDuration);
		while (regularFunctionSoftwareTimer.isRunning() == true)
		{

		}

		if (sharedVariable != magicValue)
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	// software timer with state-less functor
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x6affeaca};

		auto functorSoftwareTimer = makeDynamicSoftwareTimer(Functor{}, std::ref(sharedVariable), magicValue);
		functorSoftwareTimer.start(singleDuration);
		while (functorSoftwareTimer.isRunning() == true)
		{

		}

		if (sharedVariable != magicValue)
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	// software timer with member function of object with state
	{
		constexpr uint32_t magicValue {0x7a919ba8};
		Object object {magicValue};

		auto objectSoftwareTimer = makeDynamicSoftwareTimer(&Object::function, std::ref(object));
		objectSoftwareTimer.start(singleDuration);
		while (objectSoftwareTimer.isRunning() == true)
		{

		}

		if (object.getVariable() != magicValue)
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	// software timer with capturing lambda
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x05da8b27};

		auto capturingLambdaSoftwareTimer = makeDynamicSoftwareTimer(
				[&sharedVariable, magicValue]()
				{
					sharedVariable = magicValue;
				});
		capturingLambdaSoftwareTimer.start(singleDuration);
		while (capturingLambdaSoftwareTimer.isRunning() == true)
		{

		}

		if (sharedVariable != magicValue)
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
