
Scene(1, "Å¥ÀÌ" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "defensive" )
		Param( "move type", "move off" )
	--]
	End()
	
	Action( "wait" )
	--[
		Condition( "recv event from ts" )
		--[
			Param( "trigger type", "quest" )
			Param( "event id", 21040 )
		--]
		End()
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
		Param( "move type", "move on" )
	--]
	End()
	
	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()

