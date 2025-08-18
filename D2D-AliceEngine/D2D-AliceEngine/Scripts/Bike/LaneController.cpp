#include "LaneController.h"
#include "Manager/UpdateTaskManager.h"
#include "System/ScriptSystem.h"

void LaneController::SetLaneWidth(float _width)
{
	laneWidth = _width;
}

float LaneController::GetLaneWidth()
{
	return laneWidth;
}

int LaneController::GetLaneIndex()
{
	return currLane;
}

void LaneController::MoveUp()
{
	if (currLane < maxLane) {
		currLane++;
		initZpos = st->zPos;
		moveElipsedTime = 0.0f;
		currStat = MOVEUP;
	}
	else if (currStat == IDLE)
		currStat = MAX;
}

void LaneController::MoveDown()
{
	if (currLane > minLane) {
		currLane--;
		initZpos = st->zPos;
		moveElipsedTime = 0.0f;
		currStat = MOVEDOWN;
	}
	else if (currStat == IDLE)
		currStat = MIN;
}

void LaneController::RemoveAction(ObjectHandle _handle, ActionType _type)
{
	while (true) {
		auto it = std::find_if(actions.begin(), actions.end(),
			[_handle, _type](const Slot& action)
			{ return (action.type == _type && action.handle == _handle); });

		if (it == actions.end())
			break;
		else
			actions.erase(it);
	}
}

void LaneController::Initialize()
{
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void LaneController::OnStart()
{
	if (owner)
		st = owner->GetComponent<SkewTransform>();

	if (!st) {
		std::wstring errorMessage;
		errorMessage += owner->GetUUID();
		errorMessage += L"의 LaneController에서 SkewTransform 컴포넌트를 불러오는데 실패함!!";

		OutputDebugStringW(errorMessage.c_str());
		// SkewTransform 없을시에 삭제
		owner->RemoveComponent(this);
	}
}

void LaneController::Update(const float& deltaSeconds)
{
	if (prevStat != currStat) {
		prevStat = currStat;
		switch (currStat)
		{
		case LaneController::MIN:
			BroadCast(MIN);
			currStat = IDLE;
			break;
		case LaneController::MAX:
			BroadCast(MAX);
			currStat = IDLE;
			break;
		case LaneController::MOVEUP:
			BroadCast(MOVEUP);
			break;
		case LaneController::MOVEDOWN:
			BroadCast(MOVEDOWN); 
			break;
		case LaneController::IDLE:
			BroadCast(IDLE);
			break;
		case LaneController::END:
			currStat = IDLE;
			break;
		default:
			break;
		}
	}

	Move(deltaSeconds);
}

void LaneController::ChangeRenderLayer()
{

}

void LaneController::BroadCast(ActionType _type)
{
	for (auto it = actions.begin(); it != actions.end();) {
		if (it->type == _type) {
			if (ObjectHandler::GetInstance().IsValid(it->handle)) {
				it->func();
				it++;
			}
			else
				it = actions.erase(it);
		}
		else
			it++;
	}
}

void LaneController::Move(const float& deltaTime)
{
	if (currStat == MOVEUP || currStat == MOVEDOWN) {
		moveElipsedTime += deltaTime;
		float zDest = currLane * laneWidth;
		st->zPos = Math::EaseOut(initZpos, zDest, moveElipsedTime, changeSpeed);
		if (Math::Approximately(st->zPos, zDest)) {
			currStat = IDLE;
		}
	}

	/*if (currStat == MOVEUP || currStat == MOVEDOWN) {
		st->zPos = currLane * laneWidth;
		std::wstring message = L"current lane : " + std::to_wstring(st->zPos) + L"\n";
		OutputDebugStringW(message.c_str());
		currStat = IDLE;
	}*/
}
