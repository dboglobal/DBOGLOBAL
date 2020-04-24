
Scene(1,"1131")
--[
	
	Action("char condition")
	--[
		Param("invincible", 0)
		Param("ui disable", 1)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2029)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1131)
		Param("event id", 310)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1131)
			Param("event id", 311)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3158)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1131)
			Param("event id", 312)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3159)
	--]
	End()

	Action("path move")
	--[	
		Param("tblidx", 2030)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1131)
		Param("event id", 313)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1131)
			Param("event id", 314)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3158)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1131)
			Param("event id", 315)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3159)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2031)
		Param("run mode",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3160)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1131)
		Param("event id", 316)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()	