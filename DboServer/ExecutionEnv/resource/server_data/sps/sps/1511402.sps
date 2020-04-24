Scene( 1, "1511402" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3265 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7027 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3266 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2571 )
		Param( "event id", 520 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
