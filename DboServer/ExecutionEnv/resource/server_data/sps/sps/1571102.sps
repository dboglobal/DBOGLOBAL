
Scene(1,"1881")
--[
	Action("char condition")
	--[
		Param("ui disable",1)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2000)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1881)
		Param("event id", 242)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1881)
			Param("event id", 261)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3085)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1881)
			Param("event id", 243)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3086)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2001)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1881)
		Param("event id", 244)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1881)
			Param("event id", 262)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3085)
	--]
	End()

	
	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1881)
			Param("event id", 245)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3087)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2002)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1881)
		Param("event id", 246)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1881)
			Param("event id", 263)
		--]
		End()
	--]
	End()


	Action("say")
	--[
		Param("tblidx", 3085)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1881)
			Param("event id", 247)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3088)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2003)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3089)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3090)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1881)
		Param("event id", 248)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()
