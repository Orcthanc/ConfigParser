# ConfigParser
A small config file parser. Supports default values, custom datatypes (as long as their serialization does not contain newlines or whitespaces).

## Usage
It's best to create your own header that defines the custom datatypes and ::Config::to_string and ::Config::from_string overloads. The header should also define your configuration (CFGOPTIONS macro, which calls the CFGOPTION( key_name, data_type, default_value ) multiple times). After that, the header has to include the file Parser.hpp. For an example, see the file include/ConfigDefinitionExample.hpp. After that you can include your own header and create a ::Config::Config object. Additionally to the key_name members as passed by the macro, the Config object exposes the methods read( const std::string& filename ) and write( const std::string& filename ), which are used to read and write the config to file.
