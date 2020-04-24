
Scene(1, "507" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3061 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 4151202 )
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "send event to wps")
	--[
		Param( "wps index", 507 )
		Param( "event id", 77 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 507 )
			Param( "event id", 81 )
		--]
		End()
	--]
	End()

	Action( "send event to wps")
	--[
		Param( "wps index", 507 )
		Param( "event id", 227 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3059 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 507 )
			Param( "event id", 230 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3060 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()
