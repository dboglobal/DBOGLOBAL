
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
		Param( "tblidx", 3290 )
	--]
	End()
        
        Action( "point move" )
        --[
                Param( "loc x", -433.145 )
		Param( "loc y", -106.728 )
                Param( "loc z", -3287.913 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -444.122 )
		Param( "loc y", -103.849 )
                Param( "loc z", -3280.856 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -457.346 )
		Param( "loc y", -103.581 )
                Param( "loc z", -3289.961 )
                Param( "run mode", 1 )
        --]
        End()
       
        Action( "send event to wps" )
        --[
                Param( "wps index", 2620 )
                Param( "event id", 578 )
        --]
        End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()