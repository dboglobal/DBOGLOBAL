Scene(1, "TMQ2-3" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 1 )
		Param( "run mode", 15 )
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

Scene(2, "TMQ2-3" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 1 )
		Param( "run mode", 19 )
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

Scene(3, "TMQ2-3" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 1 )
		Param( "run mode", 18 )
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

Scene(4, "TMQ2-3" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 1 )
		Param( "run mode", 17 )
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
