Scene( 1, "34007" )
--[
--ºñºê¶ó 
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34007 )
			Param( "event id", 893 )
		--]
		End()
	--]
	End()			

--³ª»Û °¡¸á¶õ ´ë»ç		
	Action( "say" )
	--[
		Param( "tblidx", 3355 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 890 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	