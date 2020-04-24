Scene( 1, "2924" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 1 )
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
	--]
	End()

	Action( "point move")
	--[
		Param( "loc x", 2394.65 )
		Param( "loc y", -58.20 )
		Param( "loc z", -4047.84 )
		Param( "dir x", 0 )
		Param( "dir z", 1.0 )		
	--]
	End()	
	
	Action("say")
	--[
		Param( "tblidx", 3324 )
	--]
	End()				

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say")
	--[
		Param( "tblidx", 3325 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "say")
	--[
		Param( "tblidx", 3326 )
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
				
	Action( "path move" )
	--[
		Param( "tblidx", 124 )
		Param( "run mode", 1)
	--]
	End()	
	
	Action( "say")
	--[
		Param( "tblidx", 3327 )
	--]
	End()		
		
	Action( "path move" )
	--[
		Param( "tblidx", 125 )
		Param( "run mode", 1)
	--]
	End()	

	Action( "say")
	--[
		Param( "tblidx", 3328 )
	--]
	End()	

	Action( "path move" )
	--[
		Param( "tblidx", 126 )
		Param( "run mode", 1)
	--]
	End()	
	
	Action( "say")
	--[
		Param( "tblidx", 3329 )
	--]
	End()	
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "say")
	--[
		Param( "tblidx", 3330 )
	--]
	End()				

	Action( "path move" )
	--[
		Param( "tblidx", 135 )
		Param( "run mode", 1)
	--]
	End()			
		
	Action( "say")
	--[
		Param( "tblidx", 3331 )
	--]
	End()	

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say")
	--[
		Param( "tblidx", 3332 )
	--]
	End()	
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 2924)
		Param( "event id", 695)
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	