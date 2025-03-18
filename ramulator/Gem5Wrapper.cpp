#include <map>

#include "Gem5Wrapper.h"
#include "Config.h"
#include "Request.h"
#include "MemoryFactory.h"
#include "Memory.h"
#include "HBM.h"

using namespace ramulator;

Gem5Wrapper::Gem5Wrapper(const Config& configs, int cacheline, int id)
{
    const string& std_name = configs["standard"];
	assert(std_name == string("HBM"));
    //assert(name_to_func.find(std_name) != name_to_func.end() && "unrecognized standard name");
    mem = MemoryFactory<HBM>::create(configs, cacheline, id);

    if (!mem) {
        std::cerr << "Error: Failed to create Memory<HBM> in Gem5Wrapper!" << std::endl;
        throw std::runtime_error("Memory<HBM> initialization failed.");
    }

    tCK = mem->clk_ns();
}

Gem5Wrapper::~Gem5Wrapper() {
    delete mem;
}

void Gem5Wrapper::tick()
{
    mem->tick();
}

void Gem5Wrapper::init(Request& req) const {
    mem->init(req);
}

bool Gem5Wrapper::send(Request& req)
{
    if (!mem) {
        std::cerr << "Error: Attempt to call send() on uninitialized memory!" << std::endl;
        return false;
    }
    return mem->send(req);
}

void Gem5Wrapper::finish(void) {
    if (mem) mem->finish();
}
