
Scene(1,"762")
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
		Param( "attack type", "timid" )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3063 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 7523.096 )
		Param( "loc y", -116.762 )
		Param( "loc z", -930.397)
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

	Action( "say" )
	--[
		Param( "tblidx", 3072 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 762 )
			Param( "event id", 231 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3064 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()
