
Scene(1, "507" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 507 )
		Param( "event id", 78 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 507 )
			Param( "event id", 225 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3058 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 3242108 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 507 )
		Param( "event id", 228 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()
