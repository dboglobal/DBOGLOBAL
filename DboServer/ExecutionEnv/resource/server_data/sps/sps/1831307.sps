
Scene(1, "2417" )
--[
	Action( "say" )
	--[
		Param( "tblidx", 3035 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 768.781 )
		Param( "loc y", 6.442 )
		Param( "loc z", 392.637 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 2417 )
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

