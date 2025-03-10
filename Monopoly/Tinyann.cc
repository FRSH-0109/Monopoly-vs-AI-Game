/**
 * @file Tinyann.h
 *
 * @brief Header file for AI implementation of tiny library
 * from github user hav4ik, repository: https://github.com/hav4ik/tinyai
 *
 * @author hav4ik
 *
 */

#include "Tinyann.h"

double ann::neuralnet::sigmoid(double x) {
	return 2.0 / (1.0 + std::exp(-4.9 * x)) - 1;
}

void ann::neuralnet::evaluate_nonrecurrent(const std::vector<double>& input, std::vector<double>& output) {
	for (size_t i = 0; i < nodes.size(); i++)
		nodes[i].value = 0.0, nodes[i].visited = false;

	for (size_t i = 0; i < input.size() && i < input_nodes.size(); i++) {
		nodes[input_nodes[i]].value = input[i];
		nodes[input_nodes[i]].visited = true;
	}

	for (size_t i = 0; i < bias_nodes.size(); i++) {
		nodes[bias_nodes[i]].value = 1.0;
		nodes[bias_nodes[i]].visited = true;
	}

	std::stack<size_t> s;
	for (size_t i = 0; i < output_nodes.size(); i++)
		s.push(output_nodes[i]);

	while (!s.empty()) {
		size_t t = s.top();

		if (nodes[t].visited == true) {
			double sum = 0.0;
			for (size_t i = 0; i < nodes[t].in_nodes.size(); i++)
				sum += nodes[nodes[t].in_nodes[i].first].value * nodes[t].in_nodes[i].second;
			nodes[t].value = sigmoid(sum);
			s.pop();
		}

		else {
			nodes[t].visited = true;

			for (size_t i = 0; i < nodes[t].in_nodes.size(); i++)
				if (nodes[nodes[t].in_nodes[i].first].visited == false)
					// if we haven't calculated value for this node
					s.push(nodes[t].in_nodes[i].first);
		}
	}

	for (size_t i = 0; i < output_nodes.size() && i < output.size(); i++)
		output[i] = nodes[output_nodes[i]].value;
}

void ann::neuralnet::evaluate_recurrent(const std::vector<double>& input, std::vector<double>& output) {
	for (size_t i = 0; i < input.size() && i < input_nodes.size(); i++) {
		nodes[input_nodes[i]].value = input[i];
		nodes[input_nodes[i]].visited = true;
	}

	for (size_t i = 0; i < bias_nodes.size(); i++) {
		nodes[bias_nodes[i]].value = 1.0;
		nodes[bias_nodes[i]].visited = true;
	}

	// in non-recurrent, each node we will visit only one time per
	// simulation step (similar to the real world)
	// and the values will be saved till the next simulation step
	for (size_t i = 0; i < nodes.size(); i++) {
		double sum = 0.0;
		for (size_t j = 0; j < nodes[i].in_nodes.size(); j++)
			sum += nodes[nodes[i].in_nodes[j].first].value + nodes[i].in_nodes[j].second;
		if (nodes[i].in_nodes.size() > 0)
			nodes[i].value = sigmoid(sum);
	}

	for (size_t i = 0; i < output_nodes.size() && i < output.size(); i++)
		output[i] = nodes[output_nodes[i]].value;
}

ann::neuralnet::neuralnet() {}

void ann::neuralnet::from_genome(const neat::genome& a) {
	unsigned int input_size = a.network_info.input_size;
	unsigned int output_size = a.network_info.output_size;
	unsigned int bias_size = a.network_info.bias_size;

	this->recurrent = a.network_info.recurrent;

	nodes.clear();
	input_nodes.clear();
	bias_nodes.clear();
	output_nodes.clear();

	neuron tmp;
	for (unsigned int i = 0; i < input_size; i++) {
		nodes.push_back(tmp);
		nodes.back().type = 1;
		this->input_nodes.push_back(nodes.size() - 1);
	}
	for (unsigned int i = 0; i < bias_size; i++) {
		nodes.push_back(tmp);
		nodes.back().type = 3;
		this->bias_nodes.push_back(nodes.size() - 1);
	}
	for (unsigned int i = 0; i < output_size; i++) {
		nodes.push_back(tmp);
		nodes.back().type = 2;
		this->output_nodes.push_back(nodes.size() - 1);
	}

	std::map<unsigned int, unsigned int> table;
	for (unsigned int i = 0; i < input_nodes.size() + output_nodes.size() + bias_nodes.size(); i++)
		table[i] = i;

	for (auto it = a.genes.begin(); it != a.genes.end(); it++) {
		if (!(*it).second.enabled)
			continue;

		neuron n;
		if (table.find((*it).second.from_node) == table.end()) {
			nodes.push_back(n);
			table[(*it).second.from_node] = nodes.size() - 1;
		}
		if (table.find((*it).second.to_node) == table.end()) {
			nodes.push_back(n);
			table[(*it).second.to_node] = nodes.size() - 1;
		}
	}

	for (auto it = a.genes.begin(); it != a.genes.end(); it++)
		nodes[table[(*it).second.to_node]].in_nodes.push_back(
			std::make_pair(table[(*it).second.from_node], (*it).second.weight));
}

void ann::neuralnet::evaluate(const std::vector<double>& input, std::vector<double>& output) {
	if (recurrent)
		this->evaluate_recurrent(input, output);
	else
		this->evaluate_nonrecurrent(input, output);
}

void ann::neuralnet::import_fromfile(std::string filename) {
	std::ifstream o;
	o.open(filename);

	this->nodes.clear();
	this->input_nodes.clear();
	this->output_nodes.clear();

	try {
		if (!o.is_open())
			throw "error: cannot open file!";

		std::string rec;
		o >> rec;
		if (rec == "recurrent")
			this->recurrent = true;
		if (rec == "non_recurrent")
			this->recurrent = false;

		unsigned int neuron_number;
		o >> neuron_number;
		this->nodes.resize(neuron_number);

		for (unsigned int i = 0; i < neuron_number; i++) {
			unsigned int input_size, type;	// 0 = ordinal, 1 = input, 2 = output
			nodes[i].value = 0.0;
			nodes[i].visited = false;

			o >> type;
			if (type == 1)
				input_nodes.push_back(i);
			if (type == 2)
				output_nodes.push_back(i);
			if (type == 3)
				bias_nodes.push_back(i);

			nodes[i].type = type;

			o >> input_size;
			for (unsigned int j = 0; j < input_size; j++) {
				unsigned int t;
				double w;
				o >> t >> w;
				nodes[i].in_nodes.push_back(std::make_pair(t, w));
			}
		}
	} catch (std::string error_message) {
		std::cerr << error_message << std::endl;
	}

	o.close();
}

void ann::neuralnet::export_tofile(std::string filename) {
	std::ofstream o;
	o.open(filename);

	if (this->recurrent)
		o << "recurrent" << std::endl;
	else
		o << "non-recurrent" << std::endl;
	o << nodes.size() << std::endl << std::endl;

	for (size_t i = 0; i < nodes.size(); i++) {
		o << nodes[i].type << " ";
		o << nodes[i].in_nodes.size() << std::endl;
		for (unsigned int j = 0; j < nodes[i].in_nodes.size(); j++)
			o << nodes[i].in_nodes[j].first << " " << nodes[i].in_nodes[j].second << " ";
		o << std::endl << std::endl;
	}
	o.close();
}