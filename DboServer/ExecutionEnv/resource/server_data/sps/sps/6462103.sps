
Scene(1, "2620" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "wait" )
        --[
 		Param( "time", 5 )
        --]
        End()

	Action( "say" )
	--[
		Param( "tblidx", 3292 )
	--]
	End()
	        
        Action( "point move" )
        --[
                Param( "loc x", -407.921 )
		Param( "loc y", -106.939 )
                Param( "loc z", -3279.875 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -388.480 )
		Param( "loc y", -108.957 ) 
                Param( "loc z", -3282.597 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -402.815 )
		Param( "loc y", -111.297 ) 
                Param( "loc z", -3287.826 )
                Param( "run mode", 1 )
        --]
        End()
       
        Action( "send event to wps" )
        --[
                Param( "wps index", 2620 )
                Param( "event id", 580 )
        --]
        End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()