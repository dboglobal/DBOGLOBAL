
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
		Param( "event id", 79 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 507 )
			Param( "event id", 226 )
		--]
		End()
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 6681110 )
		Param( "run mode", 1 )
	--]
	End()


	Action( "send event to wps" )
	--[
		Param( "wps index", 507 )
		Param( "event id", 229 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()