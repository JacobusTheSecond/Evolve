#include <cmath>

class Node
{
public:
	Node();
	Node(int psize, Node** p);
	Node(int psize, Node** p, double* pweight);
	~Node();

	double getParentWeightAt(int i);
	double getValue();
	void setParentWeightAt(int i, double v);
	Node* getParentAt(int i);
	bool setValue(double v);
	int getParentSize();

private:

	Node** parent;
	int parentsize;
	double* parentweight;
	double value;
	bool superparent = true;

};


class NeuralNet
{
public:
	NeuralNet(int iLength, int oLength);
	NeuralNet(int iLength, int hLength, int oLength);
	~NeuralNet();

	double* simulate(double input[]);
	Node* getNodeAt(int layer, unsigned int number);

private:
	Node** inputLayer;
	Node** hiddenLayer;
	Node** outputLayer;
	int inputLength;
	int hiddenLength;
	int outputLength;
	double* output;
};

NeuralNet::NeuralNet(int iLength, int oLength)
{
	NeuralNet(iLength, iLength, oLength);
}

NeuralNet::NeuralNet(int iLength, int hLength, int oLength)
{
	inputLength = iLength;
	hiddenLength = hLength;
	outputLength = oLength;
	inputLayer = new Node*[iLength];
	for (int i = 0; i < iLength; ++i)
	{
		Node* inodeptr = new Node();
		inputLayer[i] = inodeptr;
	}

	hiddenLayer = new Node*[hiddenLength];
	for (int i = 0; i < hiddenLength; ++i)
	{
		Node* hnodeptr = new Node(inputLength, inputLayer);
		hiddenLayer[i] = hnodeptr;
	}

	outputLayer = new Node*[oLength];
	for (int i = 0; i < oLength; ++i)
	{
		Node* onodeptr = new Node(hiddenLength, hiddenLayer);
		outputLayer[i] = onodeptr;
	}
	output = new double[outputLength];

}

double* NeuralNet::simulate(double input[])
{
	for (int i = 0; i<inputLength; ++i) {
		(*(inputLayer[i])).setValue(input[i]);
	}

	for (int i = 0; i < outputLength; ++i) {
		output[i] = (*(outputLayer[i])).getValue();
	}
	return output;
}

NeuralNet::~NeuralNet()
{
	for (int i = 0; i < inputLength; ++i) {
		delete inputLayer[i];
	}
	delete inputLayer;
	for (int i = 0; i < hiddenLength; ++i) {
		delete hiddenLayer[i];
	}
	delete hiddenLayer;
	for (int i = 0; i < outputLength; ++i) {
		delete outputLayer[i];
	}
	delete outputLayer;
	delete output;
}

Node* NeuralNet::getNodeAt(int layer, unsigned int number)
{
	if (layer == 0) {
		if (number >= inputLength) {
			return nullptr;
		}
		return inputLayer[number];
	}
	else if (layer == 1) {
		if (number >= hiddenLength) {
			return nullptr;
		}
		return hiddenLayer[number];
	}
	else if (layer == 2) {
		if (number >= outputLength) {
			return nullptr;
		}
		return outputLayer[number];
	}
	else {
		return nullptr;
	}
}

Node::Node()
{
}

Node::Node(int psize, Node ** p)
{
	parentsize = psize;
	parent = p;
	superparent = false;
	parentweight = new double[parentsize];
	for (int i = 0; i < parentsize; ++i) {
		parentweight[i] = 0;
	}
}

Node::Node(int psize, Node ** p, double * pweight)
{
	parentsize = psize;
	parent = p;
	superparent = false;
	parentweight = pweight;
}

Node::~Node()
{
	delete parentweight;
}

double Node::getParentWeightAt(int i)
{
	if (i >= parentsize) {
		return 0;
	}
	else {
		return parentweight[i];
	}
}

double Node::getValue()
{
	if (!superparent) {
		value = 0;
		for (int i = 0; i < parentsize; ++i) {
			value += (*(parent[i])).getValue() * parentweight[i];
		}
		value = 1 / (1 + exp(-value));
	}

	return value;
}

void Node::setParentWeightAt(int i, double v)
{
	if (i < parentsize)
	{
		parentweight[i] = v;
	}
}

Node * Node::getParentAt(int i)
{
	if (i >= parentsize) {
		return nullptr;
	}
	else {
		return parent[i];
	}
}

bool Node::setValue(double v)
{
	if (superparent) {
		value = v;
		return true;
	}
	else {
		return false;
	}
}

int Node::getParentSize()
{
	return parentsize;
}
