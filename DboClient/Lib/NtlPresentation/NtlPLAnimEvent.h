#ifndef _ANIM_EVENT_H_
#define _ANIM_EVENT_H_

enum EAnimEvent
{
	EVENT_ANIM_END,
	EVENT_ANIM_HIT,
	EVENT_ANIM_FOOT_STEP,
};

struct SAnimMsg
{
	EAnimEvent	id;
	void		*pEventData;
};

struct SEventAnimEnd
{
	unsigned int uiAnimKey;

	SEventAnimEnd()
	{
		uiAnimKey = 0;
	}
};

struct SEventAnimHit
{
	unsigned int uiHitType;
};


struct SAnimSequenceEvent
{
	EAnimEvent	id;					//event의 종류
	float		fTime;				//시작 시간 (실제 시간)
};

#endif