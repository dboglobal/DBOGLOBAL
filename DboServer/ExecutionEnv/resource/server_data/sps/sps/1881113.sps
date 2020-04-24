-- UD5 Normal
-- Gas concentrated in Pudong
-- Should also spawn during the first click is also designed not to damage the mouth. Mobility should

Scene( 1, "1881113" )
--[
	Action( "char condition" )
	--[
		Param( "click disable", 1 )
		Param( "invincible", 1 )
	--]
	End()
	
	Action( "ai bit flag" )
	--[
		Param( "move type", "move off" )
	--]
	End()
--]
End()