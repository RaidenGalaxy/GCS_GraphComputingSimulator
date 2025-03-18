#ifndef MULTI_GCN_DATASET_H
#define MULTI_GCN_DATASET_H

#include <map>
#include <functional>
using namespace std;

using dtype = float;

enum class dataset
{
	mcf,
	aids,
	collab_all,

	simple,    // for test, 10 vertices
	citeseer,  // CS
	collab,    // CL
	cora,      // CR
	imdb,      // IB
	pubmed,    // PB
	reddit,    // RD, 114M edges
	orkut,
	patents,
	livejournal,
	livejournal_d,
	asia_osm,
	stokes,
	twitter,   // TW, 41M nodes, 1468M edges
	friendster,
	r19,
	r20,
	r21,
	r22,
	r23
};

class less_dataset {
public:
	bool operator()(const dataset a, const dataset b) const {
		return static_cast<int>(a) < static_cast<int>(b);
	}
};

const map<dataset, string> dataset2str = {
	{dataset::simple,        "simple"},
	{dataset::pubmed,        "pubmed"},
	{dataset::reddit,        "reddit"},
	{dataset::orkut,         "orkut"},
	{dataset::patents,       "patents"},
	{dataset::livejournal_d, "livejournal"},
	{dataset::asia_osm,      "asia_osm"},
	{dataset::stokes,        "stokes"},
	{dataset::twitter,       "twitter"},
	{dataset::friendster,    "friendster"},
	{dataset::r19,           "RMAT-19N-24E"},
	{dataset::r20,           "RMAT-20N-25E"},
	{dataset::r21,           "RMAT-21N-26E"},
	{dataset::r22,           "RMAT-22N-27E"},
	{dataset::r23,           "RMAT-23N-28E"}
};

const map<dataset, string> dataset2abbr = {
	{dataset::simple,        "SP"},
	{dataset::pubmed,        "PB"},
	{dataset::reddit,        "RD"},
	{dataset::orkut,         "OR"},
	{dataset::patents,       "PT"},
	{dataset::livejournal_d, "LJ"},
	{dataset::asia_osm,      "AO"},
	{dataset::stokes,        "ST"},
	{dataset::twitter,       "TW"},
	{dataset::friendster,    "FS"},
	{dataset::r19,          "R19"},
	{dataset::r20,          "R20"},
	{dataset::r21,          "R21"},
	{dataset::r22,          "R22"},
	{dataset::r23,          "R23"}
};

const map<string, dataset> str2dataset = {
	{"SP",  dataset::simple},        {"simple",      dataset::simple},
	{"PB",  dataset::pubmed},        {"pubmed",      dataset::pubmed},
	{"RD",  dataset::reddit},        {"reddit",      dataset::reddit},
	{"OR",  dataset::orkut},         {"orkut",       dataset::orkut},
	{"PT",  dataset::patents},       {"patents",     dataset::patents},
	{"LJ",  dataset::livejournal_d}, {"livejournal", dataset::livejournal_d},
	{"AO",  dataset::asia_osm},      {"asia_osm",    dataset::asia_osm},
	{"ST",  dataset::stokes},        {"stokes",      dataset::stokes},
	{"TW",  dataset::twitter},       {"twitter",     dataset::twitter},
	{"FS",  dataset::friendster},    {"friendster",  dataset::friendster},
	{"R19", dataset::r19},           {"R20",         dataset::r20},
	{"R21", dataset::r21},           {"R22",         dataset::r22}, 
	{"R23", dataset::r23}
};

const string dataset_path = "../../dataset/";

struct dataset_config {
	string filename;
	int vertex_number;
	int edge_number;
	int input_feature_length;
	int output_feature_length;

	dataset_config(string name, int vn, int en, int in, int out)
		:filename(name), vertex_number(vn), edge_number(en),
		input_feature_length(in), output_feature_length(out) {}
};

