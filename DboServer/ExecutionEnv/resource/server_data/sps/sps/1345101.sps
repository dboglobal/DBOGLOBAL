Scene(1, "1880")
--[
	Action( "char condition")
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	Action( "say" )
	--[
		Param( "tblidx", 3047 )
	--]     
	End()
	
	Action( "wait" )
	--[
		Param( "time", 2 )    
	--]
	End()

	--텍스트 추가
	Action( "say" )
	--[
		Param( "tblidx", 3048 )           
	--]
	End()	

	Action( "path move" )
	--[
		Param( "tblidx", 13451011 )
		Param( "run mode", 1 )
	--]
	End()

	--WPS로 EventId 보냄 (몹 스폰 요청) A
	Action( "send event to wps" )
	--[
		Param( "wps index", 1880 )
		Param( "event id", 378 )
	--]
	End()

	--몹 스폰 확인 신호 대기 B
	Action( "wait" )
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1880 )
			Param("event id", 379 )
		--]
		End()
	--]
	End()

	--텍스트 (레드팬츠군 발견! 레드팬츠군 발견!)
	Action( "say")
	--[
		Param( "tblidx", 3049 )           
	--]     
	End()
	
	--SPS를 받을 때까지 대기 (진행 계속 신호) C
	Action( "wait" )
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1880 )
			Param("event id", 380 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3050 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param("tblidx", 13451012)
		Param("run mode", 1 )
	--]
	End()

	--꺾어지는 부분

	-- 텍스트 (적. 주요진지. 발견. 행로. 재설정.)
	Action( "say" )
	--[
		Param( "tblidx", 3122 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 1 )    
	--]
	End()

	Action( "path move" )
	--[
		Param("tblidx", 13451014)
		Param("run mode", 1 )
	--]
	End()

	-- WPS로 EventId 보냄 (몹 스폰 요청) D
	Action( "send event to wps" )
	--[
		Param( "wps index", 1880) 
		Param( "event id", 381 )
	--]
	End()

	--몹 스폰 확인 신호 대기 E
	Action( "wait" )
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1880 )
			Param("event id", 382 )
		--]
		End()
	--]
	End()

	-- 텍스트 (레드팬츠군. 발견! 복제품!  파괴해야. 한다. 복제품!)
	Action( "say" )
	--[
		Param( "tblidx", 3051)
	--]     
	End()
	
	-- SPS를 받을 때까지 대기 (진행 계속 신호) F
	Action( "wait" )
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1880)
			Param("event id", 383)
		--]
		End()
	--]
	End()
	
	-- 텍스트 추가
	Action( "say" )
	--[
		Param( "tblidx", 3052 )           
	--]     
	End()
	
	Action( "wait" )
	--[
		Param( "time", 2 )    
	--]
	End()

	--에스코트 종료	(종료신호) G
	Action( "send event to wps" )
	--[
		Param( "wps index", 1880 ) 
		Param( "event id", 384 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 13451013 )
		Param( "run mode", 1 )
	--]
	End()

	--텍스트 (메인. 프레임. 대파. 기동 불가. 기동 불가. 임무. 수행. 불능, 정비모드로. 전환.)
	Action( "say" )
	--[
		Param( "tblidx", 3123 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )    
	--]
	End()

	--SPS종료 신호 H
	Action( "send event to wps" )
	--[
		Param( "wps index", 1880 )
		Param( "event id", 385 )
	--]
	End()

	Action( "char condition")
	--[
		Param( "ui disable", 0 )
	--]
	End()

	Action( "leave" )
	--[
	--]
	End()
--이 지점에서 PC대사 넣어줄 것
--]
End()
