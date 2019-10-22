#include <array>
#include <string>

constexpr size_t Nparams = 60; // number of unknowns for the full model

constexpr size_t Nlig = 6; // Number of ligands

// Measured in the literature
constexpr double kfbnd = 0.60; // Assuming on rate of 10^7 M-1 sec-1


template <class T>
struct bindingRates {
	T kfwd, k1rev, k2rev, k4rev, k5rev, k10rev;
	T k11rev, k13rev, k14rev, k16rev, k17rev;
	T k22rev, k23rev, k25rev, k27rev, k29rev;
	T k31rev, k32rev, k33rev, k34rev, k35rev;
};


template <class T>
class ratesS {
public:
	std::array<T, Nlig> ILs; // IL2, 15, 7, 9, 4, 21
	bindingRates<T> surface, endosome;
	T endo, activeEndo;
	T sortF, kRec, kDeg;
	std::array<T, 8> Rexpr; // Expression: IL2Ra, IL2Rb, gc, IL15Ra, IL7Ra, IL9R, IL4Ra, IL21Ra

	void setTraffic(T * traf) {
		// Set the rates
		endo = traf[0];
		activeEndo = traf[1];
		sortF = traf[2];
		kRec = traf[3];
		kDeg = traf[4];

		if (sortF > 1.0) {
			throw std::runtime_error(std::string("sortF is a fraction and cannot be greater than 1.0."));
		}
	}

	explicit ratesS(std::vector<T> rxntfR) {
		std::copy_n(rxntfR.begin(), ILs.size(), ILs.begin());
		surface.kfwd = rxntfR[6];
		endosome.kfwd = rxntfR[6];
		surface.k1rev = rxntfR[7];
		surface.k2rev = rxntfR[8];
		surface.k4rev = rxntfR[9];
		surface.k5rev = rxntfR[10];
		surface.k10rev = rxntfR[11];
		surface.k11rev = rxntfR[12];
		surface.k13rev = rxntfR[13];
		surface.k14rev = rxntfR[14];
		surface.k16rev = rxntfR[15];
		surface.k17rev = rxntfR[16];
		surface.k22rev = rxntfR[17];
		surface.k23rev = rxntfR[18];
		surface.k25rev = rxntfR[19];
		surface.k27rev = rxntfR[20];
		surface.k29rev = rxntfR[21];
		surface.k31rev = rxntfR[22];
		surface.k32rev = rxntfR[23];
		surface.k33rev = rxntfR[24];
		surface.k34rev = rxntfR[25];
		surface.k35rev = rxntfR[26];
		endosome.k1rev = rxntfR[27];
		endosome.k2rev = rxntfR[28];
		endosome.k4rev = rxntfR[29];
		endosome.k5rev = rxntfR[30];
		endosome.k10rev = rxntfR[31];
		endosome.k11rev = rxntfR[32];
		endosome.k13rev = rxntfR[33];
		endosome.k14rev = rxntfR[34];
		endosome.k16rev = rxntfR[35];
		endosome.k17rev = rxntfR[36];
		endosome.k22rev = rxntfR[37];
		endosome.k23rev = rxntfR[38];
		endosome.k25rev = rxntfR[39];
		endosome.k27rev = rxntfR[40];
		endosome.k29rev = rxntfR[41];
		endosome.k31rev = rxntfR[42];
		endosome.k32rev = rxntfR[43];
		endosome.k33rev = rxntfR[44];
		endosome.k34rev = rxntfR[45];
		endosome.k35rev = rxntfR[46];

		setTraffic(rxntfR.data() + 47);

		std::copy_n(rxntfR.data() + 52, 8, Rexpr.begin());
	}

	void print() {
		std::cout << "kfwd: " << surface.kfwd << std::endl;
		std::cout << "k4rev: " << surface.k4rev << std::endl;
		std::cout << "k5rev: " << surface.k5rev << std::endl;
		std::cout << "k10rev: " << surface.k10rev << std::endl;
		std::cout << "k11rev: " << surface.k11rev << std::endl;
		std::cout << "k16rev: " << surface.k16rev << std::endl;
		std::cout << "k17rev: " << surface.k17rev << std::endl;
		std::cout << "k22rev: " << surface.k22rev << std::endl;
		std::cout << "k23rev: " << surface.k23rev << std::endl;
		std::cout << "k27rev: " << surface.k27rev << std::endl;
		std::cout << "k31rev: " << surface.k31rev << std::endl;
		std::cout << "k33rev: " << surface.k33rev << std::endl;
		std::cout << "k35rev: " << surface.k35rev << std::endl;
		std::cout << "endo: " << endo << std::endl;
		std::cout << "activeEndo: " << activeEndo << std::endl;
		std::cout << "sortF: " << sortF << std::endl;
		std::cout << "kRec: " << kRec << std::endl;
		std::cout << "kDeg: " << kDeg << std::endl;
	}
};


constexpr double internalV = 623.0; // Same as that used in TAM model
constexpr double internalFrac = 0.5; // Same as that used in TAM model

constexpr size_t Nspecies = 62; // number of complexes in surface + endosome + free ligand
constexpr size_t halfL = 28; // number of complexes on surface alone

extern "C" int runCkine (const double *tps, size_t ntps, double *out, const double * const rxnRatesIn, const double preT, const double * const preL);
extern "C" int runCkineS (const double * const tps, const size_t ntps, double * const out, double * const Sout, const double * const actV, const double * const rxnRatesIn, const double preT, const double * const preL);
