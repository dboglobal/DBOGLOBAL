Scene( 1, "927")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3091)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",927)
			Param("event id", 240)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3092)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",927)
			Param("event id", 241)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3093)
	--]
	End()

	Action("wait")
	--[
		Param("time",2)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3094)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
