
Scene(1, "2417" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 766.779 )
		Param( "loc y", 6.352 )
		Param( "loc z", 395.817 )
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

Scene(2, "2417" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 767.154 )
		Param( "loc y", 6.836 )
		Param( "loc z", 388.871 )
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