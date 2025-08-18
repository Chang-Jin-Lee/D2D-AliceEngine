#pragma once
#include <Core/Singleton.h>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <Define/Define.h>
#include <Object/UObject.h>
#include <Math/TMath.h>
#include <Experimental/Physics/AABB.h>
#include <Manager/SceneManager.h>
#include <Component/Component.h>
#include <Component/RenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/VideoComponent.h>
#include <Component/TransformComponent.h>
#include <Component/Animator.h>
#include <Core/ObjectHandler.h>
#include <Scene/Scene.h>

//------------------- 매크로 정의 -------------------
#define REGISTER_MEMBER(map, ClassType, Member) \
    map.insert({ #Member, MemberInfo(#Member, typeid(decltype(ClassType::Member)).name(), offsetof(ClassType, Member)) })

#define REGISTER_FUNCTION(map, FuncName, ReturnType, ...) \
    map.insert({ #FuncName, FunctionInfo(#FuncName, #ReturnType, { __VA_ARGS__ }, false) })

//------------------- 멤버 정보 구조체 -------------------
struct MemberInfo
{
	std::string name;
	std::string typeName;
	size_t offset;
	MemberInfo() : name(""), typeName(""), offset(0) {} // 기본 생성자 추가
	MemberInfo(const std::string& n, const std::string& t, size_t o)
		: name(n), typeName(t), offset(o) {}
};

//------------------- 함수 정보 구조체 -------------------
// 일단 함수는 나중에.
//struct FunctionInfo 
//{
//	std::string name;
//	std::string returnType;
//	std::vector<std::string> paramTypes;
//	size_t paramCount;
//	bool isVirtual;
//	FunctionInfo(const std::string& n, const std::string& r, const std::vector<std::string>& p, bool v)
//		: name(n), returnType(r), paramTypes(p), paramCount(p.size()), isVirtual(v) {}
//};

//------------------- 메타 정보 구조체 -------------------
struct ClassMetaInfo {
	std::string className;
	std::string memberInfoKey; // allMemberInfos의 key
	std::vector<std::string> parentNames;
	std::vector<std::string> childNames;
	std::function<class UObject* ()> creator;
};

class ClassManager : public Singleton<ClassManager>
{
	std::unordered_map<std::string, std::unordered_map<std::string, MemberInfo>> allMemberInfos;
	//std::unordered_map<std::string, FunctionInfo> myClassFunctionInfo;

	// 모든 클래스의 메타 정보
	std::unordered_map<std::string, ClassMetaInfo> metaInfos;
	std::unordered_map<std::type_index, ClassMetaInfo*> typeMetaMap;

public:
	// 런타임 객체의 실제 타입에 맞는 새 객체 생성
	void ReplicateAllMembers(UObject* dst, const UObject* src)
	{
		std::string dstClassName = typeid(*dst).name();
		std::string srcClassName = typeid(*src).name();
		ReplicateRecursive(dstClassName, dst, src, srcClassName);
	}

	// 런타임 객체의 실제 타입에 맞는 새 객체 생성
	UObject* CreateClass(const UObject* obj)
	{
		std::type_index ti(typeid(*obj));
		auto it = typeMetaMap.find(ti);
		if (it != typeMetaMap.end() && it->second->creator)
			return it->second->creator();
		return nullptr;
	}

	void RegisterClassInfo() 
	{
		allMemberInfos.clear();

		// 변수 등록 (매크로 활용)
		// ==============================UObject==============================
		std::string className = typeid(UObject).name();
		REGISTER_MEMBER(allMemberInfos[className], UObject, m_name);
		REGISTER_MEMBER(allMemberInfos[className], UObject, m_uuid);
		// ==============================RenderComponent==============================
		className = typeid(RenderComponent).name();
		REGISTER_MEMBER(allMemberInfos[className], RenderComponent, bFlip);
		className = typeid(SpriteRenderer).name();
		REGISTER_MEMBER(allMemberInfos[className], SpriteRenderer, filePath);
		REGISTER_MEMBER(allMemberInfos[className], SpriteRenderer, m_bitmap);
		className = typeid(BoxComponent).name();
		REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_pBrush);
		REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_color);
		REGISTER_MEMBER(allMemberInfos[className], BoxComponent, thickness);
		REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_size);
		className = typeid(TextRenderComponent).name();
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_layout);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_dWriteTextFormat);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_pBrush);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_eTransformType);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_content);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_color);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_font);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_fontSize);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_metrics);
		REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_metricsDirty);
		className = typeid(VideoComponent).name();
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, files);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, fileDirPath);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, m_bitmaps);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, cacheSize);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, m_curClip);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, m_maxClipSize);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, bPlay);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, m_fFPSTime);
		REGISTER_MEMBER(allMemberInfos[className], VideoComponent, m_fFPSLastTime);
		className = typeid(TransformComponent).name();
		REGISTER_MEMBER(allMemberInfos[className], TransformComponent, parent);
		REGISTER_MEMBER(allMemberInfos[className], TransformComponent, children);
		REGISTER_MEMBER(allMemberInfos[className], TransformComponent, m_pivot);
		REGISTER_MEMBER(allMemberInfos[className], TransformComponent, m_localTransform);
		REGISTER_MEMBER(allMemberInfos[className], TransformComponent, m_worldTransform);
		className = typeid(Animator).name();
		REGISTER_MEMBER(allMemberInfos[className], Animator, time);
		REGISTER_MEMBER(allMemberInfos[className], Animator, curAnimationClip);
		REGISTER_MEMBER(allMemberInfos[className], Animator, nextAnimationClip);
		REGISTER_MEMBER(allMemberInfos[className], Animator, sheet);
		REGISTER_MEMBER(allMemberInfos[className], Animator, animationClips);
		REGISTER_MEMBER(allMemberInfos[className], Animator, m_bitmap);
		REGISTER_MEMBER(allMemberInfos[className], Animator, m_curTime);
		REGISTER_MEMBER(allMemberInfos[className], Animator, m_curFrame);
		REGISTER_MEMBER(allMemberInfos[className], Animator, bPlay);
		REGISTER_MEMBER(allMemberInfos[className], Animator, bLoopping);
		REGISTER_MEMBER(allMemberInfos[className], Animator, m_accumTime);

		// 2. 메타 등록 (부모부터!)
		RegisterMetaInfoTypeWithoutCreator<UObject>();
		RegisterMetaInfoType<Component>({ "UObject" });
		RegisterMetaInfoTypeWithoutCreator<RenderComponent>({ "Component" });
		RegisterMetaInfoType<SpriteRenderer>({ "RenderComponent" });
		RegisterMetaInfoType<BoxComponent>({ "RenderComponent" });
		RegisterMetaInfoType<TextRenderComponent>({ "RenderComponent" });
		RegisterMetaInfoType<VideoComponent>({ "RenderComponent" });
		//RegisterMetaInfoType<Animator>({ "RenderComponent" });
		RegisterMetaInfoType<TransformComponent>({ "Component" });

		// 함수 등록 (매크로 활용, 매개변수 타입은 문자열로)
		//REGISTER_FUNCTION(myClassFunctionInfo, Initialize, void);
		//REGISTER_FUNCTION(myClassFunctionInfo, Update, void);
		//REGISTER_FUNCTION(myClassFunctionInfo, Release, int);
		//REGISTER_FUNCTION(myClassFunctionInfo, SetK, void, typeid(const int&).name());

		// 변수 등록
		//myClassMemberInfo.insert({ "HP", { "HP", typeid(int).name(), offsetof(myClass, HP) } });
		//myClassMemberInfo.insert({ "k",  { "k",  typeid(float).name(), offsetof(myClass, k) } });
		//myClassMemberInfo.insert({ "p",  { "p",  typeid(double).name(), offsetof(myClass, p) } });
		//myClassMemberInfo.insert({ "box",{ "box", typeid(FAABB).name(), offsetof(myClass, box) } });

		//// 함수 등록
		//myClassFunctionInfo.insert({ "Update",
		//	{ "Update", "void", {}, 0, false } });
		//myClassFunctionInfo.insert({ "Render",
		//	{ "Render", "void", {}, 0, true } });
		//myClassFunctionInfo.insert({ "returnFunction",
		//	{ "returnFunction", "int", {}, 0, false } });
		//myClassFunctionInfo.insert({ "SetK",
		//	{ "SetK", "void", { string(typeid(const int&).name()) }, 1, false } });
	}

	template<typename T>
	void RegisterMetaInfoType(const std::vector<std::string>& parentNames = {})
	{
		std::string className = typeid(T).name();
		metaInfos[className] = { className, className, parentNames, {}, []() -> UObject* { return new T(); } };
		typeMetaMap[std::type_index(typeid(T))] = &metaInfos[className];
		for (const auto& parent : parentNames)
			metaInfos[parent].childNames.push_back(className);
	}

	template<typename T>
	void RegisterMetaInfoTypeWithoutCreator(const std::vector<std::string>& parentNames = {})
	{
		std::string className = typeid(T).name();
		metaInfos[className] = { className, className, parentNames, {}, nullptr };
		typeMetaMap[std::type_index(typeid(T))] = &metaInfos[className];
		for (const auto& parent : parentNames)
			metaInfos[parent].childNames.push_back(className);
	}

	ClassMetaInfo* GetMetaInfo(const std::string& className) {
		auto it = metaInfos.find(className);
		if (it != metaInfos.end()) return &(it->second);
		return nullptr;
	}

	void ReplicateRecursive(const std::string& dstClassName, void* dst, const void* src, const std::string& srcClassName)
	{
		ClassMetaInfo* dstMeta = GetMetaInfo(dstClassName);
		ClassMetaInfo* srcMeta = GetMetaInfo(srcClassName);
		if (!dstMeta || !srcMeta) return;

		// 부모 먼저 복제 (각 계층의 부모를 맞춰서 복제)
		for (size_t i = 0; i < dstMeta->parentNames.size(); ++i) 
		{
			if (i < srcMeta->parentNames.size())
				ReplicateRecursive(dstMeta->parentNames[i], dst, src, srcMeta->parentNames[i]);
		}

		auto& dstMembers = allMemberInfos[dstMeta->memberInfoKey];
		auto& srcMembers = allMemberInfos[srcMeta->memberInfoKey];

		for (const auto& it : dstMembers)
		{
			const std::string& memberName = it.first;
			const MemberInfo& dstInfo = it.second;
			// 소스에 동일 멤버가 없으면 건너뜀
			auto srcIt = srcMembers.find(memberName);
			if (srcIt == srcMembers.end()) continue;
			const MemberInfo& srcInfo = srcIt->second;
			// 타입 불일치 시 건너뜀
			if (dstInfo.typeName != srcInfo.typeName) continue;

			size_t dstOffset = dstInfo.offset;
			size_t srcOffset = srcInfo.offset;
			const std::string& type = dstInfo.typeName;

			/* ===========================================================================
			*  C++에 있는 기본 타입들
			*/
			if (type == typeid(int).name()) {
				int* dstVal = reinterpret_cast<int*>(reinterpret_cast<char*>(dst) + dstOffset);
				const int* srcVal = reinterpret_cast<const int*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(float).name()) {
				float* dstVal = reinterpret_cast<float*>(reinterpret_cast<char*>(dst) + dstOffset);
				const float* srcVal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(double).name()) {
				double* dstVal = reinterpret_cast<double*>(reinterpret_cast<char*>(dst) + dstOffset);
				const double* srcVal = reinterpret_cast<const double*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(char).name()) {
				char* dstVal = reinterpret_cast<char*>(reinterpret_cast<char*>(dst) + dstOffset);
				const char* srcVal = reinterpret_cast<const char*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(bool).name()) {
				bool* dstVal = reinterpret_cast<bool*>(reinterpret_cast<char*>(dst) + dstOffset);
				const bool* srcVal = reinterpret_cast<const bool*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(short).name()) {
				short* dstVal = reinterpret_cast<short*>(reinterpret_cast<char*>(dst) + dstOffset);
				const short* srcVal = reinterpret_cast<const short*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(long).name()) {
				long* dstVal = reinterpret_cast<long*>(reinterpret_cast<char*>(dst) + dstOffset);
				const long* srcVal = reinterpret_cast<const long*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(long long).name()) {
				long long* dstVal = reinterpret_cast<long long*>(reinterpret_cast<char*>(dst) + dstOffset);
				const long long* srcVal = reinterpret_cast<const long long*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(unsigned char).name()) {
				unsigned char* dstVal = reinterpret_cast<unsigned char*>(reinterpret_cast<char*>(dst) + dstOffset);
				const unsigned char* srcVal = reinterpret_cast<const unsigned char*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(unsigned int).name()) {
				unsigned int* dstVal = reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(dst) + dstOffset);
				const unsigned int* srcVal = reinterpret_cast<const unsigned int*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(unsigned short).name()) {
				unsigned short* dstVal = reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(dst) + dstOffset);
				const unsigned short* srcVal = reinterpret_cast<const unsigned short*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(unsigned long).name()) {
				unsigned long* dstVal = reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(dst) + dstOffset);
				const unsigned long* srcVal = reinterpret_cast<const unsigned long*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(unsigned long long).name()) {
				unsigned long long* dstVal = reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>(dst) + dstOffset);
				const unsigned long long* srcVal = reinterpret_cast<const unsigned long long*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(wchar_t).name()) {
				wchar_t* dstVal = reinterpret_cast<wchar_t*>(reinterpret_cast<char*>(dst) + dstOffset);
				const wchar_t* srcVal = reinterpret_cast<const wchar_t*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(long double).name()) {
				long double* dstVal = reinterpret_cast<long double*>(reinterpret_cast<char*>(dst) + dstOffset);
				const long double* srcVal = reinterpret_cast<const long double*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(std::string).name()) {
				std::string* dstVal = reinterpret_cast<std::string*>(reinterpret_cast<char*>(dst) + dstOffset);
				const std::string* srcVal = reinterpret_cast<const std::string*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(std::wstring).name()) {
				std::wstring* dstVal = reinterpret_cast<std::wstring*>(reinterpret_cast<char*>(dst) + dstOffset);
				const std::wstring* srcVal = reinterpret_cast<const std::wstring*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(FAABB).name()) {
				FAABB* dstVal = reinterpret_cast<FAABB*>(reinterpret_cast<char*>(dst) + dstOffset);
				const FAABB* srcVal = reinterpret_cast<const FAABB*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Transform).name()) {
				Transform* dstVal = reinterpret_cast<Transform*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Transform* srcVal = reinterpret_cast<const Transform*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(FColor).name()) {
				FColor* dstVal = reinterpret_cast<FColor*>(reinterpret_cast<char*>(dst) + dstOffset);
				const FColor* srcVal = reinterpret_cast<const FColor*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(FVector2).name()) {
				FVector2* dstVal = reinterpret_cast<FVector2*>(reinterpret_cast<char*>(dst) + dstOffset);
				const FVector2* srcVal = reinterpret_cast<const FVector2*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			/* ===========================================================================
			*  Define에 있는 정의들
			*/
			else if (type == typeid(std::string).name()) {
				std::string* dstVal = reinterpret_cast<std::string*>(reinterpret_cast<char*>(dst) + dstOffset);
				const std::string* srcVal = reinterpret_cast<const std::string*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::EDrawType).name()) {
				Define::EDrawType* dstVal = reinterpret_cast<Define::EDrawType*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::EDrawType* srcVal = reinterpret_cast<const Define::EDrawType*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::ETextFormat).name()) {
				Define::ETextFormat* dstVal = reinterpret_cast<Define::ETextFormat*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::ETextFormat* srcVal = reinterpret_cast<const Define::ETextFormat*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::ETransformType).name()) {
				Define::ETransformType* dstVal = reinterpret_cast<Define::ETransformType*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::ETransformType* srcVal = reinterpret_cast<const Define::ETransformType*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::ETickingGroup).name()) {
				Define::ETickingGroup* dstVal = reinterpret_cast<Define::ETickingGroup*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::ETickingGroup* srcVal = reinterpret_cast<const Define::ETickingGroup*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::EScriptGroup).name()) {
				Define::EScriptGroup* dstVal = reinterpret_cast<Define::EScriptGroup*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::EScriptGroup* srcVal = reinterpret_cast<const Define::EScriptGroup*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::ParameterType).name()) {
				Define::ParameterType* dstVal = reinterpret_cast<Define::ParameterType*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::ParameterType* srcVal = reinterpret_cast<const Define::ParameterType*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(Define::EBoxType).name()) {
				Define::EBoxType* dstVal = reinterpret_cast<Define::EBoxType*>(reinterpret_cast<char*>(dst) + dstOffset);
				const Define::EBoxType* srcVal = reinterpret_cast<const Define::EBoxType*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			// 기타 타입 필요시 추가
			// SpriteRender에 쓰임
			else if (type == typeid(std::shared_ptr<ID2D1Bitmap1>).name()) {
				std::shared_ptr<ID2D1Bitmap1>* dstVal = reinterpret_cast<std::shared_ptr<ID2D1Bitmap1>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const std::shared_ptr<ID2D1Bitmap1>* srcVal = reinterpret_cast<const std::shared_ptr<ID2D1Bitmap1>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal; // 참조 카운트 증가
			}
			// BoxComponent에서 COmPtr 복제
			else if (type == typeid(Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>).name()) {
				auto* dstVal = reinterpret_cast<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const auto* srcVal = reinterpret_cast<const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal; // 참조 카운트 증가, 같은 브러시 객체 공유
			}
			// 아래 3개는 TextRenderComponent 복제
			else if (type == typeid(Microsoft::WRL::ComPtr<IDWriteTextLayout>).name()) {
				auto* dstVal = reinterpret_cast<Microsoft::WRL::ComPtr<IDWriteTextLayout>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const auto* srcVal = reinterpret_cast<const Microsoft::WRL::ComPtr<IDWriteTextLayout>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal; // 참조 카운트 증가, 같은 객체 공유
			}
			else if (type == typeid(Microsoft::WRL::ComPtr<IDWriteTextFormat>).name()) {
				auto* dstVal = reinterpret_cast<Microsoft::WRL::ComPtr<IDWriteTextFormat>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const auto* srcVal = reinterpret_cast<const Microsoft::WRL::ComPtr<IDWriteTextFormat>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal; // 참조 카운트 증가, 같은 객체 공유
			}
			else if (type == typeid(DWRITE_TEXT_METRICS).name()) {
				DWRITE_TEXT_METRICS* dstVal = reinterpret_cast<DWRITE_TEXT_METRICS*>(reinterpret_cast<char*>(dst) + dstOffset);
				const DWRITE_TEXT_METRICS* srcVal = reinterpret_cast<const DWRITE_TEXT_METRICS*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			// video Component
			else if (type == typeid(std::vector<std::shared_ptr<ID2D1Bitmap1>>).name()) {
				std::vector<std::shared_ptr<ID2D1Bitmap1>>* dstVal = reinterpret_cast<std::vector<std::shared_ptr<ID2D1Bitmap1>>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const std::vector<std::shared_ptr<ID2D1Bitmap1>>* srcVal = reinterpret_cast<const std::vector<std::shared_ptr<ID2D1Bitmap1>>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal; // 내부 포인터의 참조 카운트가 증가하며 안전하게 복제됨
			}
			else if (type == typeid(std::vector<std::wstring>).name()) {
				std::vector<std::wstring>* dstVal = reinterpret_cast<std::vector<std::wstring>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const std::vector<std::wstring>* srcVal = reinterpret_cast<const std::vector<std::wstring>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			else if (type == typeid(D2D1_SIZE_U).name()) {
				D2D1_SIZE_U* dstVal = reinterpret_cast<D2D1_SIZE_U*>(reinterpret_cast<char*>(dst) + dstOffset);
				const D2D1_SIZE_U* srcVal = reinterpret_cast<const D2D1_SIZE_U*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
			}
			// TransformComponent
			else if (type == typeid(WeakObjectPtr<TransformComponent>).name()) {
				WeakObjectPtr<TransformComponent>* dstVal = reinterpret_cast<WeakObjectPtr<TransformComponent>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const WeakObjectPtr<TransformComponent>* srcVal = reinterpret_cast<const WeakObjectPtr<TransformComponent>*>(reinterpret_cast<const char*>(src) + srcOffset);
				*dstVal = *srcVal;
				}
			else if (type == typeid(std::vector<WeakObjectPtr<TransformComponent>>).name()) {
					std::vector<WeakObjectPtr<TransformComponent>>* dstVal =
						reinterpret_cast<std::vector<WeakObjectPtr<TransformComponent>>*>(reinterpret_cast<char*>(dst) + dstOffset);
					const std::vector<WeakObjectPtr<TransformComponent>>* srcVal =
						reinterpret_cast<const std::vector<WeakObjectPtr<TransformComponent>>*>(reinterpret_cast<const char*>(src) + srcOffset);
					*dstVal = *srcVal;
					}
			// Animator
			else if (type == typeid(std::unique_ptr<SpriteSheet>).name()) {
				auto* dstVal = reinterpret_cast<std::unique_ptr<SpriteSheet>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const auto* srcVal = reinterpret_cast<const std::unique_ptr<SpriteSheet>*>(reinterpret_cast<const char*>(src) + srcOffset);
				if (srcVal->get()) {
					// SpriteSheet에 Clone() 같은 복제 메서드가 있다면
					(*dstVal) = std::make_unique<SpriteSheet>(*(srcVal->get()));
					// 또는 (*dstVal) = srcVal->get()->Clone();
				}
				else {
					dstVal->reset();
				}
			}
			else if (type == typeid(std::unordered_map<std::string, std::unique_ptr<AnimationClip>>).name()) {
				auto* dstVal = reinterpret_cast<std::unordered_map<std::string, std::unique_ptr<AnimationClip>>*>(reinterpret_cast<char*>(dst) + dstOffset);
				const auto* srcVal = reinterpret_cast<const std::unordered_map<std::string, std::unique_ptr<AnimationClip>>*>(reinterpret_cast<const char*>(src) + srcOffset);
				dstVal->clear();
				for (const auto& pair : *srcVal) {
					if (pair.second) {
						// AnimationClip이 복사 생성자나 Clone() 지원 시
						(*dstVal)[pair.first] = std::make_unique<AnimationClip>(*(pair.second));
						// 또는 (*dstVal)[pair.first] = pair.second->Clone();
					}
					else {
						(*dstVal)[pair.first].reset();
					}
				}
			}

		}
	}

	template<typename T>
	void PrintClassMeta(int depth = 0) 
	{
		std::string className = typeid(T).name();
		ClassMetaInfo* meta = GetMetaInfo(className);
		if (!meta) return;
		std::string indent(depth * 2, ' ');
		std::cout << indent << "=== " << meta->className << " Variables ===\n";
		for (const auto& it : *(meta->memberInfos)) 
		{
			const MemberInfo& info = it.second;
			std::cout << indent << "[Var] Type: " << info.typeName << ", Name: " << info.name << ", Offset: " << info.offset << std::endl;
		}
		for (const auto& parent : meta->parentNames) 
		{
			// 부모도 템플릿 기반으로 출력
			if (parent == typeid(UObject).name())
				PrintClassMeta<UObject>(depth + 1);
			// 다중 상속이 있다면 else if로 추가
		}
	}
};

