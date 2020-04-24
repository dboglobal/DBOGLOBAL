------------------------------------------------------
-- 할아버지(손오반)
-- 작성자 : SongJiChul (reep@ntl-inc.com)
------------------------------------------------------

-- TLQ1 User에게 RP스킬사용 연습을 시키는 할아버지

Scene(1, "11604")
	--무적으로 만들어서 User 가 연습하다가 할아버지가 죽는 일이 없도록한다
	--mob이 대사 하기 전에 공격받지 않도록 Param("attack disallow", 1) 설정한다

	Action("char condition")
	--[
		Param("invincible", 1)
		Param("ui disable", 1)
		Param("attack disallow", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3005)
	--]
	End()

	Action("char condition")
	--[
		Param("attack disallow", 0)
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()

End()