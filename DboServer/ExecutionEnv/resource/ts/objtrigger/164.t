CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 164;
	title = 16402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 22;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActSendSvrEvt
			{
				id = 69;
				tblidx = -1;
				stradius = 200;
				stype = 3;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 25;
			elnk = 255;
			nextlnk = 26;
			prelnk = "24;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 10000;
			}
		}
		CDboTSContGAct
		{
			cid = 26;
			elnk = 255;
			nextlnk = 254;
			prelnk = "25;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 413;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 24;
			elnk = 255;
			nextlnk = 25;
			prelnk = "23;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 413;
				osh_sh = 0;
				widx = 1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 23;
			elnk = 255;
			nextlnk = 24;
			prelnk = "22;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 23;
			prelnk = "5;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 16418;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;26;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 16407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "413;";
				widx = 1;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 164;
			}
		}
	}
}

