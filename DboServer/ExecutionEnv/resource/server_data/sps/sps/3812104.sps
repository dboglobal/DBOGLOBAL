
Scene(1, "2457" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

	Action( "say" )
	--[
		Param( "tblidx", 3295)
	--]
	End()
	
        Action( "point move" )
        --[
                Param( "loc x", 98.648 )
		Param( "loc y", -20.496 )
                Param( "loc z", 858.354 )
                Param( "run mode", 1 )
        --]
        End()
        
        Action( "send event to wps" )
        --[
                Param( "wps index", 2457 )
                Param( "event id", 556 )
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