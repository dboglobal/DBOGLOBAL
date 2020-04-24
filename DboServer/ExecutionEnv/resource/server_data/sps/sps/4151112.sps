
Scene(1, "2400" )
--[
        Action( "wait" )
        --[
                Condition( "recv event from item" )
                --[
                        Param( "item index", 98018 )
                --]
                End()
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3282 )
        --]
        End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2400 )
                Param( "event id", 539 )
        --]
        End()

        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()