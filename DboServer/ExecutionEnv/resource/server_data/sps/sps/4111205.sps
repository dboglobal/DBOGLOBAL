Scene( 1, "4111205" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3263 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7026 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3264 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2543 )
		Param( "event id", 518 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
