#include "tokenize.hpp"
#include "interpreter.hpp"

Tokenize::Tokenize() : head(nullptr), tail(nullptr), currentNode(nullptr)
{

}

Tokenize::~Tokenize()
{
	delete head;
}

vector<string> Tokenize::tokenize(string expression)
{
	vector<string> token;
	string expressionHolder = "";

	for (unsigned int i = 0; i < expression.size(); ++i) // add spaces around the parentheses for easier tokenizing
	{
		if (expression[i] == '(')
			expressionHolder = expressionHolder + " ( ";
		else if (expression[i] == ')')
			expressionHolder = expressionHolder + " ) ";
		else
			expressionHolder = expressionHolder + expression[i];
	}

	/*cout << "Expression: " << expression << endl;
	cout << "Parsed Expression: " << expressionHolder << endl;*/

	expression = expressionHolder;

	string newVar = "";
	for (unsigned int i = 0; i < expression.size(); ++i) // store into my vector 
	{
		if (expression[i] == ' ' || expression[i] == 13) {

			if (!newVar.empty())
			{
				token.push_back(newVar);
			}
			newVar = "";
		}

		else {
			newVar += expression[i]; //so now newVar is the first letter we read
		}
	}

	//for (unsigned int i = 0; i < token.size(); ++i)
	//cout << "This is the token: " << token[i] << endl;

	//cout << endl;

	return token;
}

bool Tokenize::buildAST(vector<string> token)
{
	postEvalDestroy(head);
	head = nullptr;

	if (token.size() == 0 || token[0] != "(") {
		return false;
	}

	unsigned leftCount = 0;
	unsigned rightCount = 0;
	for (unsigned int i = 0; i < token.size(); i++)
	{
		if (token[i] == "(")
			leftCount++;
		else if (token[i] == ")")
			rightCount++;

		if (rightCount > leftCount)
			return false;
		if (leftCount == rightCount && i != token.size() - 1)
			return false;
	}

	if (leftCount != rightCount)
		return false;

	enum States { stateA, stateB, stateC, stateD, stateE };
	enum States currentState = stateA;

	unsigned int pos = 0;

	while (pos < token.size())
	{
		switch (currentState)
		{
		case stateA:
		{
			if (token[pos] == "(" && token[pos + 1] != ")") {
				
				tail = new Expression;

				tail->environment = environment;
				if (pos == 0) {
					tail->parent = nullptr; head = tail; tail = tail; currentNode = tail;
				}
				else {
					tail = tail;
					tail->parent = currentNode;
					currentNode->children.push_back(tail);
					currentNode = tail;
				}
				currentState = stateB;
				pos++;
			}

			else if (token[pos] == "(" && token[pos + 1] == ")")
			{
				destroyAST(head);
				return false;
			}

			else if (token[pos] == ")")
			{
				if (currentNode->parent != nullptr)
					currentNode = currentNode->parent;
				else if (currentNode->parent == nullptr && pos == token.size() - 1)
					return true;
				//else if (currentNode->parent == nullptr && pos < token.size() - 1)
				//return false;
				pos++;
				currentState = stateA;
			}

			else
			{
				tail = new Expression;
				tail->environment = environment;
				tail = tail;
				tail->parent = currentNode;
				currentNode->children.push_back(tail);
				currentState = stateB;
			}

		}
		break;

		case stateB: // check for NumberType
		{
			bool isNumber = true;
			unsigned eCount = 0;
			unsigned negCount = 0;
			unsigned decimalCount = 0;
			if (token[pos].size() == 1 && token[pos][0] == '-')
				isNumber = false;
			else {
				for (unsigned int i = 0; i < token[pos].size(); i++)
				{
					if (!isdigit(token[pos][i]))
					{
						if ((i == 0 && token[pos][i] == '-') || (i >= 2 && token[pos][i - 1] == 'e' && token[pos][i] == '-'))
							negCount++;
						else if (i > 0 && i != token[pos].size() - 1 && token[pos][i] == 'e' && token[pos][i - 1] != '-')
							eCount++;
						else if ((i > 0 && token[pos][i] == '.' && isdigit(token[pos][i - 1]) && eCount == 0 && ( isdigit(token[pos][i + 1])) ) ||
							(i == 0 && token[pos].size() >= 1 && token[pos][i] == '.' && isdigit(token[pos][i + 1])))
							decimalCount++;
						else
							isNumber = false;

						if (eCount > 1 || negCount > 2 || decimalCount > 1)
							isNumber = false;
					}
				}
			}

			if (isNumber == true)
			{
				tail->atom.atomType = NumberType;
				tail->atom.number = stod(token[pos]);
				tail->atom.var = token[pos]; /////////////////
				pos++;
				currentState = stateA;
			}

			else
				currentState = stateC;
		}
		break;

		case stateC: // check for SymbolType
		{
			if (token[pos] == "True" || token[pos] == "False")
				currentState = stateD;
			else if (token[pos] == "point" || token[pos] == "line" || token[pos] == "arc")
			{
				tail->atom.atomType = token[pos] == "point" ? PointType :
									  token[pos] == "line" ? LineType :
									  ArcType;
				
				tail->atom.var = token[pos];
				tail->atom.truthValue = false;
				pos++;
				currentState = stateA;
			}
			else if (token[pos][0] == '-' && isdigit(token[pos][1]))
				currentState = stateE;
			else if (!isdigit(token[pos][0])) {
				tail->atom.atomType = SymbolType;
				tail->atom.var = token[pos];
				pos++;
				currentState = stateA;
			}

			else
				currentState = stateE;
		}
		break;

		case stateD:
		{
			tail->atom.atomType = BoolType;
			if (token[pos] == "True")
				tail->atom.truthValue = true;
			else if (token[pos] == "False")
				tail->atom.truthValue = false;
			tail->atom.var = token[pos];////////////////
			pos++;
			currentState = stateA;
		}
		break;

		case stateE:
		{
			destroyAST(head);
			return false;
		}
		break;
		}

	}
}

//void Tokenize::traversePostOrder(Expression *currentNode)
//{
//	for (unsigned int i = 0; i < currentNode->children.size(); i++)
//	{
//		traversePostOrder(currentNode->children[i]);
//	}
//	cout << currentNode->atom.var << ": ";
//	if (currentNode->atom.atomType == 0)
//		cout << "noneType" << endl;
//	else if (currentNode->atom.atomType == 1)
//		cout << "boolType" << endl;
//	else if (currentNode->atom.atomType == 2)
//		cout << "numberType" << endl;
//	else if (currentNode->atom.atomType == 3)
//		cout << "symbolType" << endl;
//}


void Tokenize::destroyAST(Expression *temp)
{
	if (temp != nullptr)
	{
		while (!temp->children.empty())
		{
			delete temp->children[0];
			temp->children.erase(temp->children.begin());
		}

		//delete temp;
	}
}

void Tokenize::postEvalDestroy(Expression *temp)
{
	if (temp != nullptr)
	{
		while (!temp->children.empty())
		{
			if (temp->children[0] == nullptr) {
				temp->children.pop_back();
			}
			delete temp->children[0];
			temp->children.erase(temp->children.begin());
		}
		if (temp != nullptr)
			delete temp;
	}
}
