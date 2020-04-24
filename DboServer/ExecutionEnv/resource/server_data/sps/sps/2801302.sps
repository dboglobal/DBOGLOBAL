Scene(1,"1512")
--[

	Action("char condition")
	--[
		Param("ui disable",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3161)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1512)
		Param("event id", 353)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1512)
			Param("event id", 342)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3162)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1512)
			Param("event id", 343)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3163)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3164)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1512)
		Param("event id", 344)
	--]
	End()	

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1512)
			Param("event id", 345)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3162)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1512)
			Param("event id", 346)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3163)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3165)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1512)
		Param("event id", 347)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1512)
			Param("event id", 348)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3162)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1512)
			Param("event id", 349)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3166)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1512)
		Param("event id", 350)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()

	