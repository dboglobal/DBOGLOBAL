------------------------------------------------------
--
-- Chichi
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TLQ 3 Chichi" )

--불량배 경고하러 이동
--    Action( "point move" )
--    --[
--		Param( "loc x", -123.44)
--		Param( "loc z", -16.34)
--		Param( "run mode", 0 )
--    --]
--    End()
	
--이동완료 알림
--	Action( "send event to wps" )
--	--[
--		Param( "wps index", 11606 )
--		Param( "event id", 16713 )
--	--]
--	End()
	
--Wait for the command to move
	Action( "wait" )
	--[
		Condition("recv event from wps")
		--[
			Param( "wps index", 11606)
			Param( "event id", 16820)
		--]
		End()
	--]
	End()
	
--Moving towards the bully
    Action( "point move" )
    --[
		Param( "loc x", -122.55)
		Param( "loc z", 4.65)
		Param( "run mode", 0 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", -78.80)
		Param( "loc z", 5.75)
		Param( "run mode", 0 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", -82.13)
		Param( "loc z", 5.75)
		Param( "run mode", 1 )
    --]
    End()
	
--Send completion notification
	Action( "send event to wps" )
	--[
		Param( "wps index", 11606 )
		Param( "event id", 17000 )
	--]
	End()
	
--Wait for the command to move
	Action( "wait" )
	--[
		Condition("recv event from wps")
		--[
			Param( "wps index", 11606)
			Param( "event id", 16870)
		--]
		End()
	--]
	End()
	
--Move to goku and the others
    Action( "point move" )
    --[
		Param( "loc x", 100.28)
		Param( "loc z", 5.88)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", 100.91)
		Param( "loc z", -88.83)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", 106.18)
		Param( "loc z", -95.10)
		Param( "run mode", 1 )
    --]
    End()
	
--이동완료 알림
	Action( "send event to wps" )
	--[
		Param( "wps index", 11606 )
		Param( "event id", 17010 )
	--]
	End()
	
--이동 명령까지 대기
	Action( "wait" )
	--[
		Condition("recv event from ts")
		--[
			Param( "trigger type", "quest")
			Param( "event id", 16890)
		--]
		End()
	--]
	End()
	
--Move towards goku
    Action( "point move" )
    --[
		Param( "loc x", 109.72)
		Param( "loc z", -99.80)
		Param( "run mode", 1 )
    --]
    End()
	
--이동완료 알림
	Action( "send event to wps" )
	--[
		Param( "wps index", 11606 )
		Param( "event id", 17020 )
	--]
	End()
	
--경무관 밖 이동 명령까지 대기
	Action( "wait" )
	--[
		Condition("recv event from ts")
		--[
			Param( "trigger type", "quest")
			Param( "event id", 16910)
		--]
		End()
	--]
	End()
	
--Leave the goku and the others (move outside arena)
    Action( "point move" )
    --[
		Param( "loc x", 106.18)
		Param( "loc z", -95.10)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", 100.91)
		Param( "loc z", -88.83)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", 116.32)
		Param( "loc z", -82.05)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", 111.11)
		Param( "loc z", -82.05)
		Param( "run mode", 1 )
    --]
    End()
	
--chichi wait for command to disappear
	Action( "wait" )
	--[
		Condition("recv event from ts")
		--[
			Param( "trigger type", "quest")
			Param( "event id", 16930)
		--]
		End()
	--]
	End()
	
--chichi go to disappear
    Action( "point move" )
    --[
		Param( "loc x", 100.91)
		Param( "loc z", -88.83)
		Param( "run mode", 1 )
    --]
    End()

	Action( "leave" )
	--[
	--]
	End()

End()