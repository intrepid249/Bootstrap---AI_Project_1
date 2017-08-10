#pragma once
#include <unordered_map>
#include <vector>
#include <memory>

class Behaviour;
namespace aie {
	class Renderer2D;
}

class BehaviourController {
public:
	BehaviourController();
	~BehaviourController();

	void update(float deltaTime);
	void render(aie::Renderer2D *renderer);

	/** Add a behaviour to the collection*/
	void addBehaviour(std::shared_ptr<Behaviour> behaviour);
	/** Remove a specified behaviour*/
	void removeBehaviour(std::shared_ptr<Behaviour> behaviour);
	/** Get a specific behaviour*/
	const std::shared_ptr<Behaviour> &getBehaviour(unsigned int ID);
	/** Get map of active behaviours*/
	const std::unordered_map<unsigned int, std::shared_ptr<Behaviour>> &getBehaviours();

private:
	std::unordered_map<unsigned int, std::shared_ptr<Behaviour>> m_behaviours;

	enum eCommand { ADD, REMOVE };
	struct Command {
		eCommand cmd;
		std::shared_ptr<Behaviour> behaviour;
	};
	std::vector<Command> m_commands;

	void processCommands();

	void processAddBehaviour(std::shared_ptr<Behaviour> behaviour);
	void processRemoveBehaviour(std::shared_ptr<Behaviour> behaviour);
};