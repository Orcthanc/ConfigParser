/*
 * =====================================================================================
 *
 *       Filename:  ConfigDefinitionExample.hpp
 *
 *    Description:  An example config definition
 *
 *        Version:  1.0
 *        Created:  03/30/2020 03:37:00 PM
 *       Revision:  none
 *
 *         Author:  Samuel Knoethig (), samuel@knoethig.net
 *
 * =====================================================================================
 */
#pragma once

#include <string>
#include <stdint.h>

struct OwnType {
	size_t val;
};

namespace Config {
	template <typename T>
	inline std::string to_string( const T& val );

	template <typename T>
	inline T from_string( const std::string& val );

	template <>
	inline std::string to_string<OwnType>( const OwnType& t ){
		return std::to_string( t.val );
	}

	template <>
	inline OwnType from_string<OwnType>( const std::string& str ){
		return { std::stoul( str ) };
	}
}

#define CFGOPTIONS					\
	CFGOPTION( a, uint32_t, 10 )	\
	CFGOPTION( b, bool, true )		\
	CFGOPTION( ASafdsfasf, std::string, "qerqwer" )

#include "Parser.hpp"
