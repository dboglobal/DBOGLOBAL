------------------------------------------------------
--
-- 베지터
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TMQ 4 stage1" )

--퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14120)
	--]
	End()

--퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 813.287)
		Param( "loc z", -875.261)
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