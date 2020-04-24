
Scene(1, "2459" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

	Action( "say" )
	--[
		Param( "tblidx", 3297 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 1.022 )
		Param( "loc y", -24.617 )
		Param( "loc z", 1021.004 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2459 )
                Param( "event id", 560 )
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