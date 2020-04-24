
Scene(1, "1013")
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
			Param( "wps index", 1013 )
			Param( "event id", 101301 )
		--]
		End()
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 1013 )
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "say" )
	--[
        	--"¿ìÀ¡ ¸À¾ø¾î. ÀÌ°Ô ¹¹¾ß."
		Param( "tblidx" , 3315 )               
	--]     
	End()
	
	Action( "wait" )
	--[
		Param( "time", 2 )    
	--]
	End()

	Action( "say" )
	--[
        	-- "³»°¡ ´Ù½Ã´Â ÀÌ°É ¸Ô³ª ºÁ¶ó." 
		Param( "tblidx" , 3316 )              
	--]     
	End()
	
	Action( "wait" )
	--[
		Param( "time", 2 )    
	--]
	End()
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 1013 )
		Param( "event id", 101302 )
	--]
	End()
		
	Action( "leave" )
	--[
	
	--]
	End()
--]
End()	
	
	
	
	
	
	
	