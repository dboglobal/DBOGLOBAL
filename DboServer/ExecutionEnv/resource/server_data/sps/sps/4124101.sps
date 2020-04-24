Scene(1,"1608 무스탕 부하")
--[

	Action("char condition")
	--[
		Param("ui disable",1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", -1276.635)
		Param("loc z", 1221.921)
		Param("dir x", -0.284)
		Param("dir z", -0.959)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3178)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 320)
	--]
	End()

--무스탕대사 2 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 323)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3180)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 324)
	--]
	End()

-- 악수 대기 

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 326)
		--]
		End()
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60041)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 328)
	--]
	End()

--악수연출 완료 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 329)
		--]
		End()
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60042)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 330)
	--]
	End()

--무스탕 대사 3 완료 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 334)
		--]
		End()
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60044)
	--]
	End()

	Action("wait")
	--[
		Param("time",2)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3185)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", -1267)
		Param("loc z", 1238)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 336)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
