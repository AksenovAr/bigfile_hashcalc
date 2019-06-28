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
	//-----------------------
	uintmax_t get_number_id()	const {	return m_number;	}

	//! Default constructor
	explicit block_info( uintmax_t number, uintmax_t block_size, boost::iostreams::mapped_file_source& file )
		:m_number(number),
		m_block_size(block_size),
		m_file(file)
	{
	}

	//! Destructor
	~block_info()
	{
	}

	// Copying prohibited
	block_info(block_info const&) = delete;
	block_info& operator= (block_info const&) = delete;

	unsigned char result[MD5_DIGEST_LENGTH];
	std::string m_md5hash; // 32symbols
	uintmax_t m_number;
	uintmax_t m_block_size;
	//!
	boost::iostreams::mapped_file_source& m_file;
	//! thread
	std::thread m_thread;
};

#endif // BLOCK_INFO_H
