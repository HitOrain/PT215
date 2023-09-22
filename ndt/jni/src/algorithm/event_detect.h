
#ifndef __EVENT_DETECT_H_
#define __EVENT_DETECT_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY_MODE		0
#define SLIDE_MODE		1
#define KEY_SLIDE_MODE	2

// ����ֵ action: ������ǰ֡�Ķ���
#define ACTION_NONE		0
#define ACTION_DOWN		1
#define ACTION_UP		2
#define ACTION_MOVE		3
#define ACTION_KEEP		4

// ����ֵ event: ����һϵ��action�����ɵĴ����¼�
#define  EVENT_NONE		0
#define  SLIDE_UP		1
#define  SLIDE_DOWN		2
#define  PRESS_SHORT	4
#define  PRESS_LONG		8


// ����
#ifndef TIMEOUT_SHORT
    #define  TIMEOUT_SHORT	100	 // �̰���ʱ����λ��10���룩
    #define  TIMEOUT_LONG	100	 // ������ʱ����λ��10���룩
    #define  MOVE_THRD		6	 // ����������ֵ N
    #define  MOVE_UNIT		1	 // ������λ���� N
    #define  MOVE_UNIT_LIMIT	6

    #define SLIDE_VALID_START	0
    #define SLIDE_VALID_END		MODEL_G1
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct EventDetectPara
{
    unsigned int uiShortPressThr;
    unsigned int uiLongPressThr;
    unsigned char ubMoveThrBig;
    unsigned char ubMoveThrSmall;

} ST_EVENT_DETECT_PARA;

extern ST_EVENT_DETECT_PARA st_event_det_para[GR_COUNT];	//�¼�������ֵ�Ȳ���������host����

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct EventDetectVar
{
    unsigned char boMoveFlag;
    unsigned char boLongPressFlag;
    unsigned char boTouchFlag;
    unsigned char boDisKeyFlag;
    unsigned char ubAction;
    unsigned char ubLastAction;
    unsigned char ubLastMoveDir;
    unsigned char ubEvent;
    unsigned char ubTouchPosition;
    unsigned char ubCurrentPosition;
    signed char   sbMoveDistance;
    unsigned int  uiTimeCount;

} ST_EVENT_DETECT_VAR;

extern ST_EVENT_DETECT_VAR st_event_det_var[GR_COUNT];	//�������¼������м����

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct EventDetectNotify
{
    unsigned char ubNotifyFlag;
    unsigned char ubCurrentPosition;
    unsigned char ubLastPosition;
    unsigned char ubMoveDistance;

} ST_EVENT_DETECT_NOTIFY;

extern ST_EVENT_DETECT_NOTIFY st_event_det_notify[GR_COUNT];	//�������¼����ϱ�����

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void slide_init(void);
extern void slide_detect(void);
extern void action_detect(s8 TouchFlag, s8 Position, ST_EVENT_DETECT_PARA *gstEventDetectPara, ST_EVENT_DETECT_VAR * gstEventDetectVar);
extern void event_analyze(ST_EVENT_DETECT_VAR * gstEventDetectVar, ST_EVENT_DETECT_NOTIFY * gstEventDetectNotify);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* End of __EVENT_DETECT_H_ */

