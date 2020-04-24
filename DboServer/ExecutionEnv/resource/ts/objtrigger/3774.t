CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3774;
	title = 377402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 20;
			elnk = 255;
			nextlnk = 5;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 377418;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 3774;
			}
			CDboTSClickObject
			{
				objidx = "57;58;59;60;61;62;63;64;65;66;";
				widx = 15;
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
			cid = 4;
			elnk = 255;
			nextlnk = 20;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1631;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "20;";

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
				oidx = -1;
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 10;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 20000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;10;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
	}
}

