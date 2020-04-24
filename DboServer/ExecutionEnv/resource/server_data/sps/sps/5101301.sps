
Scene(1, "2620" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()
	
        Action( "direct play" )
        --[
                Param( "tblidx", 60030 )
		Param( "event type", "wps" )
		Param( "event id", 576 )
	--]
	End()
		
	Action( "send event to wps" )
	--[
		Param( "wps index", 2620 )
		Param( "event id", 577 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()