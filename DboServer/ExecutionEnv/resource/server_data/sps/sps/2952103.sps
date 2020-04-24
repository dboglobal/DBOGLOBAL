Scene( 1, "2952103" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3247 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 1002 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3248 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2409 )
		Param( "event id", 500 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
