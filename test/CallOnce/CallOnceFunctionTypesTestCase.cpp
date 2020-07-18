/**
 * \file
 * \brief CallOnceFunctionTypesTestCase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CallOnceFunctionTypesTestCase.hpp"

#include "distortos/callOnce.hpp"

#include <cstdint>

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
 * \brief Regular function for callOnce() - arguments are passed by reference and by value.
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
	 * \brief Functor's function for callOnce() - arguments are passed by reference and by value.
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
	 * \brief Object's function for callOnce() - arguments are passed in the object instance.
	 */

	void function()
	{
		variable_ = magicValue_;
	}

	/**
	 * \return contained variable
	 */

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

bool CallOnceFunctionTypesTestCase::run_() const
{
	// callOnce() with regular function
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x0567e79d};

		OnceFlag onceFlag;
		callOnce(onceFlag, regularFunction, sharedVariable, magicValue);

		if (sharedVariable != magicValue)
			return false;
	}

	// callOnce() with state-less functor
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0x28aaaa21};

		OnceFlag onceFlag;
		callOnce(onceFlag, Functor{}, sharedVariable, magicValue);

		if (sharedVariable != magicValue)
			return false;
	}

	// callOnce() with member function of object with state
	{
		constexpr uint32_t magicValue {0x82081678};
		Object object {magicValue};

		OnceFlag onceFlag;
		callOnce(onceFlag, &Object::function, object);

		if (object.getVariable() != magicValue)
			return false;
	}

	// callOnce() with capturing lambda
	{
		uint32_t sharedVariable {};
		constexpr uint32_t magicValue {0xc394a4e5};

		OnceFlag onceFlag;
		callOnce(onceFlag,
				[&sharedVariable, magicValue]()
				{
					sharedVariable = magicValue;
				});

		if (sharedVariable != magicValue)
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
