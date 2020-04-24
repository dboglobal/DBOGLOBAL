
Scene(1, "2442" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3284 )
        --]
        End()

	Action( "point move" )
	--[
		Param( "loc x", -518.349 )
		Param( "loc y", -2.033 )
		Param( "loc z", 894.467 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2442 )
                Param( "event id", 549 )
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