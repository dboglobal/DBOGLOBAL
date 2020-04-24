Scene( 1, "2984" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 1 )
		Param( "attack disallow", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3056)
	--]
	End()	
	
	Action( "point move")
	--[
		Param( "loc x", 4949.49)
		Param( "loc y", -41.49)
		Param( "loc z", -4163.26)
		Param( "dir x", -1.00 )
		Param( "dir z", -0.02 )	
		Param( "run mode", 1 )	
	--]
	End()			
	
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "attack disallow", 0 )
	--]
	End()			
	
	Action( "ai bit flag" )
	--[
		Param( "attack type", "offensive" )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 2984 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	