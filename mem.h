/*#ifndef MEM_H
#define MEM_H

#include "Gem5Wrapper.h"
#include "Config.h"
#include "def.h"
#include "Statistics.h"

#include <set>
#include <vector>
#include <iostream>
#include <exception>
#include <functional>
#include <unordered_map>
#include <sstream>

using namespace std;
using namespace ramulator;

namespace DRAM{
    
class single_mem {
    private:
        Gem5Wrapper memory;
        function<int(addr_type)> addr2id;
    
    public:
        const addr_type swap_start;
    
        single_mem(uint64_t swap_start) : swap_start(swap_start) {

            const Config cfg = run_cfg.get_ramulator_config();
    
            //init
            memory = Gem5Wrapper(cfg, run_cfg.cacheline_byte_amount, 0);  //only 1 memory module, idx = 0
    
            //addr to id mapping
            addr2id = [](addr_type addr) -> int {
                return 0;
            };
        }

        ~single_mem() {
            memory.finish();
        }
    
        //try send req
        template<bool is_read>
        bool try_send(Request& req) const {
            const int idx = addr2id(req.addr);  //idx = 0
            return memory.try_send<is_read>(req);
        }
    
        //send req
        bool send(Request& req) {
            const int idx = addr2id(req.addr);
            return memory.send(req);
        }
    
        //init req
        void init(Request& req) const {
            const int idx = addr2id(req.addr);
            memory.init(req);
        }

        void advance() {
            memory.tick();
        }

        addr_type capacity() const {
            return 1 << 32;
        }

        addr_type swap_space() const {
            return capacity() - swap_start;
        }
    };
}
    
#endif
*/


#ifndef MEM_H
#define MEM_H

#include "Gem5Wrapper.h"
#include "Config.h"
#include "def.h"
#include "Statistics.h"

#include <functional>
#include <mutex>
#include <unordered_map>

using namespace ramulator;

namespace MEM {
class single_mem {
public:
    using CallbackType = std::function<void(addr_type addr, int data)>;

private:
    ramulator::Gem5Wrapper memory;
    std::function<int(addr_type)> addr2id;
    mutable std::mutex mtx;
    const addr_type swap_start;

    std::unordered_map<addr_type, int> pending_requests;
    CallbackType global_callback;
    mutable std::mutex req_mtx;

public:
    single_mem(uint64_t swap_start, const runtime_config& run_cfg) 
        : swap_start(swap_start),
          memory(run_cfg.get_ramulator_config(), run_cfg.cacheline_byte_amount, 0) 
    {
        addr2id = [](addr_type addr) { return 0; };
    }

    void register_global_callback(const CallbackType& cb) {
        std::lock_guard<std::mutex> lock(req_mtx);
        global_callback = cb;
    }

    bool send(Request& req) {
        std::lock_guard<std::mutex> lock(req_mtx);
        if (!memory.send(req)) {
            std::cerr << "Error: Memory send failed for addr " << req.addr << std::endl;
            return false;
        }
        pending_requests[req.addr] = req.coreid;
        return true;
    }

    void notify_data_ready(addr_type addr, int data) {
        std::lock_guard<std::mutex> lock(req_mtx);
        if (pending_requests.find(addr) == pending_requests.end()) {
            std::cerr << "Warning: Received unexpected data for addr " << addr << std::endl;
            return;
        }

        if (global_callback) {
            global_callback(addr, data);
        }
        pending_requests.erase(addr);
    }

    template<bool is_read>
    bool try_send(Request& req) const {
        std::lock_guard<std::mutex> lock(mtx);
        return memory.try_send<is_read>(req);
    }

    void init(Request& req) const {
        memory.init(req);
    }

    void advance() {
        memory.tick();
    }

    addr_type capacity() const {
        return 1ULL << 32;
    }

    addr_type swap_space() const {
        return capacity() - swap_start;
    }
};
}

#endif
