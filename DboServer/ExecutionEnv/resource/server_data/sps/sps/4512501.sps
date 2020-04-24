-- UD6
-- 墨林(唱皋农府胶呕 备开)

Scene(1, "34001")
--[
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34001 )
			Param( "event id", 888 )
		--]
		End()
	--]
	End()
	
	Action( "char condition")
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
