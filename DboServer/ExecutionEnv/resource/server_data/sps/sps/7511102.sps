------------------------------------------------------
--
-- 트랭크스
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TMQ1" )

-- 트랭크스 게이트로 이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 11040)
	--]
	End()

--트랭크스 게이트로 이동
    Action( "point move" )
	--[
		Param( "loc x", 796.00)
		Param( "loc z", -801.57)
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 794.57)
		Param( "loc z", -792.68)
		Param( "run mode", 1 )
	--]
	End()
	
--트랭크스 이동완료 알림
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 11050 )
	--]
	End()
	
-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 11100)
	--]
	End()
	
--트랭크스 퇴장을 위한 이동
	Action( "point move" )
    --[
		Param( "loc x", 788.92)
		Param( "loc z", -793.53)
		Param( "run mode", 1 )
    --]
	End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End() 
	
End()


------------------------------------------------------