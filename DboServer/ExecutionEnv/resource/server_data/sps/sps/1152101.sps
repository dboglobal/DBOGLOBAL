------------------------------------------------------
--
-- 가면사나이
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TMQ 4 가면사나이" )

--퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14510)
	--]
	End()

--퇴장 연출
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


Scene( 2, "TMQ 4 가면사나이" )

--퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14570)
	--]
	End()

--퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 477.66)
		Param( "loc z", -465.97)
		Param( "run mode", 1 )
    --]
    End()
	
--퇴장 연출
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