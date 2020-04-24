Scene(1, "UD6 키리정제소 NPC 정제소 관리하사 2")
--[
	
	--34012 WPS에서 이벤트를 받는다.
	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 34012)
			Param("event id", 976)
		--]
		End()
	--]
	End()
	
	--NPC의 UI를 가려준다.
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()
	
	Action("sps end")
	--[
	
	--]
	End()

--]
End()