
Scene(1, "2442" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3280 )
        --]
        End()

	Action( "point move" )
	--[
		Param( "loc x", -520.488 )
		Param( "loc y", -1.991 )
		Param( "loc z", 897.665 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2442 )
                Param( "event id", 548 )
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