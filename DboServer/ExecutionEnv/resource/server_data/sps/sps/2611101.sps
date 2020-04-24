--Ver. 01

Scene(1,"555")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()	

	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 65 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "rotate" )	
	--[
		Param( "dir x", -0.99 )
		Param( "dir y", 0 )
		Param( "dir z", -0.10 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3058 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60026 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60027 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3059 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 5550 )
		Param( "run mode", 1 )
		Param( "radius", 5 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()

	Action( "leave" )
	--[
	
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()

--]
End()
