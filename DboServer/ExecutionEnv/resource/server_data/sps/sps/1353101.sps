--ver01

Scene(1,"534")
--[
	Action("send event to wps")
	--[
		Param("wps index", 534)
		Param("event id", 205)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		--크헉헉! 잘못했어. 한번만 봐줘~ 질 좋은 소금은 지금까지 내가 다른 곳에다 몰래 팔았다구.
		Param("tblidx", 3056)
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60023)
	--]
	End()

	Action("say")
	--[
		--지금 남아 있는 좋은 소금들을 다 줄테니까 제발 용서해줘~ 
		Param("tblidx", 3057)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 534)
		Param("event id", 62)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
