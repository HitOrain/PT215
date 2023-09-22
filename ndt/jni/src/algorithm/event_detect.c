
#include "algorithm_init.h"

ST_EVENT_DETECT_PARA st_event_det_para[GR_COUNT];//�¼�������ֵ�Ȳ���������host����
ST_EVENT_DETECT_VAR st_event_det_var[GR_COUNT];//�������¼������м����
ST_EVENT_DETECT_NOTIFY st_event_det_notify[GR_COUNT];//�������¼����ϱ�����

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ر����Ͳ����ĳ�ʼ��
void EventDetectInitSub(ST_EVENT_DETECT_PARA *gstEventDetectPara, ST_EVENT_DETECT_VAR * gstEventDetectVar, ST_EVENT_DETECT_NOTIFY * gstEventDetectNotify)
{
    gstEventDetectPara->ubMoveThrBig = MOVE_THRD;
    gstEventDetectPara->ubMoveThrSmall = MOVE_UNIT;
    gstEventDetectPara->uiLongPressThr = TIMEOUT_LONG;
    gstEventDetectPara->uiShortPressThr = TIMEOUT_SHORT;
    memset(gstEventDetectVar, 0, sizeof(ST_EVENT_DETECT_VAR));
    memset(gstEventDetectNotify, 0, sizeof(ST_EVENT_DETECT_NOTIFY));
}

