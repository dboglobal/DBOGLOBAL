
Scene(1, "2458" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

	Action( "say" )
	--[
		Param( "tblidx", 3296 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 173.531 )
		Param( "loc y", 4.115 )
		Param( "loc z", 1069.975 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2458 )
                Param( "event id", 558 )
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