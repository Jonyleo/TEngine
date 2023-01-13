////////////////////////////////////////////////////////////////////////////////
//	CGJ Project - 2022-2023
//	
//	"Assignment 3 - 3D TANGRAM"
//
//	Group 1:
//		Joao Oliveira  | 93728
//		Giulio Camasso | 105147
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>
#include <filesystem>
#include <vector>
#include <algorithm>

#include <nlohmann/json.hpp>

#include <GL/glew.h>

#include <tengine/mglApp.hpp>
#include <tengine/mglConventions.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>
#include <tengine/Input.hpp>

#include "tangram.hpp"

namespace tangram {

	void TangramScript::init()
	{
		for (auto &child : entity.getChildren())
		{
			std::string childId = child->getName();
			if (childId.ends_with("Start"))	{
				startTransforms[childId.substr(0, childId.length() - 5)] = child->getComponent<tengine::Transform>();

			}
			else if(childId.ends_with("End")){
				endTransforms[childId.substr(0, childId.length() - 3)] = child->getComponent<tengine::Transform>();
			} else {
				pieceTransforms[childId] = child->getComponent<tengine::Transform>();
			}
		}

		
		tengine::InputManager::getInstance().registerKey(GLFW_KEY_LEFT,
			[this] (tengine::KeyInput input) {
				if(input.press)
					transition += 1;
				else
					transition -= 1;
			}
		);

		tengine::InputManager::getInstance().registerKey(GLFW_KEY_RIGHT,
			[this] (tengine::KeyInput input) {
				if(input.press)
					transition -= 1;
				else
					transition += 1;
			}
		);
		
		

		
	}

	

	void TangramScript::update(double elapsedTime)
	{
		if (transition == 0)
			return;

		timePassed += elapsedTime * transition;
		if (timePassed > transitionTime)
			timePassed = transitionTime;
		if (timePassed < 0)
			timePassed = 0;

		for (auto &startEntry : startTransforms)
		{
			auto &start = startEntry.second;
			auto &end = endTransforms[startEntry.first];

			std::shared_ptr<tengine::Transform> pieceTrans = pieceTransforms[startEntry.first];

			glm::quat deltaRotation = glm::mix(
				start->getRotation(),
				end->getRotation(),
				timePassed / transitionTime);

			glm::vec3 deltaPosition = glm::mix(
				start->getPosition(),
				end->getPosition(),
				timePassed / transitionTime);

			pieceTrans->rotateTo(deltaRotation);
			pieceTrans->moveTo(deltaPosition);
		}
	}
	

	TangramScript::TangramScript(tengine::Entity &entity) : Component(entity) {}
	TangramScript::TangramScript(tengine::Entity &entity, json &data) : Component(entity) {
		timePassed = data["timePassed"].get<float>();
	}

	void TangramScript::saveComponent(json &data) {
		data["timePassed"] = timePassed;
	}

}




