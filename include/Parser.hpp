/*
 * =====================================================================================
 *
 *       Filename:  Parser.hpp
 *
 *    Description:  The Parser
 *
 *        Version:  1.0
 *        Created:  03/30/2020 03:35:46 PM
 *       Revision:  none
 *
 *         Author:  Samuel Knoethig (), samuel@knoethig.net
 *
 * =====================================================================================
 */

/**
 *	#define CFGOPTIONS								\
 *		CFGOPTION( name, datatype, defaultvalue )	\
 *		CFGOPTION( xres, uint32_t, 1920 )			\
 *		CFGOPTION( fullscreen, bool, false )
 *
 *	All passed datatypes need to have Config::to_string<datatype> (defaults to std::to_string) and Config::from_string (defaults to std::sto? where applicable and T( const std::string ) otherwise ) defined
 */

#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <functional>

#include <string.h>

#include "CommonConversions.hpp"

namespace Config {

	/**
	 *	Enum containing all the Config Options
	 */
	enum class CfgOption: int {
		#define CFGOPTION( name, type, value ) \
		name,
		CFGOPTIONS
		#undef CFGOPTION
	};

	struct Config {
		Config(
				std::function<void(const std::string&)> error_callback = []( const std::string& msg ){ std::cout << "Error: " << msg << std::endl; },
				std::function<void(const std::string&)> warning_callback = []( const std::string& msg ){ std::cout << "Warning: " << msg << std::endl; }
				): error_callback( error_callback ), warning_callback( warning_callback ){
			#define CFGOPTION( name, type, value ) \
			name = ( value );
			CFGOPTIONS
			#undef CFGOPTION
		};

		bool write( const std::string& filename ){
			std::ofstream file( filename );

			if( !file.is_open() ){
				error_callback( "Could not write config to file: \"" + filename + "\"" );
				return false;
			}

			#define CFGOPTION( name, type, value ) \
			file << #name << " = " << to_string<type>( name ) << std::endl;
			CFGOPTIONS
			#undef CFGOPTION

			return true;
		}

		bool read( const std::string& filename ){
			std::ifstream file( filename );

			if( !file.is_open() ){
				warning_callback( "Could not read config file: \"" + filename + "\"" );
				return false;
			}

			char buffer[1024];

			while( file.getline( buffer, 1024 )){
				std::string name( strtok( buffer, ":= " ));
				name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
				std::string value( strtok( nullptr, ":= " ));
				value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

				if( name[0] == '#' )
					continue;

				#define CFGOPTION( name_, type_, value_ ) \
				if( #name_ == name ){ \
					name_ = from_string<type_>( value ); \
				}
				CFGOPTIONS
				#undef CFGOPTION
			}

			return true;
		}

		template<int enumval>
		void get(){}

		#define CFGOPTION( name, type, value )	\
		template <>						\
		type& get<CfgOption::name>(){	\
			return name;				\
		}
		#undef CFGOPTION

		#define CFGOPTION( name, type, value ) \
		type name;
		CFGOPTIONS
		#undef CFGOPTION

		private:
			std::function<void(const std::string&)> error_callback;
			std::function<void(const std::string&)> warning_callback;
	};
}
