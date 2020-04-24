Scene( 1, "3006" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 3006 )
		Param( "run mode", 1)
	--]
	End()	
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 3006 )
		Param( "event id", 714 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	