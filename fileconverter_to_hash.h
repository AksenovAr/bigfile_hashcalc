#ifndef FILECONVERTER_TO_HASH_H
#define FILECONVERTER_TO_HASH_H

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <boost/config.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/atomic/atomic.hpp>
#include "block_info.h"

using boost::multi_index_container;
using namespace boost::multi_index;

class cdm_parser;

typedef std::function<void (std::string&)> handler_t;

class fileconverter_to_hash
{
	public :

	handler_t m_output_fun;

	static std::shared_ptr<fileconverter_to_hash> getInstance()
	{
		std::cout <<  " Enter in getInstance \n ";
		static std::shared_ptr<fileconverter_to_hash> obj(new fileconverter_to_hash);
		return obj;
	}

	void add_handler(handler_t& h);
	void doWork(const cdm_parser& parser);
	void block_reading(block_info* info);
	void md5data_to_string();

	fileconverter_to_hash() { std::cout << " Ctor singltone ! \n" ;} ;
	~fileconverter_to_hash() { std::cout << " Dtor singltone ! \n" ;} ;

	typedef std::unique_ptr< block_info > info_ptr;

	using input_stream_list = multi_index_container< info_ptr,
		indexed_by<
			ordered_unique<
				tag<block_info::by_number_id>, const_mem_fun<block_info, uintmax_t, &block_info::get_number_id>
			>
		>
	>;

	private:
	std::string m_input_file_path;
	std::string m_output_file_path;
	std::string m_all_block_hash;
	uint32_t m_block_size;

	input_stream_list m_threads_container;
	boost::atomic< uint32_t > m_count_of_threads;
	std::mutex mut;
	std::condition_variable cv;

	fileconverter_to_hash&  operator = (const fileconverter_to_hash& )  = delete;
	fileconverter_to_hash&  operator = (const fileconverter_to_hash&& ) = delete;
	fileconverter_to_hash( const fileconverter_to_hash& ) = delete;
	fileconverter_to_hash( const fileconverter_to_hash&& ) = delete;
};

#endif // FILECONVERTER_TO_HASH_H
