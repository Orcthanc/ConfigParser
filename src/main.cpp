#include "Parser.hpp"

enum class eOptions {
	eAsdf,
	eHjkl,
};

int main( int argc, char** argv ){
	Config::ConfigParser<eOptions> parser;
	parser.add({ eOptions::eAsdf, "asdf", 'a', 0, "asdfasdfasdf" });
	parser.add({ eOptions::eHjkl, "jkl", 'j', 0, "jklhjklhjkl" });

	bool exit = false;

	auto options = parser.read_config( "./config.cfg", argc, argv, &exit );
}
