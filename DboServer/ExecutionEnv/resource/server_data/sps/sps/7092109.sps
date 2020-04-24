Scene( 1, "7092109" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3245 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 1001 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3246 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2392 )
		Param( "event id", 498 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