void slide_init(void)
{
    u8 i;

    for (i = 0; i < GR_COUNT; i++)
    {
        EventDetectInitSub(&st_event_det_para[i], &st_event_det_var[i], &st_event_det_notify[i]);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//����������
void action_detect(s8 TouchFlag, s8 Position, ST_EVENT_DETECT_PARA *gstEventDetectPara, ST_EVENT_DETECT_VAR * gstEventDetectVar)
{
    //����������Ч��Χ�ڲ�ȥ�������ж�
    if(TouchFlag
            && Position <= SLIDE_VALID_END
            && Position >= SLIDE_VALID_START
      )
    {
        gstEventDetectVar->boTouchFlag = TouchFlag;
        gstEventDetectVar->ubCurrentPosition = Position;
    }
    else
    {
        gstEventDetectVar->boTouchFlag = 0;
    }


    if((gstEventDetectVar->ubLastAction == ACTION_NONE || gstEventDetectVar->ubLastAction == ACTION_UP)
            && gstEventDetectVar->boTouchFlag
      )
    {
        gstEventDetectVar->ubAction = ACTION_DOWN;
        gstEventDetectVar->ubTouchPosition = gstEventDetectVar->ubCurrentPosition;
        gstEventDetectVar->uiTimeCount = 0;
    }
    else if (gstEventDetectVar->ubLastAction != ACTION_NONE
             && gstEventDetectVar->ubLastAction != ACTION_UP
             && !gstEventDetectVar->boTouchFlag
            )
    {
        gstEventDetectVar->ubAction = ACTION_UP;
    }
    else if (gstEventDetectVar->boTouchFlag)
    {
        gstEventDetectVar->sbMoveDistance = gstEventDetectVar->ubCurrentPosition - gstEventDetectVar->ubTouchPosition;

        gstEventDetectVar->uiTimeCount++;

        if (!gstEventDetectVar->boMoveFlag)
        {
            // �ж��Ƿ񳬹�����������ֵ
            if (ABS(gstEventDetectVar->sbMoveDistance) >= gstEventDetectPara->ubMoveThrBig)
            {
                gstEventDetectVar->ubAction = ACTION_MOVE;
                gstEventDetectVar->boMoveFlag = 1;

                // �������
                gstEventDetectVar->ubTouchPosition = gstEventDetectVar->ubCurrentPosition;

                gstEventDetectVar->sbMoveDistance = gstEventDetectVar->sbMoveDistance / ABS(gstEventDetectVar->sbMoveDistance);
            }
            else
            {
                gstEventDetectVar->ubAction = ACTION_KEEP;
            }
        }
        else
        {
            // �жϻ����Ƿ񳬹�����1��������λ
            if (ABS(gstEventDetectVar->sbMoveDistance) >= gstEventDetectPara->ubMoveThrSmall)
            {
                //�������ϴεĻ��������෴
                if ((gstEventDetectVar->sbMoveDistance < 0 && gstEventDetectVar->ubLastMoveDir == SLIDE_DOWN)
                        || (gstEventDetectVar->sbMoveDistance > 0 && gstEventDetectVar->ubLastMoveDir == SLIDE_UP)
                   )
                {
                    gstEventDetectVar->ubAction = ACTION_KEEP;
                    gstEventDetectVar->ubLastMoveDir = ACTION_NONE;
                }
                else
                {
                    gstEventDetectVar->ubAction = ACTION_MOVE;
                }
            }
            else
            {
                gstEventDetectVar->ubAction = ACTION_KEEP;
            }

            // todo ������һ�λ������򣬽��з�������
        }

        //�޷�
        if (gstEventDetectVar->sbMoveDistance > MOVE_UNIT_LIMIT)
        {
            gstEventDetectVar->sbMoveDistance = MOVE_UNIT_LIMIT;
        }

        if (gstEventDetectVar->sbMoveDistance < -MOVE_UNIT_LIMIT)
        {
            gstEventDetectVar->sbMoveDistance = -MOVE_UNIT_LIMIT;
        }
    }
    else
    {
        gstEventDetectVar->ubAction = ACTION_NONE;
    }

    gstEventDetectVar->ubLastAction = gstEventDetectVar->ubAction;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�¼�����
void event_analyze(ST_EVENT_DETECT_VAR * gstEventDetectVar, ST_EVENT_DETECT_NOTIFY * gstEventDetectNotify)
{
    //�̰��¼�
    if (gstEventDetectVar->ubAction == ACTION_UP
            && !gstEventDetectVar->boMoveFlag
            && gstEventDetectVar->uiTimeCount < TIMEOUT_SHORT
       )
    {
        //gstEventDetectVar->ubEvent = PRESS_SHORT;
    }
    //�����¼�
    else if(gstEventDetectVar->ubAction == ACTION_KEEP
            && !gstEventDetectVar->boLongPressFlag
            && !gstEventDetectVar->boMoveFlag
            && gstEventDetectVar->uiTimeCount >= TIMEOUT_LONG
           )
    {
        //gstEventDetectVar->ubEvent = PRESS_LONG;
        gstEventDetectVar->boLongPressFlag = 1;
    }
    //�����¼�
    else if(gstEventDetectVar->ubAction == ACTION_MOVE
            && !gstEventDetectVar->boLongPressFlag
           )
    {
        gstEventDetectVar->ubEvent = gstEventDetectVar->sbMoveDistance < 0 ? SLIDE_UP : SLIDE_DOWN;
        gstEventDetectVar->ubLastMoveDir = gstEventDetectVar->ubEvent; //��¼�����ƶ�����
    }
    //���¼�
    else
    {
        gstEventDetectVar->ubEvent = EVENT_NONE;
    }

    gstEventDetectNotify->ubNotifyFlag = gstEventDetectVar->ubEvent;
    gstEventDetectNotify->ubCurrentPosition = gstEventDetectVar->ubCurrentPosition;
    gstEventDetectNotify->ubLastPosition = gstEventDetectVar->ubTouchPosition;
    gstEventDetectNotify->ubMoveDistance = ABS(gstEventDetectVar->sbMoveDistance);
    ///////////////////update event to notify process///////////////////

    //�����¼���������Ҫ���±���λ����Ϣ
    if (gstEventDetectVar->ubEvent == SLIDE_DOWN || gstEventDetectVar->ubEvent == SLIDE_UP)
    {
        gstEventDetectVar->ubTouchPosition = gstEventDetectVar->ubCurrentPosition;
        gstEventDetectVar->boDisKeyFlag = TRUE;//���������¼�  ��ֹ�����ϱ�
    }

    //�����ɽ�ʱ�����������
    if (gstEventDetectVar->ubAction == ACTION_UP)
    {
        memset(gstEventDetectVar, 0, sizeof(ST_EVENT_DETECT_VAR));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

