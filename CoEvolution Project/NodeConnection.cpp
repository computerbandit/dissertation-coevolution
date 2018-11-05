#include "NodeConnection.h"

NodeConnection::NodeConnection(Node *next, Node * prev, float weight): _next(next) , _prev(prev), _weight(weight)
{

	_next->SetIn(this);
	_prev->SetOut(this);

}
