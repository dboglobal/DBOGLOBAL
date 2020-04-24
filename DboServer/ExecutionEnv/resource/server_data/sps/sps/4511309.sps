--ver03

Scene(1, "Q351")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3010 )
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7111)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 351)
		Param("event id", 462)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 351)
			Param("event id", 463)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3230)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 351)
			Param("event id", 464)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3231)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7112)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3012 )
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60015)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60016)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 351)
		Param("event id", 465)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()
