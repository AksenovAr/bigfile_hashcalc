#include "fileconverter_to_hash.h"
#include "cmd_parser.h"
#include "custom_exceptions.h"
#include "block_info.h"

#include <openssl/md5.h>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <functional>
#include <boost/iostreams/device/mapped_file.hpp>

using namespace boost::iostreams;

void fileconverter_to_hash::doWork(const cdm_parser& parser)
{
	unsigned char result[MD5_DIGEST_LENGTH];
	std::string s_path_to_input_file = parser.get_input_file_path();
	uintmax_t i_block_size = parser.get_block_size();

	std::streamsize const length = std::distance(std::istreambuf_iterator<char>(std::ifstream(s_path_to_input_file).rdbuf()), {});

	m_count_of_threads = length / i_block_size;
	uintmax_t i_balance = length - (m_count_of_threads * i_block_size);
	if (i_balance) m_count_of_threads++;

	boost::iostreams::mapped_file_source file( s_path_to_input_file, mapped_file_base::mapmode::priv, length+i_balance );
	std::fill( (unsigned char*) file.data()+length, (unsigned char*) file.data()+length+i_balance, '0');

	for (uintmax_t thread_counter = 0; thread_counter < m_count_of_threads; thread_counter++)
	{

		std::unique_ptr< block_info > info;
		try
		{
			info.reset(new block_info(thread_counter, i_block_size, file));
		}
		catch (std::bad_alloc&)
		{
			//throw Imp::ResourceLimitation("Out of memory");
		}
		catch (boost::system::system_error& e)
		{

		}
		catch (std::exception& e)
		{
		}

		info->m_thread = std::thread([self = (this), p = info.get()]() { self->block_reading(p); });

		// Don't destroy a joinable thread. The thread will end soon anyway, so we can just detach it.
		if (info->m_thread.joinable())
			info->m_thread.detach();

		m_threads_container.insert(std::move(info));
	}

	md5data_to_string();
}

void fileconverter_to_hash::block_reading(block_info* info)
{
	std::unique_lock<std::mutex> lck(mut);

	uintmax_t offset = info->m_block_size * info->m_number;
	MD5((unsigned char*) info->m_file.data() + offset, info->m_block_size, info->result);

	--m_count_of_threads;
	cv.notify_one();

}

void fileconverter_to_hash::add_handler(handler_t& h)
{
	m_output_fun = h;
}

void fileconverter_to_hash::md5data_to_string()
{
	std::unique_lock<std::mutex> lck(mut);
	while(m_count_of_threads != 0)
	{
		cv.wait(lck);
	}

	input_stream_list::iterator it = m_threads_container.begin();
	for (; it !=  m_threads_container.end(); ++it)
	{
		m_all_block_hash += std::string(reinterpret_cast<char*>((*it)->result));
	}

	m_output_fun(m_all_block_hash);
}
