#include "byte_stream.hh"
#include<string>
using namespace std;
// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}
using namespace std;
//string内存分配按照:(n*16-1),特别注意string的capacity特性
ByteStream::ByteStream(const size_t capacity):_capacity(capacity),_str(string()),_isEnd(false),_error(false),_write_count(0),_read_count(0){

	_str.reserve(1000000);//这个调用不用管，用_capacity来维护了,开比较大只是在调度中发现有超时的情况，分析后估计是多次取网页要扩大str，copy的开销大

}

size_t ByteStream::write(const string &data) {
	size_t ll=data.size();
    if(ll > remaining_capacity()){
    	set_error();
	ll=remaining_capacity();
    }
    _write_count+=ll;//write_count要每次都加
    	_str+=data.substr(0,ll);

    return ll;//返回值是每次加的长度
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {

    //substr完美解决当len超过size的情况
    return _str.substr(0,len);

}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
	//本来erase能完美解决这个问题的，但要计算read_count
	size_t ll=len;
	if (len > _str.size()) {
		ll=_str.size();
    }
	_read_count+=ll;
	_str.erase(0,ll);
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string s=peek_output(len);
	pop_output(len);
    return s;
}

void ByteStream::end_input() {
	_isEnd=true;
}

bool ByteStream::input_ended() const {
	return _isEnd;
}

size_t ByteStream::buffer_size() const {
	return _str.size() ;
}

bool ByteStream::buffer_empty() const {
	return  buffer_size()==0;
}

//这个比较特殊，竟然要buffer为空且结束了输入
bool ByteStream::eof() const {
	return buffer_empty()&&input_ended();
}

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }


size_t ByteStream::remaining_capacity() const {
	return _capacity-_str.size();
}
