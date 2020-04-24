Scene( 1, "empty")
--[
	Action("sps end")
	--[
	--]
	End()

--]
End()


Scene( 2, "Q107" )
--[	
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3224)
	--]
	End()

	Action("wait")
	--[
		Param("time", 10)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 101)
		Param("run mode", 0)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3225)
	--]
	End()

	Action("wait")
	--[
		Param("time", 5)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3226)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 102)
		Param("run mode", 0)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 107)
		Param("event id", 458)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 107)
			Param("event id", 459)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3227)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 107)
			Param("event id", 460)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3228)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 103)
		Param("run mode", 0)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3229)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 107)
		Param("event id", 461)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()
