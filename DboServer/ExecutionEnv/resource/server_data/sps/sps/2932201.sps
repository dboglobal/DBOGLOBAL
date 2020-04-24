------------------------------------------------------
--
-- 손오반
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TMQ 3 Gohan" )

--세뇌공룡 모두 죽을때 까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13195)
	--]
	End()
	
--풀숲까지 이동
    Action( "point move" )
    --[
        Param( "loc x", 895.33)
        Param( "loc z", -451.78)
        Param( "dir x", 0.26)
        Param( "dir z", -0.97)
        Param( "run mode", 0 )
    --]
    End()

--세뇌중인 공룡들 모두 죽을때 까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13200)
	--]
	End()
	
--역사상 장소까지 이동
    Action( "point move" )
    --[
        Param( "loc x", 928.46)
        Param( "loc z", -502.02)
        Param( "dir x", 0.20)
        Param( "dir z", 0.98)
        Param( "run mode", 0 )
    --]
    End()

	Action( "sps end" )
	--[
	--]
	End()
	
End()


------------------------------------------------------