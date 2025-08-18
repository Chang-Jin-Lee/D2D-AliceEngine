![C++ Version](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![IDE](https://img.shields.io/badge/IDE-Visual%20Studio%202022-5C2D91?logo=visualstudio&logoColor=white)
![Engine](https://img.shields.io/badge/Engine-D2D-blue?style=flat)
![JSON Parser](https://img.shields.io/badge/JSON_Parser-Nolman-green?style=flat)

#### 협업으로 완성된 게임은 다음의 경로에서 볼 수 있습니다.

https://github.com/TanoshiiMoe/AliceEngine

# AliceEngine
D2D 기반으로 개발한 2D 게임 엔진입니다.  
C++로 구현했으며, 2D 캐릭터 애니메이션, 게임 메커니즘(게임오브젝트, 충돌처리, 리소스매니저, 컴포넌트, 씬)을 지원합니다.

---

## 프로젝트 구조
- `Engine/` : 엔진 핵심 모듈
- `Game/` : 게임 콘텐츠 빌드

---

## UML 다이어그램

엔진의 아키텍처와 클래스 구조를 한눈에 볼 수 있도록 UML 다이어그램을 제공합니다.

<p align="center">
  <img src="https://github.com/user-attachments/assets/da2f5c4f-b14e-4564-94b6-bb285df8a903" width="400"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/ff437a5f-cc4d-4de5-852c-52ffc3be20bb" width="500"/>
</p>

![image](https://github.com/user-attachments/assets/a5d5a4bc-5844-4a9d-9515-8609e8be56fa)
![image](https://github.com/user-attachments/assets/4eed35f8-791a-41d5-ba9f-167964154676)
<img width="8364" height="6484" alt="image" src="https://github.com/user-attachments/assets/322820e5-b1b0-452b-88bb-a9552b3e7141" />
![image](https://github.com/user-attachments/assets/3ba63e7f-b3b9-4d83-a0e0-b80878f3dca6)
<img width="12488" height="6844" alt="image" src="https://github.com/user-attachments/assets/68f469f4-3287-4290-8973-de75c8fec962" />
<img width="5628" height="3728" alt="image" src="https://github.com/user-attachments/assets/d4588b8d-9f20-4be8-85cf-90eb786c03a6" />
<img width="5604" height="3008" alt="image" src="https://github.com/user-attachments/assets/d4a5fe4d-f989-49a5-9a11-49f6f3d6fa33" />

---

## 주요 성과

- 자체 개발 2D 엔진 **AliceEngine** 제작  
- Spine2D 애니메이션, TileMap, 파티클, UI 기능을 포함한 게임 실행 환경 구현  
- 게임 콘텐츠 개발자가 엔진을 활용해 실제 플레이 가능한 게임 빌드 성공  

엔진 코어 개발을 전담했습니다.  
아키텍처 설계부터 Spine2D, TileMap, 파티클, UI 시스템까지 전반적인 엔진 핵심 기능을 구현했습니다.

### 엔진 코어 총괄
- 아키텍처 설계 및 모듈 경계 정의  
- 객체 생명주기 및 메모리 관리 (`WeakObjectPtr`, `ObjectHandler`, `NewObject` 규약)  
- 업데이트 파이프라인 (`ETickingGroup`: PrePhysics, Physics, PostPhysics, PreRender, Render)  
- 렌더링 파이프라인 (RenderQueue 정렬, Batch 최적화, 카메라 행렬)  
- Spine2D 런타임 (JSON/Atlas 로더, 본 변환, Mesh/Region Attachment 렌더링, 좌표계 보정)  

### 트랜스폼 시스템 (핵심 설계)
- 모든 gameObject는 `TransformComponent`(position, rotation[deg], scale, pivot/size)를 보유  
- 부모-자식 계층 구조 지원: relativeTransform → 부모 worldTransform에 연결  
- **Dirty 플래그 기반 갱신**: 로컬 변경 시 상위→하위로 전파, 불필요한 행렬 연산 최소화  
- 업데이트 순서: PrePhysics(트랜스폼·콜라이더 동기화) → Physics/Collision → PreRender(최종 확정)  
- 합성 규칙: `Scale → Rotation → Translation` 순, pivot 보정은 렌더 단계에서 처리  
- 최종 행렬:  
  - 월드 스페이스: `flipY * world * cameraInv * UnityScreen`  
  - UI(ScreenSpace): `flipY * world * UnityScreen`  
- SpineRenderer는 본 행렬 + 애태치먼트 행렬 합성 후 카메라/좌표계 적용  
- Collider/BoxComponent: Debug·Collision 용도 분리, AABB는 BoxComponent의 중심·사이즈로 계산  

### 주요 기능 구현
- Spine2D 애니메이션: 아티스트 제작 애니메이션을 런타임에서 재생 가능  
- SpriteBatch 기반 TileMap: 수천 개 타일을 1회 드로우콜로 처리, 2000개 이상 배치해도 60FPS 유지  
- 파티클 시스템: 수백 개 파티클을 Batch 렌더링, Additive Blend 및 파라미터 제어 지원  
- UI 시스템: WorldSpace / ScreenSpace 지원, HUD 및 인터랙션(버튼·Hover) 구현  

---

## 협업 경험

- 팀원 3명이 GitHub 레포지토리에서 브랜치/PR 기반 협업  
- 아티스트/게임 콘텐츠 개발자의 요구사항을 받아 Spine, TileMap, 파티클, UI 기능을 제공  
- 코드 리팩토링 과정을 공유하며 팀 전체가 엔진 구조를 이해할 수 있도록 문서화  

---

<!--  
## 시연 자료

- [Spine2D 애니메이션 캡처](링크 또는 GIF 첨부)  
- [TileMap 데모](링크 또는 GIF 첨부)  
- [파티클 이펙트 데모](링크 또는 GIF 첨부)  
- [UI 동작 캡처](링크 또는 GIF 첨부)  

---
-->

## 빌드 및 실행

1. Visual Studio 2022 이상 설치  
2. 레포지토리 클론  
   ```bash
   git clone https://github.com/Chang-Jin-Lee/D2D-AliceEngine.git
