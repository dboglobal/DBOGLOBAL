
Scene(1, "2451" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "wait" )
        --[
                Param( "time", 3 )
        --]
        End()
           
        Action( "point move" )
        --[
                Param( "loc x", 188.713 )
                Param( "loc z", 921.730 )
		Param( "loc y", 2.340 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", 198.387 )
		Param( "loc y", 0.000 )
                Param( "loc z", 916.938 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", 209.429 )
		Param( "loc y", -16.825 )
                Param( "loc z", 896.600 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2451 )
                Param( "event id", 554 )
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