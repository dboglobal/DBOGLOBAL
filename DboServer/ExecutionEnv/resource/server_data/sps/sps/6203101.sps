Scene(1,"1608 무스탕")
--[

	Action("char condition")
	--[
		Param("ui disable",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3177)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 319 )
	--]
	End()

--부하대사 1 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 321)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3179)
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
		Param("event id", 322)
	--]
	End()

--부하대사 2 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 325)
		--]
		End()
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60040)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()
	
	Action("say")
	--[
		Param("tblidx", 3181)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1608)
		Param("event id", 327)
	--]
	End()

--변신 연출 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 331)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3182)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3183)
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
		Param("event id", 332)
	--]
	End()

--원래모습 대기

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 333)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3184)
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60050)
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
		Param("event id", 335)
	--]
	End()

--무스탕 대사 4

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1608)
			Param("event id", 337)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3186)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1608)
		Param("event id", 338)
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
