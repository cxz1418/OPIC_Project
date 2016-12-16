코드가 너무길고 정리가 되지않아
 간단하게 요약설명을 붙였습니다.


저희코드의 구조는 Spring의 MVC패턴과 유사한 방향으로

따라갔습니다
. 여기서 어떤 Controller를 선택할지 결정하는

Dispatcher와 View를 합쳐서 관리했습니다.

주요한 클래스는 

(View + Dispatcher)

main.cpp  (WinAPI의 기본구조 루프가 돌아갑니다)
UI_Window.h   (각Usecase Stage별 Scene을 관리합니다)
UI_Component.h   (각Scene에 들어갈 구성요소들입니다)


(Model)

ptManager.h 
(MorphData를 만들어내는 클래스입니다.)
(뼈대를 이용해 구조를 만들때주로 쓰이고 변환과정에서는
Model Data로써만 사용하게 됩니다)

(Controller)[Service Classes]

MorphManager.h   (Converter : 변환을담당합니다)
CSmtp_custom.h   (EmailSender : 이메일 전송을 담당합니다)
soketToDlib.h    (Extracter1입니다. Dlib을 이용하여 특징을 추출합니다)
LuxandFaceSDK.h  (Extracter2입니다. LuxandSDK를 이용하여 특징을 추출합니다)


