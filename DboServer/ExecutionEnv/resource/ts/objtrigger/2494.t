CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2494;
	title = 249402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 13;
			prelnk = "12;";

			CDboTSActObjState
			{
				mstate = 3;
				oidx = 203;
				osh_sh = 255;
				widx = 14;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 12;
			prelnk = "11;";

			CDboTSActCustomEvt
			{
				ceid = 249401;
				taid = 2;
			}
			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 249418;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 254;
			prelnk = "13;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 203;
				osh_sh = 255;
				widx = 14;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 14;
			prelnk = "16;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 16;
			prelnk = "15;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 30000;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 15;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = 203;
				osh_sh = 255;
				widx = 14;
				osh_uc = 255;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 203;
				osh_sh = 255;
				widx = 14;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;14;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 10;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 249407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckCustomEvent
			{
				qid = 2494;
			}
			CDboTSClickObject
			{
				objidx = "203;";
				widx = 14;
			}
		}
	}
}

