#ifndef COMPONENT_H
#define COMPONENT_H

struct Component {
	virtual void f() {}
};

using ComponentPtr = std::shared_ptr<Component>;

#endif