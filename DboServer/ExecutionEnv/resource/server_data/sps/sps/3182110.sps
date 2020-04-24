Scene( 1, "3182110" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3243 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7025 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3244 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2303 )
		Param( "event id", 496 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
