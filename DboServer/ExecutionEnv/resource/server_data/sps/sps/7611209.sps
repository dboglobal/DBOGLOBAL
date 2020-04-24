Scene(1,"∞≈∫œ¿Ã »£¿ß")
--[

	Action("wait")
	--[
		Condition("recv event from ts")
		--[
			Param("trigger type", "quest")
			Param("event id", 659)
		--]
		End()
	--]
	End()


	Action("escort")
	--[
		Param("type", "target follow")
		Param("dest x", -682.66)
		Param("dest z", 1684.36)
		Param("radius", 15)
		Param("event id", 658)
	--]
	End()
	
	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(2, "∞≈∫œ¿Ã »£¿ß II")
--[
	Action("wait")
	--[
		Condition("recv event from ts")
		--[
			Param("trigger type", "quest")
			Param("event id", 669)
		--]
		End()
	--]
	End()
	
	Action("escort")
	--[
		Param("type", "target follow")
		Param("dest x", -572.31)
		Param("dest z", 1831.03)
		Param("radius", 15)
		Param("event id", 639)
	--]
	End()
	
	Action("send event to wps")
	--[
		Param("wps index", 70120)
		Param("event id", 692)
	--]
	End()
	
	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(3, "∞≈∫œ¿Ã º˚±‚")
--[
	Action("point move")
	--[
		Param("loc x", -648.46)
		Param("loc y", -78.31)
		Param("loc z", 1648.67)
		Param("dir x", -0.10)
		Param("dir z", 0.99)
		Param("run mode", 1)
	--]
	End()
	
	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(4, "∞≈∫œ¿Ã πŸ¥Ÿ∑Œ ∞°±‚")
--[
	Action("point move")
	--[
		Param("loc x", -566.58)
		Param("loc y", -118.17)
		Param("loc z", 1855.09)
		Param("dir x", 0.31)
		Param("dir z", 0.95)
		Param("run mode", 1)
	--]
	End()
	
	Action("send event to wps")
	--[
		Param("wps index", 70120)
		Param("event id", 668)
	--]
	End()
	
	Action("leave")
	--[
	--]
	End()
	
	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(5,"∞≈∫œ¿Ã »£¿ß III")
--[

	Action("wait")
	--[
		Condition("recv event from ts")
		--[
			Param("trigger type", "quest")
			Param("event id", 751)
		--]
		End()
	--]
	End()


	Action("escort")
	--[
		Param("type", "target follow")
		Param("dest x", -641.86)
		Param("dest z", 1656.06)
		Param("radius", 15)
		Param("event id", 752)
	--]
	End()
	
	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(6, "∞≈∫œ¿Ã º˚±‚")
--[
	Action("point move")
	--[
		Param("loc x", -711.63)
		Param("loc y", -75.92)
		Param("loc z", 1685.56)
		Param("dir x", 0.99)
		Param("dir z", -0.12)
		Param("run mode", 1)
	--]
	End()
	
	Action("sps end")
	--[
	--]
	End()
--]
End()
