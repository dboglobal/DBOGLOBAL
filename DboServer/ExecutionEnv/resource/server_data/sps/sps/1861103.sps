Scene( 1, "1861103" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3251 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 1004 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3252 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2414 )
		Param( "event id", 506 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
