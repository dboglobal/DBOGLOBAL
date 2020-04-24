Scene(1, "1598")
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
		Param("tblidx", 3011)
	--]
	End()

	Action("char condition")
	--[
		Param("invincible", 0)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", -2830.826)
		Param("loc y", -38.952)
		Param("loc z", 1239.241)
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
