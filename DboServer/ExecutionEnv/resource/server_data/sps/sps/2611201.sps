--Ver. 01

Scene(1,"555")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 555 )
		Param( "event id", 206 )
	--]
	End()

	Action( "escort" )
	--[
		Param( "type", "under escort" )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 555 )
		Param( "run mode", 0 )
		Param( "radius", 5 )
	--]
	End()

	Action( "send sps event" )
	--[
		Param( "event type", "escort" )
		Param( "trigger type", "quest" )
		Param( "event id", 63 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3060 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3061 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60024 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60025 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 4 )
	--]
	End()
	
	Action( "say" )
	--[
		Param( "tblidx", 3062 )
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

	Action( "send sps event" )
	--[
		Param( "event type", "wps" )
		Param( "trigger type", "quest" )
		Param( "event id", 66 )
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
