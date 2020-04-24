Scene(1, "3221212" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3239 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7023 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3240 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2199 )
		Param( "event id", 492 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
