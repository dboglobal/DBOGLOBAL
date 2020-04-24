Scene(1,"966")
--[
	Action("wait")
	--[
		Condition("recv event from item")
		--[
			Param("item index", 98007)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3202)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 966)
		Param("event id", 403)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()

