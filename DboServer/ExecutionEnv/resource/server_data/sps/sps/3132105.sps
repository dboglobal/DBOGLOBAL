Scene( 1, "2984" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2984)
		Param( "event id", 702 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	