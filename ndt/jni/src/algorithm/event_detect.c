
#include "algorithm_init.h"

ST_EVENT_DETECT_PARA st_event_det_para[GR_COUNT];//事件检测的阈值等参数，可由host设置
ST_EVENT_DETECT_VAR st_event_det_var[GR_COUNT];//滑动等事件检测的中间变量
ST_EVENT_DETECT_NOTIFY st_event_det_notify[GR_COUNT];//滑动等事件的上报变量

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//相关变量和参数的初始化
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

//动作检测过程
void action_detect(s8 TouchFlag, s8 Position, ST_EVENT_DETECT_PARA *gstEventDetectPara, ST_EVENT_DETECT_VAR * gstEventDetectVar)
{
    //坐标落在有效范围内才去做滑动判断
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
            // 判断是否超过滑动击发阈值
            if (ABS(gstEventDetectVar->sbMoveDistance) >= gstEventDetectPara->ubMoveThrBig)
            {
                gstEventDetectVar->ubAction = ACTION_MOVE;
                gstEventDetectVar->boMoveFlag = 1;

                // 更新起点
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
            // 判断滑动是否超过至少1个滑动单位
            if (ABS(gstEventDetectVar->sbMoveDistance) >= gstEventDetectPara->ubMoveThrSmall)
            {
                //本次与上次的滑动方向相反
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

            // todo 根据上一次滑动方向，进行防抖处理
        }

        //限幅
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

//事件分析
void event_analyze(ST_EVENT_DETECT_VAR * gstEventDetectVar, ST_EVENT_DETECT_NOTIFY * gstEventDetectNotify)
{
    //短按事件
    if (gstEventDetectVar->ubAction == ACTION_UP
            && !gstEventDetectVar->boMoveFlag
            && gstEventDetectVar->uiTimeCount < TIMEOUT_SHORT
       )
    {
        //gstEventDetectVar->ubEvent = PRESS_SHORT;
    }
    //长按事件
    else if(gstEventDetectVar->ubAction == ACTION_KEEP
            && !gstEventDetectVar->boLongPressFlag
            && !gstEventDetectVar->boMoveFlag
            && gstEventDetectVar->uiTimeCount >= TIMEOUT_LONG
           )
    {
        //gstEventDetectVar->ubEvent = PRESS_LONG;
        gstEventDetectVar->boLongPressFlag = 1;
    }
    //滑动事件
    else if(gstEventDetectVar->ubAction == ACTION_MOVE
            && !gstEventDetectVar->boLongPressFlag
           )
    {
        gstEventDetectVar->ubEvent = gstEventDetectVar->sbMoveDistance < 0 ? SLIDE_UP : SLIDE_DOWN;
        gstEventDetectVar->ubLastMoveDir = gstEventDetectVar->ubEvent; //记录本次移动方向
    }
    //无事件
    else
    {
        gstEventDetectVar->ubEvent = EVENT_NONE;
    }

    gstEventDetectNotify->ubNotifyFlag = gstEventDetectVar->ubEvent;
    gstEventDetectNotify->ubCurrentPosition = gstEventDetectVar->ubCurrentPosition;
    gstEventDetectNotify->ubLastPosition = gstEventDetectVar->ubTouchPosition;
    gstEventDetectNotify->ubMoveDistance = ABS(gstEventDetectVar->sbMoveDistance);
    ///////////////////update event to notify process///////////////////

    //滑动事件发生，需要更新备份位置信息
    if (gstEventDetectVar->ubEvent == SLIDE_DOWN || gstEventDetectVar->ubEvent == SLIDE_UP)
    {
        gstEventDetectVar->ubTouchPosition = gstEventDetectVar->ubCurrentPosition;
        gstEventDetectVar->boDisKeyFlag = TRUE;//发生滑动事件  禁止按键上报
    }

    //发生松建时，将变量清空
    if (gstEventDetectVar->ubAction == ACTION_UP)
    {
        memset(gstEventDetectVar, 0, sizeof(ST_EVENT_DETECT_VAR));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

