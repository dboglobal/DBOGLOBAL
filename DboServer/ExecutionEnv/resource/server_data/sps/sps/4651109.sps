Scene(1, "UD6 키리정제소 NPC 발간")
--[
	--NPC의 UI를 가려준다.
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()
	
	--34012 WPS에서 이벤트를 받는다.
	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 34012)
			Param("event id", 975)
		--]
		End()
	--]
	End()
	
	--시간을 5초 기다린다.
	Action("wait")
	--[
		Param("time", 5)
	--]
	End()
	
	--NPC의 UI를 보여준다.
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