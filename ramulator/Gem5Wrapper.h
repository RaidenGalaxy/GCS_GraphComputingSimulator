#ifndef __GEM5_WRAPPER_H
#define __GEM5_WRAPPER_H

#include <string>

#include "Config.h"
#include "Memory.h"

using namespace std;

namespace ramulator
{

class Request;
class MemoryBase;

class Gem5Wrapper 
{
private:
    MemoryBase *mem;
public:
    double tCK;
    Gem5Wrapper(const Config& configs, int cacheline, int id);
    ~Gem5Wrapper();
    void tick();
    void init(Request& req) const ;
    bool send(Request& req);
    void finish(void);
    template<bool is_read>
    bool try_send(Request& req) const {
        if (is_read) return mem->try_send_read(req);
        else return mem->try_send_write(req);
    }
};

} /*namespace ramulator*/

#endif /*__GEM5_WRAPPER_H*/
