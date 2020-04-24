CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 762;
	title = 76202;

	CNtlTSGroup
	{
		gid = 0;

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
			cid = 7;
			elnk = 202;
			nextlnk = 202;
			prelnk = "6;";

			CDboTSActObjWPS
			{
				sid = 762;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 203;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 10;
			prelnk = "202;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 547;
				osh_sh = 0;
				widx = 1;
				osh_uc = 255;
				taid = 1;
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
				mstate = 2;
				oidx = 547;
				osh_sh = 1;
				widx = 1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 547;
				osh_sh = 1;
				widx = 1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 8;
			prelnk = "7;7;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 180000;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "10;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 547;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 76207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 762;
			}
			CDboTSClickObject
			{
				objidx = "547;";
				widx = 1;
			}
		}
	}
}

