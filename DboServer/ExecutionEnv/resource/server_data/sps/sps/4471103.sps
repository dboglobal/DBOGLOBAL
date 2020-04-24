Scene(1, "2931" )
--[
	Action( "wait" )
	--[
		Condition( "recv event from item" )
		--[
			Param( "item index", 98028 )
		--]
		End()
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2931 )
		Param( "event id", 791 )
	--]
	End()

	Action( "send event to ts" )
	--[
		Param( "trigger type", "quest" )
		Param( "event id", 833 )
	--]
	End()
--]
End()