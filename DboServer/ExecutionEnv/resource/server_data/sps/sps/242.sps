-- 2011³â 05¿ù 13ÀÏ ÇöÀç »ç¿ë ¾ÈÇÔ ---
--ver02

Scene(1, "242")
--[
--	Action( "say" )
	--[
--		"¿ì¿Õ¤»±»¤»"
	--]
--	End()

	Action( "wait")
	--[
		Param( "time", 2 )
	--]
	End()

--	Action( "say" )
	--[
--		"³­ ¾Æ´Ï¶ó´« »ì·Á´Þ¶ó´«"
	--]
--	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 4060.823 )
		Param( "loc z", -2878.556 )
		Param( "run mode", 1)
	--]
	End()

	Action( "leave" )
	--[
	
	--]
	End()

--]
End()


Scene(2, "242")
--[

	Action( "point move" )
	--[
		Param( "loc x", 4097.48 )
		Param( "loc z", -2913.57 )
		Param( "run mode", 1)
	--]
	End()

--	Action( "say" )
	--[
--		"ÈÞ »ì¾Ò´ç" 
	--]
--	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

--	Action( "say" )
	--[
--		"ÇÇ¶Ë½Ò»·Çß³×"
	--]
--	End()

	Action( "sps end" )
	--[

	--]
	End()

--]
End()