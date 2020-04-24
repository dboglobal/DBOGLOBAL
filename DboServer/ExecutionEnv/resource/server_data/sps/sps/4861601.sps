Scene( 1, "34007" )
--[
--쓰러진 가멜란

--대사 기다림
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34007 )
			Param( "event id", 881 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3373 )
	--]
	End()		

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

-- 오브젝트 움직여라는 이벤트 쏨
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id",  886 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()