
Scene(1,"469")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 3272102 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3074 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 469 )
		Param( "event id", 88 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 469 )
			Param( "event id", 92 )
		--]
		End()
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 3272104 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()