#ifndef __REQUEST_H
#define __REQUEST_H

#include <vector>
#include <functional>

using namespace std;

using idx_type = int64_t;
using addr_type = uint64_t;

namespace ramulator
{

class Request
{
public:
    bool is_first_command;
    addr_type addr;
    // long addr_row;
    vector<int> addr_vec;
    // specify which core this request sent from, for virtual address translation
    int coreid;

    // -1 for meta data, -2 for short list, >=0 for send list
    idx_type tag;

    enum class Type
    {
        READ,
        WRITE,
        REFRESH,
        POWERDOWN,
        SELFREFRESH,
        EXTENSION,
        MAX
    } type;

    long arrive = -1;
    long depart = -1;
    function<void(Request&)> callback; // call back with more info

    Request(addr_type addr, Type type, idx_type tag = 0, int coreid = 0)
        : is_first_command(true), addr(addr), tag(tag), coreid(coreid), type(type),
      callback([](Request& req){}) {}

    Request(addr_type addr, Type type, function<void(Request&)> callback, idx_type tag = 0, int coreid = 0)
        : is_first_command(true), addr(addr), tag(tag), coreid(coreid), type(type), callback(callback) {}

    Request(vector<int>& addr_vec, Type type, function<void(Request&)> callback, idx_type tag = 0, int coreid = 0)
        : is_first_command(true), addr(-1), addr_vec(addr_vec), tag(tag), coreid(coreid), type(type), callback(callback) {}

    Request()
        : is_first_command(true), addr(-1), tag(0), coreid(0) {}
};

} /*namespace ramulator*/

#endif /*__REQUEST_H*/

