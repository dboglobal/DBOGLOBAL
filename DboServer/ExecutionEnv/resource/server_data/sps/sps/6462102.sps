
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
		Param( "tblidx", 3291 )
	--]
	End()
	        
        Action( "point move" )
        --[
                Param( "loc x", -417.954 )
		Param( "loc y", -110.274 ) 
                Param( "loc z", -3289.867 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -421.979 )
		Param( "loc y", -109.915 ) 
                Param( "loc z", -3299.257 )
                Param( "run mode", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -407.682 )
		Param( "loc y", -108.140 ) 
                Param( "loc z", -3314.149 )
                Param( "run mode", 1 )
        --]
        End()
        
        Action( "send event to wps" )
        --[
                Param( "wps index", 2620 )
                Param( "event id", 579 )
        --]
        End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()