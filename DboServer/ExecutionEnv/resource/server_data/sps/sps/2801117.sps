Scene(1, "2143" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3235 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7021 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3236 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2143 )
		Param( "event id", 488 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
