�ڵ尡 �ʹ���� ������ �����ʾ�
 �����ϰ� ��༳���� �ٿ����ϴ�.


�����ڵ��� ������ Spring�� MVC���ϰ� ������ ��������

���󰬽��ϴ�
. ���⼭ � Controller�� �������� �����ϴ�

Dispatcher�� View�� ���ļ� �����߽��ϴ�.

�ֿ��� Ŭ������ 

(View + Dispatcher)

main.cpp  (WinAPI�� �⺻���� ������ ���ư��ϴ�)
UI_Window.h   (��Usecase Stage�� Scene�� �����մϴ�)
UI_Component.h   (��Scene�� �� ������ҵ��Դϴ�)


(Model)

ptManager.h 
(MorphData�� ������ Ŭ�����Դϴ�.)
(���븦 �̿��� ������ ���鶧�ַ� ���̰� ��ȯ����������
Model Data�νḸ ����ϰ� �˴ϴ�)

(Controller)[Service Classes]

MorphManager.h   (Converter : ��ȯ������մϴ�)
CSmtp_custom.h   (EmailSender : �̸��� ������ ����մϴ�)
soketToDlib.h    (Extracter1�Դϴ�. Dlib�� �̿��Ͽ� Ư¡�� �����մϴ�)
LuxandFaceSDK.h  (Extracter2�Դϴ�. LuxandSDK�� �̿��Ͽ� Ư¡�� �����մϴ�)


