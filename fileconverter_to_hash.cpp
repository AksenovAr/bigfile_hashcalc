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

uintmax_t block_info::m_file_lenght = 0;
uintmax_t block_info::m_block_size = 0;
bool ready = false;

void fileconverter_to_hash::doWork(const cdm_parser& parser)
{
	unsigned char result[MD5_DIGEST_LENGTH];
	const std::string s_path_to_input_file = parser.get_input_file_path();
	block_info::m_file_lenght = std::distance(std::istreambuf_iterator<char>(std::ifstream(s_path_to_input_file).rdbuf()), {});
	block_info::m_block_size = parser.get_block_size();
	m_count_of_threads = m_countdown_counter_of_threads = block_info::m_file_lenght / block_info::m_block_size;

	file.open(s_path_to_input_file, block_info::m_file_lenght);

	for (uintmax_t thread_counter = 0; thread_counter <= m_count_of_threads; thread_counter++)
	{
		std::unique_ptr< block_info > info;
		try
		{
			info.reset(new block_info(thread_counter, file));
		}
		catch (std::bad_alloc&)
		{
			//throw Imp::ResourceLimitation("Out of memory");
			std::cout << "Error1";
		}
		catch (boost::system::system_error& e)
		{
			std::cout << "Error1";
		}
		catch (std::exception& e)
		{
			std::cout << "Error2";
		}

		info->m_thread = std::thread([self = (this), p = info.get()]() { self->block_reading(p); });

		// Don't destroy a joinable thread. The thread will end soon anyway, so we can just detach it.
		if (info->m_thread.joinable())
			info->m_thread.detach();

		m_threads_container.insert(std::move(info));
	}

	md5data_to_string();
}

void fileconverter_to_hash::block_reading(block_info* p_info)
{
	std::unique_lock<std::mutex> lck(mut);
	uintmax_t i_offset = p_info->m_block_size * p_info->m_number;
	uintmax_t i_balance = block_info::m_file_lenght - i_offset;

	if (i_balance >= p_info->m_block_size)
	{
		MD5((unsigned char*) p_info->m_file.data() + i_offset, p_info->m_block_size, p_info->result);
	}
	else
	{
		// balance less then block size
		// by the condition increase size till block_size and fill data with zero
	//	static std::vector<unsigned char> vec( p_info->m_block_size, '0');
	//	std::copy( p_info->m_file.begin()+i_offset, p_info->m_file.end(), vec.begin());

	//	MD5( (unsigned char*) *vec.begin(), p_info->m_block_size, p_info->result);
	}

	--m_countdown_counter_of_threads;
	if (m_countdown_counter_of_threads == 0)
	{
		ready = true;
		cv.notify_all();
	}
}

void fileconverter_to_hash::add_handler(handler_t& h)
{
	m_output_fun = h;
}

void fileconverter_to_hash::md5data_to_string()
{
	std::unique_lock<std::mutex> lck(mut);
	cv.wait( lck, [] () { return ready; });

	input_stream_list::iterator it = m_threads_container.begin();
	for (; it !=  m_threads_container.end(); ++it)
	{
		m_all_block_hash += std::to_string( (*it)->m_number );
		m_all_block_hash += "------------";
		m_all_block_hash += std::string(reinterpret_cast<char*>((*it)->result));
	}

	m_output_fun(m_all_block_hash);
}

fileconverter_to_hash::~fileconverter_to_hash()
{
	file.close();
	std::cout << " Dtor singltone ! \n" ;
}
