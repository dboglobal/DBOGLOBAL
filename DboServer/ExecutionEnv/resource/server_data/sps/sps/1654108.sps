
Scene(1,"461")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 461 )
		Param( "event id", 210 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 461 )
			Param( "event id", 211 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		-- "으악! 우릴 공격한다." 
		Param( "tblidx", 3312 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()

