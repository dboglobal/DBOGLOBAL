Scene( 1, "1657102" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	-- 이런 실수했다. 난 일이 있어서 먼저 간다.
	Action( "say" )
	--[
		Param( "tblidx", 3270 )
	--]
	End()
	
	-- 대사를 반복해서 하게 하기 위해 sps end넣지 않음

--]
End()
