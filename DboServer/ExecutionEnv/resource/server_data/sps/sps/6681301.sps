
Scene(1,"718")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()

	Action( "say" )
	--[
		-- "ÄíÇìÇì~¾êµé¾Æ Àâ¾Æ¶ó"
		Param( "tblidx" , 3053 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 4 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 7047.07 )
		Param( "loc y", -111.525 )
		Param( "loc z", -50.43 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "attack disallow", 0 )
		Param( "invincible", 0 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "offensive" )
	--]
	End()

	Action( "sps end" )
	--[
		
	--]
	End()
--]
End()







