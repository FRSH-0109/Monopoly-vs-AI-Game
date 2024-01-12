/**
 * @file Tinyann.h
 *
 * @brief Header file for AI implementation of tiny library
 * from github user hav4ik, repository: https://github.com/hav4ik/tinyai
 *
 * @author hav4ik
 *
 */

#ifndef _ARTIFICIAL_NEURAL_NETWORK_HPP_
#define _ARTIFICIAL_NEURAL_NETWORK_HPP_

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Tinyneat.h"

namespace ann {

enum type { RECURRENT, NON_RECURRENT };

class neuron {
   public:
	int type = 0;  // 0 = ordinal, 1 = input, 2 = output, 3 = bias
	double value = 0.0;
	bool visited = false;
	std::vector<std::pair<size_t, double>> in_nodes;
	neuron() {}
	~neuron() { in_nodes.clear(); }
};

class neuralnet {
   private:
	std::vector<neuron> nodes;
	bool recurrent = false;

	std::vector<size_t> input_nodes;
	std::vector<size_t> bias_nodes;
	std::vector<size_t> output_nodes;

	double sigmoid(double x);
	void evaluate_nonrecurrent(const std::vector<double>& input, std::vector<double>& output);

	void evaluate_recurrent(const std::vector<double>& input, std::vector<double>& output);

   public:
	neuralnet();

	void from_genome(const neat::genome& a);

	void evaluate(const std::vector<double>& input, std::vector<double>& output);

	void import_fromfile(std::string filename);

	void export_tofile(std::string filename);
};

}  // end of namespace ann

#endif
