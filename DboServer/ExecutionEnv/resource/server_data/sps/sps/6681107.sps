
Scene(1,"559")
--[
	Action( "char condition" )
	--[
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3062 )
	--]
	End()

	Action("point move")
	--[
		Param( "loc x", -135 )
		Param( "loc y", 5.031 )
		Param( "loc z", 522 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "attack disallow", 0 )
		Param( "invincible",0 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "offensive" )
	--]
	End()

	Action("sps end")
	--[

	--]
	End()

--]
End()


