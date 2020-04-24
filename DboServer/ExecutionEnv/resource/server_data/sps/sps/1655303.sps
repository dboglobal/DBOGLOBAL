Scene( 1, "1655303" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3261 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 1007 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3262 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2536 )
		Param( "event id", 516 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
