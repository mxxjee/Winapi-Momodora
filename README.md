# Winapi-Momodora
![제목 없는 다이어그램 drawio](https://github.com/user-attachments/assets/d490b923-59ef-4cc0-bc67-9811ee2b751d)

2025.06.19~2025.07.01



## 📝 소개
windows api를 사용하여 제작한 <모모도라 : 달 아래의 진혼곡> 모작입니다.

- 총 12스테이지 제작
- 에디터 제작
- 스프라이트 애니메이션 관리
- 슬롯, 게임 저장 / 불러오기 구현

<br><br>
## 🎞 영상
게임 플레이 영상은 아래에서 확인할 수 있습니다.

👉🏻 [영상보기](https://youtu.be/9n5yCr9qoXs)


<br><br>
## ⚙ 주요 구현 내용
프로젝트에서 주로 구현한 내용을 설명합니다.
### 에디터 구현과 클라이언트와 파싱

|Editor #1|Editor #2|
|:---:|:---:|
|<img src="https://github.com/user-attachments/assets/a081726c-d809-40b1-9b14-cd07a63e83cf" width="400"/>|<img src="https://github.com/user-attachments/assets/7b129208-8f19-4da0-abf4-d35dd93a41c4" width="400"/>|

- 마우스 드래그를 통해 맵 오브젝트의 위치와 크기를 설정하고, 파일 입출력을 통해 .dat 파일에 바이너리 형식으로 저장합니다.
- 오브젝트 정보는 중심 좌표와 크기를 Vec2 구조체로 직렬화하여 저장하며,
- 클라이언트에서는 동일한 구조체를 사용해 파일을 역직렬화하고, 게임에 오브젝트를 재구성합니다.


<br><br>

### 게임 내 플레이어 데이터의 저장, 불러오기 구현 및 UI와 연계

|Save/Load #1|Save/Load #2|
|:---:|:---:|
|<img src="https://github.com/user-attachments/assets/fea3349a-f868-41e5-8ca7-99f2d3394ebb" width="400"/>|<img src="https://github.com/user-attachments/assets/57bb6b31-afd4-4a73-ade2-3b799995c57b" width="400"/>|


- 게임의 저장 및 불러오기 기능을 담당하는 싱글턴 매니저 CSaveMgr 클래스를 구성했습니다.
- 슬롯 기반(Save1.dat~Save3.dat) 저장 파일을 통해 게임 상태, 플레이어 정보, 이벤트 진행도, 인벤토리를 파일 입출력을 이용해 관리합니다.
- 저장 대상 데이터는 현재시각, 현재 씬 번호, 플레이어 HP, 이벤트 상태, 인벤토리 정보 등 입니다.
- 이벤트 상태와 인벤토리는 별도 구조체 및 매니저를 통해 Save/Load를 처리하도록 구성했습니다.
  
