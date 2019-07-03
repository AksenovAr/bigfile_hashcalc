#ifndef BLOCK_INFO_H
#define BLOCK_INFO_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <boost/iostreams/device/mapped_file.hpp>

struct block_info
{
	struct by_number_id {};

	uintmax_t get_number_id()	const {	return m_number;	}

	//! Default constructor
	explicit block_info( uintmax_t number, boost::iostreams::mapped_file_source& file )
		:m_number(number),
		m_file(file)
	{
		memset(result, '0', MD5_DIGEST_LENGTH);
	}

	//! Destructor
	~block_info()
	{
	}

	// Copying prohibited
	block_info(block_info const&) = delete;
	block_info(block_info const&&) = delete;
	block_info& operator= (block_info const&) = delete;
	block_info& operator= (block_info const&&) = delete;

	unsigned char result[MD5_DIGEST_LENGTH];

	uintmax_t m_number;
	static uintmax_t m_block_size;
	static uintmax_t m_file_lenght;
	//!
	boost::iostreams::mapped_file_source& m_file;
	//! thread
	std::thread m_thread;
};

#endif // BLOCK_INFO_H
