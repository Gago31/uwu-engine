#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <memory>
#include "Component.h"

struct Entity {
	std::string name;
	std::vector<ComponentPtr> components;
};

#endif