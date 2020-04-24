
Scene( 1, "Q292" )
--[	
	Action( "path move" )
	--[
		Param( "tblidx", 292 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 292 )
		Param( "event id", 213 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()