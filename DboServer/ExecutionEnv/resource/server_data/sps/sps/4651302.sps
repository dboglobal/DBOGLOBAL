-- µ§µ§Æ÷·Î
Scene( 1, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 994 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()