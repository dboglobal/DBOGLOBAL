
Scene(1, "6037")
--[
	Action( "direct play" )
	--[
		Param( "tblidx", 60001 )
		Param( "event type", "ts" )
		Param( "event id", 428 )
	--]
	End()
	
	Action( "direct play" )
	--[
		Param( "tblidx", 60002 )
	--]
	End()

	Action( "say" )
	--[
		-- "Ah. I lived."
		Param( "tblidx", 3313 ) 
	--]
	End()

	Action( "wait")
	--[
		Param( "time", 2 )
	--]
	End()
	
	Action( "say" )
	--[
		-- "Thanks."
		Param( "tblidx", 3314 ) 
	--]
	End()
	
	Action( "direct play" )
	--[
		Param( "tblidx", 60003 )
	--]
	End()
	
	Action( "direct play" )
	--[
		Param( "tblidx", 60004 )
	--]
	End()


	Action( "send event to wps" )
	--[
		Param( "wps index", 6037 )
		Param( "event id", 429 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 5779.379 )
		Param( "loc y", -90.418 )
		Param( "loc z", 3999.440 )
		Param( "dir x", 0.731 )
		Param( "dir z", 0.683 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()	
--]
End()
