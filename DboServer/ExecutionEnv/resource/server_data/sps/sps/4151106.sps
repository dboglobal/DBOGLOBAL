
Scene(1,"670")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3068 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 670 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 670 )
		Param( "event id", 56 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3069 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 670 )
			Param( "event id", 210 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3070 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 6700 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3071 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 670 )
		Param( "event id", 211 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()