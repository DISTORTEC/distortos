/**
 * \file
 * \brief ThreadFunctionTypesTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-28
 */

#include "ThreadFunctionTypesTestCase.hpp"

#include "distortos/scheduler/StaticThread.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {128};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Regular function for a thread - arguments are passed by reference and by value.
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
	 * \brief Functor's function for a thread - arguments are passed by reference and by value.
	 *
	 * \param [out] sharedVariable is a reference to shared variable
	 * \param [in] magicValue is the value which will be assigned to shared variable
	 */

	void operator()(uint32_t& sharedVariable, const uint32_t magicValue)
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

	constexpr Object(const uint32_t magicValue) :
			variable_{},
			magicValue_{magicValue}
	{

	}

	/**
	 * \brief Object's function for a thread - arguments are passed in the object instance.
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

	/// value which will be assigned to contained variable variable - used as argument
	const uint32_t magicValue_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadFunctionTypesTestCase::run_() const
{
	using scheduler::makeStaticThread;

	// thread with regular function
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x394e3bae};

		auto regularFunctionThread = makeStaticThread<testThreadStackSize>(UINT8_MAX, regularFunction,
				std::ref(sharedVariable), magicValue);
		regularFunctionThread.start();
		regularFunctionThread.join();

		if (sharedVariable != magicValue)
			return false;
	}

	// thread with state-less functor
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x2c2b7c30};

		auto functorThread = makeStaticThread<testThreadStackSize>(UINT8_MAX, Functor{}, std::ref(sharedVariable),
				magicValue);
		functorThread.start();
		functorThread.join();

		if (sharedVariable != magicValue)
			return false;
	}

	// thread with member function of object with state
	{
		constexpr uint32_t magicValue {0x33698f0a};
		Object object {magicValue};

		auto objectThread = makeStaticThread<testThreadStackSize>(UINT8_MAX, &Object::function, std::ref(object));
		objectThread.start();
		objectThread.join();

		if (object.getVariable() != magicValue)
			return false;
	}

	// thread with capturing lambda
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x24331acb};

		auto capturingLambdaThread = makeStaticThread<testThreadStackSize>(UINT8_MAX,
				[&sharedVariable, magicValue]()
				{
					sharedVariable = magicValue;
				});
		capturingLambdaThread.start();
		capturingLambdaThread.join();

		if (sharedVariable != magicValue)
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
