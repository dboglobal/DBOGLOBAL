
Scene(1, "2460" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "wait" )
        --[
                Condition( "recv event from wps" )
                --[
                        Param( "wps index", 2460 )
                        Param( "event id", 563 )
                --]
                End()
        --]
        End()
        
        Action( "say" )
        --[
                Param( "tblidx", 3281 )
        --]
        End()

	Action( "point move" )
	--[
		Param( "loc x", 165.467 )
		Param( "loc y", -33.997 )
		Param( "loc z", 403.763 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2460 )
                Param( "event id", 564 )
        --]
        End()
 
	Action( "sps end" )
        --[
        
        --]
        End()
--]
End()