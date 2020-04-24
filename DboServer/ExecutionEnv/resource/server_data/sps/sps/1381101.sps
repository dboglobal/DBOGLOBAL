-- DWC1
-- 1381101: 호위 부르마. 피격있고, LP바 있음.

-- 부르마 escort
Scene( 1, "70001" )
--[
	-- npc ui 안나오도록 함
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	-- 호위 부르마 escort 하는 sps 진입 완료된 것 확인
	Action( "send event to wps" )
	--[
		Param( "wps index", 70001 )
		Param( "event id", 634 )
	--]
	End()

	-- 이렇게 하면 npc가 target follow하는 중에 say할 수 있다고 주형씨가 그랬는데 작동하지 않음
	-- 확인해볼것
	Action( "function" )
	--[
		Condition( "child" )
		--[
			-- 부르마가 늑대 만나면
			Action( "wait" )
			--[
				Condition( "recv event from wps" )
				--[
					Param( "wps index", 70003 )
					Param( "event id", 834 )
				--]
				End()
			--]
			End()
			
			-- 으악~! 무서운 늑대다!!
			Action( "say" )
			--[
				Param( "tblidx", 3351 )
			--]
			End()
			
			Action( "wait" )
			--[
				Param( "time", 3 )
			--]
			End()
		--]
		End()
		
		Condition( "child" )
		--[
			-- 부르마가 공룡 만나면
			Action( "wait" )
			--[
				Condition( "recv event from wps" )
				--[
					Param( "wps index", 70004 )
					Param( "event id", 835 )
				--]
				End()
			--]
			End()
			
			-- 저리 가! 이 괴물!
			Action( "say" )
			--[
				Param( "tblidx", 3352 )
			--]
			End()
			
			Action( "wait" )
			--[
				Param( "time", 3 )
			--]
			End()
		--]
		End()

		-- escort 대기
		Action( "escort" )
		--[
			Param( "type", "target follow" )
			Param( "dest x", 0 )
			Param( "dest z", 0 )
			Param( "radius", 20 )
			Param( "event id", 633 )
		--]
		End()

	--]
	End()



	Action( "sps end" )
	--[
	--]
	End()
--]
End()

-- 부르마 소리지름
Scene( 2, "70001" )
--[
	-- 꺄악~! 괴물이 나타났다~!
	Action( "say" )
	--[
		Param( "tblidx", 3350 )
	--]
	End()

	-- 3초 기다렸다가
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	-- 꺄악~! 괴물이 나타났다~! 한번 더 말하고
	Action( "say" )
	--[
		Param( "tblidx", 3350 )
	--]
	End()

	-- sps 종료
	Action( "sps end" )
	--[
	--]
	End()
--]
End()