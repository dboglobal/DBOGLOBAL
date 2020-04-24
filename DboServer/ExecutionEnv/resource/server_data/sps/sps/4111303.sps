
Scene(1, "2544" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

	Action( "say" )
	--[
		Param( "tblidx", 3259 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", -2289.636 )
		Param( "loc y", -99.443 )
		Param( "loc z", -3768.487 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2544 )
                Param( "event id", 567 )
        --]
        End()
 
        Action( "leave" )
        --[
        
        --]
        End() 
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()