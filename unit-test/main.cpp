#define CATCH_CONFIG_MAIN	// use default main()
#include "catch.hpp"
#include "trompeloeil.hpp"

namespace trompeloeil
{

template<>
void reporter<specialized>::send(const severity severity, const char* const file, const unsigned long line,
		const char* const message)
{
	std::ostringstream outputStream;
	if (line != 0)
		outputStream << file << ':' << line << std::endl;
	outputStream << message;
	const auto failure = outputStream.str();
	if (severity == severity::fatal)
	{
#ifdef CATCH_CONFIG_PREFIX_ALL
		CATCH_FAIL(failure);
#else
		FAIL(failure);
#endif
	}
	else
	{
#ifdef CATCH_CONFIG_PREFIX_ALL
		CATCH_CAPTURE(failure);
		CATCH_CHECK(failure.empty());
#else
		CAPTURE(failure);
		CHECK(failure.empty());
#endif
	}
}

template<>
void reporter<specialized>::sendOk(const char* const trompeloeilMockCallsDoneCorrectly)
{
#ifdef CATCH_CONFIG_PREFIX_ALL
	CATCH_REQUIRE(trompeloeilMockCallsDoneCorrectly != nullptr);
#else
	REQUIRE(trompeloeilMockCallsDoneCorrectly != nullptr);
#endif
}

}	// namespace trompeloeil
