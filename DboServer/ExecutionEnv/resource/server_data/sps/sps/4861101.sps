Scene( 1, "34007" )
--[
--가멜란
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

--대사 완료 기다림
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34007 )
			Param( "event id", 891 )
		--]
		End()
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()			

--멍청한 비브라 대사		
	Action( "say" )
	--[
		Param( "tblidx", 3356 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
		
	Action( "say" )
	--[
		Param( "tblidx", 3357 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

--역마봉파
	Action( "direct play" )
	--[
		Param( "tblidx", 60061 )
	--]
	End()

-- 역마봉파 이벤트 줌		
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 892 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	


Scene( 2, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	-- 생각보다 제법 하는군...
	Action( "say" )
	--[
		Param( "tblidx", 3378 )
	--]
	End()

	-- 시작지점으로 이동
	Action( "point move" )
	--[
		Param( "loc x", -399.25 )
		Param( "loc y", 144.86 )
		Param( "loc z", 719.4 )
		Param( "dir x", -0.91 )
		Param( "dir z", -0.41 )
		Param( "run mode", 1 )
	--]
	End()
	
	-- 포로를 본다(그런데 현재 rotate작동하지 않음)
	Action( "rotate" )
	--[
		Param( "dir x", -0.91 )
		Param( "dir z", -0.41 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()
	
	-- 나와랏! 나의 포로들아. 
	Action( "say" )
	--[
		Param( "tblidx", 3369 )
	--]
	End()
	
	-- wps에 포로 npc spawn시키라고 알려줌. (wps에서는 잠시의 딜레이 후 npc spawn시킴)
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 987 )
	--]
	End()
	
	-- 가멜란이 포로소환 애니메이션 재생
	Action( "direct play" )
	--[
		Param( "tblidx", 60063 )
	--]
	End()
	
	-- wps에서 포로 npc 모두 spawn시킬 때 까지 대기
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 34007 )
			Param( "event id", 988 )
		--]
		End()
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()

	-- 포로 방향으로 한 발짝 이동
	Action( "point move" )
	--[
		Param( "loc x", -399.47 )
		Param( "loc y", 144.86 )
		Param( "loc z", 718.12 )
		Param( "dir x", -0.59 )
		Param( "dir z", -0.81 )
		Param( "run mode", 0 )
	--]
	End()
	
	-- 가소롭군! 덤벼라. 애송이들아.
	Action( "say" )
	--[
		Param( "tblidx", 3370 )
	--]
	End()
	
	-- wps에 알려줌
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 989 )
	--]
	End()
	
	-- 가멜란 포로흡수
	Action( "direct play" )
	--[
		Param( "tblidx", 60064 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()



Scene( 3, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	Action( "say" )
	--[
		Param( "tblidx", 3379 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", -399.25 )
		Param( "loc y", 144.86 )
		Param( "loc z", 719.4 )
		Param( "dir x", -0.91 )
		Param( "dir z", -0.41 )
		Param( "run mode", 1 )
	--]
	End()
	
	-- 포로를 본다(그런데 현재 rotate작동하지 않음)
	Action( "rotate" )
	--[
		Param( "dir x", -1 )
		Param( "dir z", 0.07 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()
	
	-- 포로따위의 힘을 사용하는 것을 영광으로 알아라.
	Action( "say" )
	--[
		Param( "tblidx", 3371 )
	--]
	End()
	
	-- 포로 방향으로(덴덴 방향) 한 발짝 이동
	Action( "point move" )
	--[
		Param( "loc x", -398.01 )
		Param( "loc y", 144.86 )
		Param( "loc z", 718.66 )
		Param( "dir x", 0.41 )
		Param( "dir z", -0.91 )
		Param( "run mode", 0 )
	--]
	End()
	
	-- wps에 알려줌
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 990 )
	--]
	End()
	
	-- 가멜란 포로흡수
	Action( "direct play" )
	--[
		Param( "tblidx", 60064 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()




Scene( 4, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	-- 나를 짜증나게 하는군.
	Action( "say" )
	--[
		Param( "tblidx", 3380 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", -399.25 )
		Param( "loc y", 144.86 )
		Param( "loc z", 719.4 )
		Param( "dir x", -0.91 )
		Param( "dir z", -0.41 )
		Param( "run mode", 1 )
	--]
	End()
	
	-- 포로를 본다(그런데 현재 rotate작동하지 않음)
	Action( "rotate" )
	--[
		Param( "dir x", -0.91 )
		Param( "dir z", -0.41 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()
	
	-- 공포에 벌벌 떨게 해주마.
	Action( "say" )
	--[
		Param( "tblidx", 3372 )
	--]
	End()
	
	-- 포로 방향(장로방향)으로 한 발짝 이동
	Action( "point move" )
	--[
		Param( "loc x", -400.53 )
		Param( "loc y", 144.86 )
		Param( "loc z", 718.96 )
		Param( "dir x", -1 )
		Param( "dir z", 0.07 )
		Param( "run mode", 0 )
	--]
	End()
	
	-- wps에 알려줌
	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 991 )
	--]
	End()
	
	-- 가멜란 포로흡수
	Action( "direct play" )
	--[
		Param( "tblidx", 60064 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()