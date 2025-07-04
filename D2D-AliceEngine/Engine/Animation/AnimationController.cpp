#include "pch.h"
#include "AnimationController.h"
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <Helpers/FileHelper.h>
#include <Helpers/StringHelper.h>

Define::ParameterType AnimatorController::StringToParameterType(const std::string& type)
{
	if (type == "Int") return Define::ParameterType::Int;
	else if (type == "Float") return Define::ParameterType::Float;
	else if (type == "Bool") return Define::ParameterType::Bool;
	else if (type == "Trigger") return Define::ParameterType::Trigger;
	return Define::ParameterType::Int; // �⺻��
}

void AnimatorController::LoadAnimatorController(const std::wstring& filePath, AnimatorController& out)
{
	std::ifstream file(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH) + filePath);
	if (file.is_open()) // ifstream���� ���� �б�
	{
		json input;
		file >> input;
		file.close();
		out.controllerName = input["controllerName"];
		out.parameters = input["parameters"].get<std::vector<Parameter>>();

		for (const auto& param : out.parameters) 
{
			// key:�Ķ�����̸�, value: type  �߰� 
			out.paramTypes[param.name] = StringToParameterType(param.type);
		}

		out.defaultState = input["defaultState"];
		if (input.contains("states")) 
		{
			out.states = input["states"].get<std::vector<State>>();
		}

		for (int i = 0; i < out.states.size(); ++i) 
		{
			out.stateNameToIndex[out.states[i].name] = i;
			out.states[i].index = i; // ���� �ε��� ����

			for (auto& trans : out.states[i].transitions) {
				for (auto& condi : trans.conditions) {
					// �Ķ�����̸� -> type ��ȯ
					condi.type = out.paramTypes[condi.parameter];
				}
			}
		}

		if (input.contains("anyStateTransitions")) 
		{
			out.anyStateTransitions = input["anyStateTransitions"].get<std::vector<AnyStateTransition>>();

			for (auto& trans : out.anyStateTransitions) {
				for (auto& condi : trans.conditions) {
					// �Ķ�����̸� -> type ��ȯ
					condi.type = out.paramTypes[condi.parameter];
				}
			}
		}
	}
	else {
		throw std::runtime_error("Failed to open animation clip file: " + StringHelper::wstring_to_string(filePath));
	}
}

void from_json(const nlohmann::json& in, Parameter& out) {
	in.at("name").get_to(out.name);
	in.at("type").get_to(out.type);
	in.at("defaultFloat").get_to(out.defaultFloat);
	in.at("defaultInt").get_to(out.defaultInt);
	in.at("defaultBool").get_to(out.defaultBool);
}

void from_json(const nlohmann::json& in, Condition& out) {
	in.at("parameter").get_to(out.parameter);
	in.at("mode").get_to(out.mode);
	in.at("threshold").get_to(out.threshold);
}

void from_json(const nlohmann::json& in, Transition& out) {
	in.at("fromState").get_to(out.fromState);
	in.at("toState").get_to(out.toState);

	if (in.contains("conditions")) {
		out.conditions = in.at("conditions").get<std::vector<Condition>>();
	}
	in.at("exitTime").get_to(out.exitTime);
	in.at("hasExitTime").get_to(out.hasExitTime);
}

void from_json(const nlohmann::json& in, AnyStateTransition& out) {
	in.at("toState").get_to(out.toState);
	if (in.contains("conditions")) {
		out.conditions = in.at("conditions").get<std::vector<Condition>>();
	}
}

void from_json(const nlohmann::json& in, State& out) {
	in.at("name").get_to(out.name);
	in.at("motionName").get_to(out.motionName);
	in.at("clipLength").get_to(out.clipLength);
	in.at("loop").get_to(out.loop);
	if (in.contains("transitions")) {
		out.transitions = in.at("transitions").get<std::vector<Transition>>();
	}
}

void from_json(const nlohmann::json& in, AnimatorController& out) {
	in.at("controllerName").get_to(out.controllerName);
	out.parameters = in.at("parameters").get<std::vector<Parameter>>();

	// ù��° Layer�� ���¸ӽŸ� ó��
	in.at("defaultState").get_to(out.defaultState);
	if (in.contains("states")) {
		out.states = in.at("states").get<std::vector<State>>();
	}
	if (in.contains("anyStateTransitions")) {
		out.anyStateTransitions = in.at("anyStateTransitions").get<std::vector<AnyStateTransition>>();
	}
}