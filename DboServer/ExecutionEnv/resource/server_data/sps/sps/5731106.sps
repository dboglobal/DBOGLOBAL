
Scene(1, "2368" )
--[
	Action( "wait" )
	--[
		Condition( "recv event from item" )
		--[
			Param( "item index", 98017 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3285 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2368 )
		Param( "event id", 536 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()

Scene(2, "2369" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()
        
	Action( "point move" )
	--[
		Param( "loc x", 801.287 )
		Param( "loc y", -26.579 )
		Param( "loc z", -383.324 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2369 )
		Param( "event id", 538 )
	--]
	End()
        
	Action( "sps end" )
	--[

	--]
	End()
--]
End()

Scene( 3, "2365" )
--[
	Action( "wait" )
	--[
		Condition( "recv event from item" )
		--[
			Param( "item index", 98016 )
		--]
		End()
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2365 )
		Param( "event id", 587 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()