-- Vehicle roam the western city of NPC
-- Sps file to run in maeptul Lim

Scene(1, "car")
--[
	Action( "char condition")
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 128 )
		Param( "run mode", 0 )
	--]
	End()
--]
End()