
Scene(1, "1243")
--[

	Action("send event to wps")
	--[
		Param( "wps index", 1243)
		Param( "event id", 169)
	--]
	End()


	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1243)
			Param("event id", 173)
		--]
		End()
	--]
	End()


	Action( "char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2433.81)
		Param("loc z", 3176.6)
		Param("dir x", -0.08)
		Param("dir z", -0.9)
		Param("run mode", 1)
	--]
	End()

	Action("wait")
	--[
		Param("time",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3029)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3031)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3033)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2434.52)
		Param("loc z", 3184.59)
		Param("dir x", -0.99)
		Param("dir z", 0.12)
		Param("run mode",1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1243)
		Param("event id", 200)
	--]
	End()

	Action( "char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action("sps end")
	--[

	--]
	End()
--]
End()