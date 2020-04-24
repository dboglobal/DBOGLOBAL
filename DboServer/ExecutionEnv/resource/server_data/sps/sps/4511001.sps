-- 전사포로
Scene( 1, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 993 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()