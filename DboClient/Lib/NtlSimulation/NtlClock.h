#ifndef _NTL_CLOCK_H_
#define _NTL_CLOCK_H_


class CNtlClock
{
// Member variables
protected:
	bool								m_bAvatarTimePause;
	RwUInt32							m_uiAvatarTime_Start;
	RwUInt32							m_uiAvatarTime_End;
	RwUInt32							m_uiAvatarTime_Accumulate;

// Constructions and Destructions
public:
	CNtlClock( void );
	~CNtlClock( void );

// Methods
public:
	// Avatar time
	bool								IsPause_AvatarTime( void ) const;
	void								SetPause_AvatarTime( bool bPause, RwUInt32 uiCurTime );
	RwUInt32							GetTime_AvatarTime( void ) const;
	void								Reset_AvatarTime( void );

	// Clock update
	void								Update( RwUInt32 uiCurTime );
};


inline bool CNtlClock::IsPause_AvatarTime( void ) const
{
	return m_bAvatarTimePause;
}

inline RwUInt32 CNtlClock::GetTime_AvatarTime( void ) const
{
	return m_uiAvatarTime_Accumulate + (m_uiAvatarTime_End - m_uiAvatarTime_Start);
}


#endif