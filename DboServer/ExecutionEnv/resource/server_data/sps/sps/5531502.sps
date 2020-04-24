Scene( 1, "5531502" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3253 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 1006 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3254 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2435 )
		Param( "event id", 508 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
