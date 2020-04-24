
Scene(1,"1411")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "defensive")
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3153)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2022)
		Param( "run mode", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3157)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()
	
	Action("path move")
	--[
		Param("tblidx", 2023)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1411)
		Param("event id", 305)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1411)
			Param("event id", 306)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3154)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1411)
			Param("event id", 307)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3155)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2024)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3156)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1411)
		Param("event id", 308)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()


