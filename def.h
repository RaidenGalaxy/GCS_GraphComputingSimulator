#ifndef DEF_H
#define DEF_H

#include <iostream>
#include <sstream>
#include "Config.h"
#include "dataset.h"
#include <bitset>

namespace MEM {

#define KB_SIZE                             ((uint64_t)1<<10)
#define MB_SIZE                             ((uint64_t)1<<20)
#define GB_SIZE                             ((uint64_t)1<<30)

// whether to print performance log
#define PRINT_PERF                          (true)
#define PRINT_COMPUTE_ALLOC                 (true)

// debug option
#define GLOBAL_THROW                        (true)
#define LOAD_10_PERCENT                     (false)

string format_memory(uint64_t num);

	struct runtime_config {
		// level option
		bool enable_swap = true;
		bool single_round = true;
		bool merge_packet = false;
		bool A2C_order = true;
		uint64_t fix_agg_compute = 0;

		// network option
		uint64_t network_bandwidth = 150;
		uint64_t network_pipe_latency = 500;
		uint64_t network_router_capacity = 1536 * KB_SIZE;

		// compute option
		uint64_t pe_row_number = 8;
		uint64_t agg_unit_number = 256;
		uint64_t comb_unit_number = 1;
		uint64_t comb_concurrency = 8;

		// on-chip memory option
		int agg_buf_bank = 32;
		int reorder_item = 16;
		uint64_t we_cap = 256 * KB_SIZE;
		uint64_t ce_cap = 64 * KB_SIZE;
		uint64_t rob_cap = 64 * KB_SIZE;
		uint64_t sync_cap = 512 * KB_SIZE;

		uint64_t agg_buf_capacity = 1 * MB_SIZE;
		uint64_t send_fifo_capacity = 512 * KB_SIZE;
		uint64_t manage_fifo_capacity = 128 * KB_SIZE;
		
		uint64_t comb_buf_capacity = 256 * KB_SIZE;
		uint64_t weight_buf_capacity = 2 * MB_SIZE;

		float feature_length_factor = 1;
		// off-chip memory option, currently only HBM
		int cacheline_bits = 6;
		int channel_bits = 4;
		int rank_bits = 2;

		uint64_t cacheline_byte_amount = 64;
		uint64_t channel_amount = 16;
		uint64_t memory_frequency = 500;
		uint64_t executor_frequency = 1000;

		ramulator::Config get_ramulator_config() const {
			stringstream ss;
			ss << "standard = HBM\n";
			ss << "speed = HBM_1Gbps\n";
			ss << "org = HBM_4Gb\n";
			ss << "channels = " << (1 << channel_bits) << '\n';
			ss << "ranks = " << (1 << rank_bits) << '\n';
			ss << "cpu_tick = 32\n";
			ss << "mem_tick = 5\n";
			ramulator::Config ret;
			ret.parse(ss);
			ret.set_core_num(1);
			return ret;
		}
		
		void complete() {
			cacheline_byte_amount = (uint64_t)1 << cacheline_bits;
			channel_amount = (uint64_t)1 << channel_bits;
		}

		runtime_config() = default;

		string get_config_option() const {
			const int base = 8;
			const string ident = string(3, ' ');
			stringstream ss;
			ss << boolalpha;
			ss << string(base, '=') << " MultiGCN Runtime Configuration " << string(base, '=') << '\n';

			ss << string(base + 9, '-') << " Level Option " << string(base + 9, '-') << '\n';
			ss << ident << "Enable SWAP in Off-Chip Memory:   " << enable_swap << '\n';
			ss << ident << "Execute in Single-Round Mode:     " << single_round << '\n';
			ss << ident << "Enable Multicast in Network:      " << merge_packet << '\n';
			ss << ident << "Execute in AGG -> COMB order:     " << A2C_order << '\n';

			ss << string(base + 8, '-') << " Network Option " << string(base + 8, '-') << '\n';
			ss << ident << "Pipe Bandwidth (GBps):            " << network_bandwidth << '\n';
			ss << ident << "Pipe Latency (Cycles):            " << network_pipe_latency << '\n';
			ss << ident << "Router Capacity (Bytes):          " << format_memory(network_router_capacity) << '\n';

			ss << string(base + 8, '-') << " Compute Option " << string(base + 8, '-') << '\n';
			ss << ident << "PE Array Row Number:              " << pe_row_number << '\n';
			ss << ident << "AGG Unit Number:                  " << agg_unit_number << '\n';
			ss << ident << "COMB Unit Number:                 " << comb_unit_number << '\n';
			ss << ident << "COMB Concurrency:                 " << comb_concurrency << '\n';

			ss << string(base + 4, '-') << " On-Chip Memory Options " << string(base + 4, '-') << '\n';
			ss << ident << "Write-Edge FIFO Capacity:         " << format_memory(we_cap) << '\n';
			ss << ident << "Commit-Edge FIFO Capacity:        " << format_memory(ce_cap) << '\n';
			ss << ident << "Feature Reorder Buffer Capacity:  " << format_memory(rob_cap) << '\n';
			ss << ident << "Feature Reorder Buffer Item Num:  " << reorder_item << '\n';
			ss << ident << "Sync-Out FIFO Capacity:           " << format_memory(sync_cap) << '\n';
			ss << ident << "AGG Buffer Capacity:              " << format_memory(agg_buf_capacity) << '\n';
			ss << ident << "AGG Buffer Bank Number:           " << agg_buf_bank << '\n';
			ss << ident << "COMB Buffer Capacity:             " << format_memory(comb_buf_capacity) << '\n';
			ss << ident << "Send FIFO Capacity:               " << format_memory(send_fifo_capacity) << '\n';
			ss << ident << "Manage FIFO Capacity:             " << format_memory(manage_fifo_capacity) << '\n';
			ss << ident << "Weight Buffer Capacity:           " << format_memory(weight_buf_capacity) << '\n';

			ss << string(base + 4, '-') << " Off-Chip Memory Option " << string(base + 4, '-') << '\n';
			ss << ident << "Cache Line Bits (Amount):         " << cacheline_bits << " (" << cacheline_byte_amount << ")\n";
			ss << ident << "Channel Bits (Amount):            " << channel_bits << " (" << channel_amount << ")\n";
			ss << ident << "Rank Bits (Amount):               " << rank_bits << " (" << ((uint64_t)1 << rank_bits) << ")\n";

			ss << string(base * 2 + 32, '=') << '\n';
			string ret = ss.str();
			return ret;
		}

	};

	runtime_config run_cfg;

}

#endif // !MULTI_GCN_DEF_H
