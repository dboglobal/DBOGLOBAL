------------------------------------------------------
--
-- 손오공
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene(1, "TMQ 4 stage1" )

--퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14110)
	--]
	End()

--퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 826.559)
		Param( "loc z", -875.116)
		Param( "run mode", 1 )
    --]
    End()
	
--퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50048)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  

End()

------------------------------------------------------