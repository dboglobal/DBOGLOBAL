Scene( 1, "7632107" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3241 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7024 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3242 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2285 )
		Param( "event id", 494 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
