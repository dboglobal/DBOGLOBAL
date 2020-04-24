
Scene(1, "1329" )
--[
	Action( "function" )
	--[
		Condition( "child" )
		--[
			Action( "wait" )
			--[
				Condition( "recv event from wps" )
				--[
					Param( "wps index", 1329 )
					Param( "event id", 106 )
				--]
				End()
			--]
			End()

			Action( "point move" )
			--[
				Param( "loc x", 2433.892 )
				Param( "loc z", 3180.351 )
				Param( "run mode", 1 )
			--]
			End()

			Action( "function" )
			--[
				Action( "function" )
				--[
					Condition( "child" )
					--[
						Action( "wait" )
						--[
							Param( "time", 2 )
						--]
						End()


						Action( "say" )
						--[
							Param( "tblidx", 3027 )
						--]
						End()
					--]
					End()

					Action( "wait" )
					--[
						Condition( "recv event from wps" )
						--[
							Param( "wps index", 1329 )
							Param( "event id", 107 )
						--]
						End()
					--]
					End()
				--]
				End()

				Action( "say" )
				--[
					Param( "tblidx", 3028 )
				--]
				End()

				Action( "wait" )
				--[
					Param( "time", 3 )
				--]
				End()

				Action( "send event to wps" )
				--[
					Param( "wps index", 1329 )
					Param( "event id", 108 )
				--]
				End()

				Action( "point move" )
				--[
					Param( "loc x", 2433.64 )
					Param( "loc z", 3170.00 )
					Param( "run mode", 1 )
				--]
				End()

				Action( "rotate" )
				--[
					Param( "dir x", -0.97 )
					Param( "dir y", 0 )
					Param( "dir z", 0.22 )
				--]
				End()
			--]
			End()	
		--]
		End()

		Action( "wait" )
		--[
			Condition( "recv event from wps" )
			--[
				Param( "wps index", 1329 )
				Param( "event id", 107 )
			--]	
			End()
		--]
		End()

		Action( "say" )
		--[
			Param( "tblidx", 3028 )
		--]
		End()

		Action( "wait" )
		--[
			Param( "time", 3 )
		--]
		End()

		Action( "send event to wps" )
		--[
			Param( "wps index", 1329 )
			Param( "event id", 108 )
		--]
		End()

		Action( "char condition" )
		--[
			Param( "ui disable", 0 )
		--]
		End()

		Action( "sps end" )
		--[

		--]
		End()
	--]
	End()
--]
End()

Scene(2, "1243")
--[
	Action("send event to wps")
	--[
		Param( "wps index", 1243)
		Param( "event id", 170)
	--]
	End()


	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1243)
			Param("event id", 171)
		--]
		End()
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable",1)
	--]
	End()

	Action("wait")
	--[
		Param("time", 5)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", 0.03)
		Param("dir y", 0)
		Param("dir z", 1.0)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3030)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3032)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3034)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", -0.97)
		Param("dir y",0)
		Param("dir z", 0.22)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1243)
		Param("event id", 201)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable",0)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
	

	
