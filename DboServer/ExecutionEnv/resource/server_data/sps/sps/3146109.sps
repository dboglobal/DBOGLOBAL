Scene( 1, "3146109" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3267 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7028 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3268 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2684 )
		Param( "event id", 522 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
