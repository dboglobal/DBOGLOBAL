Scene(1, "TMQ2-3" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 36 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 12312 )
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

