
Scene(1, "6004")
--[
	Action("wait")
	--[
		Param("operation", "or")

		Condition("child")
		--[
			Action( "wait" )
			--[
				Condition( "recv event from wps" )
				--[
					Param( "wps index", 6004 )
					Param( "event id", 600401 )
				--]
				End()
			--]
			End()
	
			Action( "say" )
			--[
				Param( "tblidx", 3035 )
			--]
			End()
	
			Action( "wait" )
			--[
				Param( "time", 3 )
			--]
			End()
			
			Action( "send sps event" )
			--[
				Param( "event type", "wps" )
				Param( "event id", 600402 )
			--]
			End()

			Action( "wait" )
			--[
				Condition( "recv event from wps" )
				--[
					Param( "wps index", 6004 )
					Param( "event id", 600403 )
				--]
				End()
			--]
			End()

			Action( "wait" )
			--[
				Param( "time", 2 )
			--]
			End()

			Action( "say" )
			--[
				Param( "tblidx", 3036 )
			--]
			End()

			Action( "wait" )
			--[
				Param( "time", 3 )
			--]
			End()

			Action( "say" )
			--[
				Param( "tblidx", 3037 )
			--]
			End()
		--]
		End()

		Condition("recv event from wps")
		--[
			Param( "wps index", 6004 )
			Param( "event id", 197)
		--]
		End()
	--]
	End()
--]
End()

