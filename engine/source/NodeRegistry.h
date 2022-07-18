#ifndef NODE_REGISTRY_H
#define NODE_REGISTRY_H


#include <map>
#include <string>
#include "Node.h"

#define REGISTER_NODE(name,nodeType) if (nodeMapping.count(name) == 0) nodeMapping[name] = &createInstance<nodeType>;
#define REGISTER_CUSTOM_NODE(name,nodeType) NodeRegistry::registerNode<nodeType>(name)

class NodeRegistry {
	private:
		inline static std::map<std::string, Node*(*)()> nodeMapping;
		template<typename T> static Node* createInstance() { return new T; }
	public:
		static void initialize();
		template<typename T> static void registerNode(std::string name);
		static Node* instanceNode(std::string class_name) {
			return nodeMapping[class_name]();
		}
		static void clear() {
			nodeMapping.clear();
		}
};

#endif // !NODE_REGISTRY_H




