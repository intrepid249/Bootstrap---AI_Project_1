#include <vector>
#include <iostream>
#include <ios>
#include <fstream>
#include <assert.h>
#include <string>
#include "inc\st_utility.h"

struct vec2 {
	int x, y;
};

struct Edge;
struct Node {
	vec2 m_data;
	std::vector<Edge> edges;
};

struct Edge {
	Node to;
	Node from;
};

#include <fstream>

int main() {
	std::vector<Node> nodeList;

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			Node n;
			n.m_data.x = x;
			n.m_data.y = y;

			nodeList.push_back(n);
		}
	}

	for (size_t i = 0; i < nodeList.size(); i++) {
		Edge e1;
		e1.to = nodeList[i];
		if (i % 2 == 0) {
			Edge e2;
			e2.to = nodeList[i];
			nodeList[i].edges.push_back(e2);
		}
		nodeList[i].edges.push_back(e1);
	}

	std::fstream file("test.dat", std::ios_base::out);
	if (!file.fail()) {
		for (auto node : nodeList) {

			file.write(std::to_string(node.m_data.x).c_str(), sizeof(const char *));
			file.write(", ", sizeof(const char *));

			if (node.edges.size() > 0) {
				for (size_t i = 0; i < node.edges.size(); i++) {
					auto edge = node.edges[i];


					if (i != node.edges.size() - 1);
						
				}

			}

		}
		file.close();
	} else {
		std::cout << "Error loading file\n";
	}

	system("pause");
	
}