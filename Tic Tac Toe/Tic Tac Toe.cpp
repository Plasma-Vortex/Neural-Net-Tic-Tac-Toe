#include "Macros.h"
#include "FullyConnectedLayer.h"
#include "ActivationFunction.h"
#include "Neural Net.h"


/*
struct SigmoidActivationFunction {
	static Mat activation(const Mat& a);
	static Mat activationDeriv(const Mat& x);
};

struct TanhActivationFunction {
	static Mat activation(const Mat& a);
	static Mat activationDeriv(const Mat& x);
};

struct SoftMaxActivationFunction {
	static Mat activation(const Mat& a);
	static Mat activationDeriv(const Mat& x);
};

static double sigmoid(double x) {
	return 1.0 / (1.0 + exp(-x));
}

static double sigmoidDeriv(double x) {
	return sigmoid(x)*(1 - sigmoid(x));
}


static double tanhDeriv(double x) {
	return 1 - (tanh(x)*tanh(x));
}

static double mytanh(double x) { return tanh(x); }
static double myexp(double x) { return exp(x); }

Mat SigmoidActivationFunction::activation(const Mat& x) {
	return x.unaryExpr(&sigmoid);
}
Mat SigmoidActivationFunction::activationDeriv(const Mat& x) {
	return x.unaryExpr(&sigmoidDeriv);
}

Mat TanhActivationFunction::activation(const Mat& x) {
	return x.unaryExpr(&mytanh);
}
Mat TanhActivationFunction::activationDeriv(const Mat& x) {
	return x.unaryExpr(&tanhDeriv);
}

Mat SoftMaxActivationFunction::activation(const Mat& x) {
	Mat y = x.unaryExpr(&myexp);
	for (int c = 0; c < y.cols(); ++c)
		y.col(c) /= y.col(c).sum();
	return y;
}

Mat SoftMaxActivationFunction::activationDeriv(const Mat& x) {
	return Mat(1, 1); // not actually used
}
*/




pair<int, double> check(const Mat& tocheck, const Mat& correct)
{
	if (tocheck.rows() != correct.rows() || tocheck.cols() != correct.cols()) {
		printf("ERROR in check: Vectors are different sizes\n");
		return make_pair(0, 0);
	}
	int count = 0;
	double cost = 0.0;
	for (int col = 0; col < tocheck.cols(); col++)
	{
		bool works = true;
		double colCost = 0, max = 0;
		for (int i = 0; i < tocheck.rows(); ++i) {
//			double error = abs(tocheck(i, col) - correct(i, col));
//			if (error >= 0.5)
//				works = false;
//			cost += error * error;
			colCost += -correct(i, col)*log(tocheck(i, col));
			if (tocheck(i, col) > tocheck(max, col))
				max = i;
		}
		if (correct(max, col))
			++count;
		cost += colCost;
	}
	return make_pair(count, cost / tocheck.cols());
}

/*
int main()
{
	srand(time(NULL));

	vector<Layer*> layers;
	layers.push_back(new SigmoidLayer(10, 50));
	layers.push_back(new SoftMaxLayer(50, 10));
	Network2 n(layers, check, 10, 10, 16, 0.5);
    return 0;
}
*/

Vec binary(ll i, int bits)
{
	Vec v(bits);
	for (int j = 0; j < bits; ++j) {
		if (i & (1LL << j))
			v[j] = 1.0;
		else
			v[j] = 0.0;
	}
	return v;
}

Vec mod(long long i, int m) {
	Vec v = Vec::Zero(m);
	v[i % m] = 1.0;
	return v;
}

int main() {
	srand(time(NULL));

	int bits = 13, m = 14;

	typedef FullyConnectedLayer<SigmoidActivationFunction> SigmoidLayer;
	typedef FullyConnectedLayer<SoftMaxActivationFunction> SoftMaxLayer;

	vector<Layer*> layers;
	layers.push_back(new SigmoidLayer(bits, 20 * bits));
	layers.push_back(new SoftMaxLayer(20 * bits, m));
	Network2 n(layers, check, bits, m, 16, 1);
	trbatch training(100000), testing(1000);

	for (trdata& data : testing) {
		ll i = rand() % (1 << bits);
//		ll j = rand() % (1 << bits);
//		data.first = binary((i << bits) + j, 2 * bits);
//		data.second = binary(i + j, bits + 1);
		data.first = binary(i, bits);
		data.second = mod(i, m);
	}
	for (trdata& data : training) {
		ll i = rand() % (1 << bits);
//		ll j = rand() % (1 << bits);
//		data.first = binary((i << bits) + j, 2 * bits);
//		data.second = binary(i + j, bits + 1);
		data.first = binary(i, bits);
		data.second = mod(i, m);
	}

	n.train(training, testing, 500);
}