const map<dataset, dataset_config, less_dataset> dataset2config = {
	//        name                          filename         #vertex   #edge       len(input) len(output)
	{dataset::simple,        dataset_config("Simple",        10,       28,         128,       128)},
	{dataset::pubmed,        dataset_config("Pubmed",        19717,    88648,      500,       128)},
	// above for test
	// below for produce
	{dataset::reddit,        dataset_config("Reddit",        232965,   114615892,  602,       128)},
	{dataset::orkut,         dataset_config("Orkut",         3072627,  117185083,  500,       128)},	
	{dataset::livejournal_d, dataset_config("lj-d",          4847571,  68475391,   500,       128)},
	{dataset::stokes,        dataset_config("Stokes",        11449533, 349321980,  512,       128)},
	{dataset::twitter,       dataset_config("Twitter",       41652230, 1468364884, 1323,      128)},
	{dataset::friendster,    dataset_config("Friendster",    124836180,1806067135, 512,       128)},
	{dataset::r19,           dataset_config("RMAT-19N-24E",  1<<19,    1<<24,      512,       128)},
	{dataset::r20,           dataset_config("RMAT-20N-25E",  1<<20,    1<<25,      512,       128)},
	{dataset::r21,           dataset_config("RMAT-21N-26E",  1<<21,    1<<26,      512,       128)},
	{dataset::r22,           dataset_config("RMAT-22N-27E",  1<<22,    1<<27,      512,       128)},
	{dataset::r23,           dataset_config("RMAT-23N-28E",  1<<23,    1<<28,      512,       128)}
	/*
		Patents:	V-in = 2089345,  V-out = 3258983,  V = 6009555,   E = 16518947,   loop edge = 1.
		lj-d:		V-in = 4294332,  V-out = 4471652,  V = 4847571,   E = 68475391,   loop edge = 518382.
		orkut:		V-in = 2723360,  V-out = 3072440,  V = 3072627,   E = 117185083.
		twitter:	V-in = 40103278, V-out = 35689135, V = 41652230,  E = 1468364884, loop edge = 298.
		friendster:	V-in = 37551359, V-out = 64961029, V = 124836180, E = 1806067135.
	*/
	/* 
	test dataset:
	{dataset::mcf,           dataset_config("MCF-7",         732998,   1584284,    512,       128)},
	{dataset::aids,          dataset_config("AIDS",          31385,    64780,      512,       128)}, 
	{dataset::collab_all,    dataset_config("COLLAB-all",    372474,   24574995,   492,       128)},
	{dataset::simple,        dataset_config("Simple",        10,       28,         128,       128)},
	{dataset::citeseer,      dataset_config("Citeseer",      3327,     9104,       3703,      128)},
	{dataset::collab,        dataset_config("COLLAB",        12087,    1446010,    492,       128)},
	{dataset::cora,          dataset_config("Cora",          2708,     10556,      1433,      128)},
	{dataset::imdb,          dataset_config("IMDB",          2647,     28624,      136,       128)},
	{dataset::pubmed,        dataset_config("Pubmed",        19717,    88648,      500,       128)},
	original data:
	{dataset::reddit,        dataset_config("Reddit",        232965,   114615892,  602,       128)},
	{dataset::orkut,         dataset_config("Orkut",         3072441,  117185083,  512,       128)},
	{dataset::patents,       dataset_config("Patents",       3774768,  16518948,   512,       128)},
	{dataset::livejournal_d, dataset_config("lj-d",          4847571,  68993773,   512,       128)},
	{dataset::asia_osm,      dataset_config("Asia-osm",      11950757, 25423206,   512,       128)},
	{dataset::stokes,        dataset_config("Stokes",        11449533, 349321980,  512,       128)},
	{dataset::twitter,       dataset_config("Twitter",       41652230, 1468364884, 512,       128)},
	{dataset::friendster,    dataset_config("Friendster",    65608366, 1806067135, 512,       128)},
	*/
};

#endif // !MULTI_GCN_DATASET_H

/*
================================
main: run dataset Reddit, reading data
load: #vertex = 232965, #edge = 114615892
sample: filt 107757905 edge(s) in all
================================
main: run dataset Patents, reading data
load: #vertex = 6009555, #edge = 16518947
sample: filt 513692 edge(s) in all
================================
main: run dataset Orkut, reading data
load: #vertex = 3072627, #edge = 117185083
sample: filt 52446357 edge(s) in all
================================
main: run dataset Asia-osm, reading data
load: #vertex = 11950757, #edge = 25423204
sample: filt 0 edge(s) in all
================================
main: run dataset lj-d, reading data
load: #vertex = 4847571, #edge = 68475391
sample: filt 24633891 edge(s) in all
*/
