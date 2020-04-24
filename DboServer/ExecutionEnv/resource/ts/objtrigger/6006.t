CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6006;
	title = 600602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 7;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 253;
			prelnk = "8;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 6;
				osh_sh = 0;
				widx = 800000;
				osh_uc = 0;
				taid = 2;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 7;
				osh_sh = 0;
				widx = 800000;
				osh_uc = 0;
				taid = 1;
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
				tblidx = 10000;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 8;
			prelnk = "4;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 6;
				osh_sh = 0;
				widx = 800000;
				osh_uc = 1;
				taid = 2;
			}
			CDboTSActObjState
			{
				mstate = 255;
				oidx = 7;
				osh_sh = 0;
				widx = 800000;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 409;
				tblidx = 6025;
				stradius = 0;
				stype = 4;
				taid = 3;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 199;
				tblidx = -1;
				stradius = 0;
				stype = 4;
				taid = 4;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 253;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "7;2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 99082;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 600607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 6025;
			}
			CDboTSClickObject
			{
				objidx = "6;";
				widx = 800000;
			}
		}
	}
}

