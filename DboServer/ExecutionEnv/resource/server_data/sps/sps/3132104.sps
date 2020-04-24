Scene( 1, "3132104" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3249 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 1003 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3250 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2410 )
		Param( "event id", 503 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
