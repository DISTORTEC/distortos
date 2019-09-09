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
		FAIL(failure);
	}
	else
	{
		CAPTURE(failure);
		CHECK(failure.empty());
	}
}

}	// namespace trompeloeil
