#pragma once

#include <string>
#include <map>
#include <fstream>
#include <functional>
#include <iostream>

#include <string.h>
#include <getopt.h>

namespace Config {

	/**
	 * 	A option that can be passed to the parser
	 */
	template <typename eOptionEntry>
	struct Option;

	/**
	 * 	An object containing a configuration
	 */
	template <typename eOptionEntry>
	struct Config;

	/**
	 * 	Used to create a config object from a file and Options array
	 */
	template <typename eOptionEntry>
	struct ConfigParser;





/*----------------------------------------------------------------------------*/
	template <typename eOptionEntry>
	struct Option {
	/**
	 * 	Constructor
	 *	@param optionID id used in the map returned by Config
	 *	@param name name read by the cmd-line parser and/or fileparser
	 *	@param shortcut One-letter-shortcut read by the cmd-line-parser
	 *	@param args Indicates if the arg requires a parameter (0 = no, 1 = yes, 2 = maybe)
	 *	@param description description displayed when invoked with --help
	 */
		Option( eOptionEntry optionID, std::string name, int shortcut, int args, std::string description ):
			optionID( optionID ), name( std::move( name )), shortcut( shortcut ), args( args ), description( std::move( description )){}

		eOptionEntry optionID;		/**< The id to wich the resulting value will be mapped. Should be unique and probably an enum */
		std::string name;			/**< The name used to save to file and passed with '--' */
		int shortcut;				/**< The oneletter name passed with '-' when reading cmd-line args. */
		int args;					/**< Indicates if the option needs a argument 0 = no, 1 = yes, 2 = maybe */
		std::string description;	/**< The description shown when passing --help or -h */
		std::string default_val;	/**< The default value assigned if none is passed */
	};

	template <typename eOptionEntry>
	struct Config {
		std::map<eOptionEntry, std::string> options;
		std::map<eOptionEntry, std::string> id_to_name;
	};

	template <typename eOptionEntry>
	struct ConfigParser {

		/**
		 *	Adds an option to the Parser
		 *	@param o The option.
		 */
		bool add( Option<eOptionEntry>&& o ){
			if( id_to_option.find( o.optionID ) != id_to_option.end() ){
				last_error = "ID already registered by command " + id_to_option.find( o.optionID )->second.name;
				return false;
			}

			name_to_id.emplace( o.name, o.optionID );
			if( o.shortcut )
				shortcut_to_id.emplace( o.shortcut, o.optionID );

			id_to_option.emplace( o.optionID, std::move( o ));
			return true;
		}

		/**
		 *	Returns the last error in human readable format
		 */
		std::string error_msg(){
			return last_error;
		}

		/**
		 *	Reads a configfile and commandlineoptions
		 *	@param filename The name of the configfile or nullptr
		 *	@param argc Number of args in argv (at least one)
		 *	@param argv Arguments where argv[0] is the executable path
		 *	@param[out] exit Gets set to true if the program is requested to exit (--help)
		 */
		Config<eOptionEntry> read_config( const char* filename, int argc, char** argv, bool* exit ){
			// Reads in order cmd, file, default, because insert doesn't overwrite
			Config<eOptionEntry> config;

			// Read cmdline
			std::vector<struct option> options;
			std::string valid_chars = "h";
			options.push_back({ "help", no_argument, nullptr, 'h' });
			for( auto& o: id_to_option ){
				options.push_back({ o.second.name.c_str(), o.second.args, nullptr, o.second.shortcut });
				if( o.second.args < 128 ){
					valid_chars += o.second.shortcut;
					if( o.second.args ){
						valid_chars += ':';
						if( o.second.args == 2 )
							valid_chars += ':';
					}
				}
			}
			options.push_back({ nullptr, 0, nullptr, 0 });

			int c;


			while (( c = getopt_long( argc, argv, valid_chars.c_str(), options.data(), nullptr )) != -1 ){
				if( c == 1 || c == 0 )
					continue;
				if( c == '?' || c == ':' ){
					error_callback( "Encountered unknown cmd option. Try --help. Aborting..." );
					*exit = true;
					return Config<eOptionEntry>();
				}
				if( c == 'h' ){
					for( auto& o: id_to_option ){
						printf( "\t-%c, --%-32s %s\n", o.second.shortcut, o.second.name.c_str(), o.second.description.c_str() );
					}
					*exit = true;
					return Config<eOptionEntry>();
				}
				if( id_to_option.at( shortcut_to_id.at( c )).args ){
					config.options.emplace( shortcut_to_id.at( c ), optarg ? optarg : "" );
				} else {
					config.options.emplace( shortcut_to_id.at( c ), "" );
				}
			}


			// Read file
			if( filename ){
				std::ifstream infile( filename );
				if( !infile.fail()){
					char line[1024];
					char* temp = nullptr;

					while ( infile.getline( line, 1023 )){
						temp = strtok( line, "=:" );
						auto name = name_to_id.find( temp );
						if( name == name_to_id.end() ){
							warn_callback( "Encountered unknown config option " + std::string( temp ) + ". Skipping..." );
							continue;
						}
						config.options.emplace( name->second, strtok( nullptr, "=:" ));
					}
				}
			}
			// Default and init id_to_name
			for ( auto& o: id_to_option ) {
				config.options.emplace( o.first, o.second.default_val );
				config.id_to_name.emplace( o.first, o.second.name );
			}

			return config;
		}

		ConfigParser( 
				std::function<void(std::string)> error_callback = 
						[]( std::string msg ){ std::cout << msg << std::endl; },
				std::function<void(std::string)> warning_callback =
						[]( std::string msg ){ std::cout << msg << std::endl; })
					: last_error( "" ), id_to_option(), shortcut_to_id(), name_to_id(), error_callback( error_callback ), warn_callback( warning_callback ){}

		private:
			std::string last_error;
			std::map<eOptionEntry, Option<eOptionEntry>> id_to_option;
			std::map<char, eOptionEntry> shortcut_to_id;
			std::map<std::string, eOptionEntry> name_to_id;
			std::function<void(std::string)> error_callback;
			std::function<void(std::string)> warn_callback;

	};
}
