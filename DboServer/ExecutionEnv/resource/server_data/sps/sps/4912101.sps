Scene( 1, "34007" )
--[
--위크

--대사 기다림
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34007 )
			Param( "event id", 882 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3374 )
	--]
	End()		

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

-- 가멜란 죽이는 애니메이션
	Action( "direct play" )
	--[
		Param( "tblidx", 60065 )
		Param( "event id", 884 )
		Param( "event type", "wps" )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60067 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3375 )
	--]
	End()		

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3376 )
	--]
	End()		

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

-- 대사 다헀다고 이벤트 쏨
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 885 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()