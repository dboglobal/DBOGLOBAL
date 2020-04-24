Scene( 1, "3000" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()		

	Action( "path move" )
	--[
		Param( "tblidx", 3000 )
		Param( "run mode", 1)
	--]
	End()		
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 3000 )
		Param( "event id", 710 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	