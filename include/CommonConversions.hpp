/*
 * =====================================================================================
 *
 *       Filename:  CommonConversions.hpp
 *
 *    Description:  Contains the most common conversions between string and Type
 *
 *        Version:  1.0
 *        Created:  03/30/2020 04:34:54 PM
 *       Revision:  none
 *
 *         Author:  Samuel Knoethig (), samuel@knoethig.net
 *
 * =====================================================================================
 */
#pragma once

#include <string>
#include <stdexcept>

namespace Config {
	template <typename T>
	inline std::string to_string( const T& val ){
		return std::to_string( val );
	}

	template <typename T>
	inline T from_string( const std::string& val ){
		return T( val );
	}

	template <>
	inline std::string to_string<std::string>( const std::string& val ){
		return val;
	}

	template <>
	inline std::string from_string<std::string>( const std::string& val ){
		return val;
	}

	template <>
	inline std::string to_string<bool>( const bool& val ){
		return val ? "true" : "false";
	}

	template <>
	inline bool from_string<bool>( const std::string& val ){
		if( val == "true" || val == "TRUE" || val == "True" || val == "yes" || val == "Yes" || val == "YES" ){
			return true;
		}

		return atoi( val.c_str() );
	}

	template <>
	inline int from_string<int>( const std::string& val ){
		return std::stoi( val );
	}

	template <>
	inline unsigned from_string<unsigned>( const std::string& val ){
		unsigned long temp = std::stoul( val );
		if( std::numeric_limits<unsigned>::max() < temp )
			throw std::overflow_error( "from_string<unsigned>( " + val + " )" );
		return static_cast<unsigned>( temp );
	}

	template <>
	inline long from_string<long>( const std::string& val ){
		return std::stol( val );
	}

	template <>
	inline unsigned long from_string<unsigned long>( const std::string& val ){
		return std::stoul( val );
	}

	template <>
	inline long long from_string<long long>( const std::string& val ){
		return std::stoll( val );
	}

	template <>
	inline unsigned long long from_string<unsigned long long>( const std::string& val ){
		return std::stoull( val );
	}

	template <>
	inline float from_string<float>( const std::string& val ){
		return std::stof( val );
	}

	template <>
	inline double from_string<double>( const std::string& val ){
		return std::stod( val );
	}

	template <>
	inline long double from_string<long double>( const std::string& val ){
		return std::stold( val );
	}	
}
