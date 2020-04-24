
Scene(1, "6013")
--[
	Action( "wait" )
	--[
		Param( "operation", "or" )
		
		Condition( "child" )
		--[
			Action( "wait" )
			--[
				Condition( "recv event from wps")
				--[
					Param( "wps index", 6013 )
					Param( "event id", 194 )
				--]
				End()
			--]
			End()
		
			Action( "char condition" )
			--[
				Param( "ui disable", 1 )
			--]
			End()
		
			Action( "say" )
			--[
				Param( "tblidx", 3038 )
			--]
			End()
		
			Action( "wait" )
			--[
				Param( "time", 1 )
			--]
			End()

			Action( "send event to wps" )
			--[
				Param( "wps index", 6013 )
				Param( "event id", 195 )
			--]
			End()

			Action( "char condition" )
			--[
				Param( "ui disable", 0 )
			--]
			End()
		--]
		End()

		Condition( "recv event from wps" )
		--[
			Param( "wps index", 6013 )
			Param( "event id", 198 )
		--]
		End()
	--]
	End()
--]
End()
