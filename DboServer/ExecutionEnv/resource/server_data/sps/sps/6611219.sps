Scene(1,"1133")
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
		Param("tblidx", 3013)
	--]
	End()

	Action("char condition")
	--[
		Param("invincible", 0)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", -410.592)
		Param("loc z", 1650.821)
		Param("dir x", 0.864)
		Param("dir z", -0.5)
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
