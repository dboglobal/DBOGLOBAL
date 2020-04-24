Scene( 1, "34007" )
--[
--가멜란
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

--대사 완료 기다림
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34007 )
			Param( "event id", 897 )
		--]
		End()
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()			

--정신 돌아왔다		
	Action( "say" )
	--[
		Param( "tblidx", 3361 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()			
		
	Action( "say" )
	--[
		Param( "tblidx", 3362 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3363 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", -399.602 )
		Param( "loc y", 144.86 )
		Param( "loc z", 714.42 )
		Param( "dir x", 0.83 )
		Param( "dir z", 0.56 )
		Param( "run mode", 1 )
	--]
	End()

-- 자폭 이벤트 쏨	
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id",  898 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	