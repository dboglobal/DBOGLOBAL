Scene(1, "1300")
--[
	Action("say")
	--[
		Param("tblidx", 3007)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2218.213)
		Param("loc y", -49.915)
		Param("loc z", 3227.997)
		Param("run mode", 1)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(2, "1300")
--[
	Action("say")
	--[
		Param("tblidx", 3008)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2210.030)
		Param("loc y", -49.899)
		Param("loc z", 3217.590)
		Param("run mode", 1)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(3, "1300")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
		Param("invincible", 1)
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "defensive")
	--]
	End()
	
	Action("say")
	--[
		Param("tblidx", 3009)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
		Param("invincible", 0)
	--]
	End()
	
	Action("point move")
	--[
		Param("loc x", 1835)
		Param("loc y", -65.65)
		Param("loc z", 3408.65)
		Param("dir x", 0.985)
		Param("dir z", -0.172)
		Param("run mode", 1)
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "offensive")
	--]
	End()

	Action("sps end")
	--[

	--]
	End()
--]
End()

Scene(4, "1300")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
		Param("invincible", 1)
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "defensive")
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3010)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
		Param("invincible", 0)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 1848.741)
		Param("loc y", -65.65)
		Param("loc z", 3407.208)
		Param("dir x", -0.986)
		Param("dir z", 0.16)
		Param("run mode", 1)
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "offensive")
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
