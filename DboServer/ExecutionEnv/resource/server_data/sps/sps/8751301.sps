
Scene(1, "2442" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3288 )
        --]
        End()

	Action( "point move" )
	--[
		Param( "loc x", -521.715 )
		Param( "loc y", -1.698 )
		Param( "loc z", 894.523 )
		Param( "run mode", 1 )
	--]
	End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2442 )
                Param( "event id", 550 )
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