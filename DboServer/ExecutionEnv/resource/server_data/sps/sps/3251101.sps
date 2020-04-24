------------------------------------------------------
--
-- 불량한 무도가
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------

Scene( 1, "TLQ 3 삥뜯기 대기중" )

--대기
	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()

--앉아있기
	Action( "sitdown" )
	--[
		Param( "sitdown", "true")
	--]
	End()
	
--일으키기 명령까지 대기
	Action( "wait" )
	--[
		Condition("recv event from ts")
		--[
			Param( "trigger type", "quest")
			Param( "event id", 16710)
		--]
		End()
	--]
	End()
	
--일어섬
	Action( "sitdown" )
	--[
		Param( "sitdown", "false")
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()

End()

Scene( 2, "TLQ 3 두목 데리고 복수전" )

    Action( "point move" )
    --[
		Param( "loc x", -74.77)
		Param( "loc z", -1.25)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", -79.93)
		Param( "loc z", 1.03)
		Param( "run mode", 1 )
    --]
    End()
	
	Action( "sps end" )
	--[
	--]
	End() 

End()