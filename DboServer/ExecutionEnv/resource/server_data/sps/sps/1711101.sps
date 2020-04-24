------------------------------------------------------
--
-- 찌찌
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TLQ 2 쓰러진 찌찌" )

--쓰러진 모션 Idle
	
--대기
	Action( "wait" )
	--[
		Param("event type", "ts")
		Param( "event id", 16520)
	--]
	End()
	
--일어서는 동작
	Action( "direct play" )
	--[
		Param( "tblidx", 50000)
	--]     
	End()

--2초 Delay
	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

--일어서 있어라
	Action( "direct play" )
	--[
		Param( "tblidx", 50031)
	--]     
	End()
	
--쫑
	Action("sps end")
	--[
	--]
	End()
	
End()


------------------------------------------------------