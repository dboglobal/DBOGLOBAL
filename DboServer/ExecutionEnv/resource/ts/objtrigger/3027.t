CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3027;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 254;
			prelnk = "9;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 2175;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 0;
				oidx = 2181;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 20000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "10;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 2175;
				osh_sh = 255;
				widx = 1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 2181;
				osh_sh = 0;
				widx = 1;
				osh_uc = 1;
				taid = 2;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 2175;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActCustomEvt
			{
				ceid = 3021;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckCustomEvent
			{
				qid = 3021;
			}
			CDboTSClickObject
			{
				objidx = "2181;";
				widx = 1;
			}
		}
	}
}

