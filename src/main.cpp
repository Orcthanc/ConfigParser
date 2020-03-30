#include "ConfigDefinitionExample.hpp"

Config::Config cfg;

int main( int argc, char** argv ){
	cfg.read( "./config.cfg" );
	std::cout << cfg.a << std::endl;
	cfg.write( "./config.cfg" );
}
