
Scene(1, "2553" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "point move" )
        --[
                Param( "loc x", -2397.562 )
                Param( "loc y", -140.987 )
                Param( "loc z", -4517.249 )
                Param( "run mode", 1 )
        --]
        End()

	Action( "say" )
	--[
		Param( "tblidx", 3302 )
	--]
	End()
	        
        Action( "target fix attack" )
        --[
                Param( "obj type", "npc" )
                Param( "bot tblidx", 3213107 )
                Param( "scan range", 50 )
                Param( "end type", "percent target lp" )
                Param( "end value", 0 )
        --]
        End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()