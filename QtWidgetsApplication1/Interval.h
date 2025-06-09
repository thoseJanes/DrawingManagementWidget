#pragma once
#include <vector>
#include <cassert>
class Interval {
public:
	int dim;
	std::vector<double> lower = {};
	std::vector<double> upper = {};
	bool init = false;
	Interval(int dim = 1) {
		this->dim = dim;
		this->init = false;
		for (int i = 0; i < dim; i++) {
			lower.push_back(0);
			upper.push_back(0);
		}
	};
	Interval(std::vector<double> lower, std::vector<double> upper, int dim = 1) {
		this->dim = dim;
		this->lower = lower;
		this->upper = upper;
		this->init = true;
	};
	Interval(double lower, double upper) {
		this->dim = 1;
		this->lower.push_back(lower);
		this->upper.push_back(upper);
		this->init = true;
	};
	~Interval() {
	};
	void clear() {
		this->lower = {};
		this->upper = {};
		this->init = false;
		for (int i = 0; i < dim; i++) {
			lower.push_back(0);
			upper.push_back(0);
		}
	}
	void intersect() {

	}
	void merge(Interval* interval) {
		assert(interval->dim == dim);
		if (this->init) {
			if (interval->init) {
				for (int i = 0; i < dim; i++) {
					if (lower[i] > interval->lower[i]) {
						lower[i] = interval->lower[i];
					}
					if (upper[i] < interval->upper[i]) {
						upper[i] = interval->upper[i];
					}
				}
			}
			else {
				return;
			}
		}
		else {
			this->lower = interval->lower;
			this->upper = interval->upper;
			this->init = interval->init;

		}
	}
	void merge(std::vector<double> point) {
		assert(point.size() == dim);
		if (this->init) {
			for (int i = 0; i < dim; i++) {
				if (lower[i] > point[i]) {
					lower[i] = point[i];
				}
				else if (upper[i] < point[i]) {
					upper[i] = point[i];
				}
			}
		}
		else {
			for (int i = 0; i < dim; i++) {
				lower[i] = point[i];
				upper[i] = point[i];
			}
			this->init = true;
		}
	}
	void merge(double* point) {
		//assert(point.size() == dim);
		if (this->init) {
			for (int i = 0; i < dim; i++) {
				if (lower[i] >= point[i]) {
					lower[i] = point[i];
				}
				else if (upper[i] <= point[i]) {
					upper[i] = point[i];
				}
			}
		}
		else {
			for (int i = 0; i < dim; i++) {
				lower[i] = point[i];
				upper[i] = point[i];
			}
			this->init = true;
		}
	}
};