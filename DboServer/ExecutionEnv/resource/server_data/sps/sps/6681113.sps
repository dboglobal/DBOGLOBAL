
Scene(2, "760" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 7523.188 )
		Param( "loc y", -116 )
		Param( "loc z", -936.504 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "say" )
	--[
		-- "크으! 크르르르~~!!" 
		Param( "tblidx" , 3052 )
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








